#include "bplus_node.h"

#include "bplus_tree_private.h"
#include "bplus_rebalance.h"

#include <string.h>

static BplusNode* bplus_node_create(BplusTree* tree, uint8_t const create_leaf, BplusNode* left_leaf)
{
    g_return_val_if_fail(tree != NULL, NULL);
    g_return_val_if_fail(!create_leaf || left_leaf != NULL || bplus_node_is_leaf(left_leaf), NULL);

    BplusNode* node = g_slice_new(BplusNode);
    // g_debug("create node %p tree %p", node, tree);

    node->key       = 0;
    node->size      = 0;
    node->parent    = 0;
    node->leaf_link = 0;

    if (create_leaf)
    {
        node->leaf_link        = g_slice_new(BplusLeafLink);
        node->leaf_link->right = 0;
        node->leaf_link->left  = 0;

        if (left_leaf != NULL)
        {
            BplusNode* right_leaf = left_leaf->leaf_link->right;
            if (right_leaf != NULL)
            {
                node->leaf_link->right      = right_leaf;
                right_leaf->leaf_link->left = node;
            }

            node->leaf_link->left       = left_leaf;
            left_leaf->leaf_link->right = node;

            if (tree->last == left_leaf)
                tree->last = node;

        }
    }

    return node;
}

static void bplus_node_destroy(BplusTree* tree, BplusNode* node)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(node != NULL);
    // g_debug("destroy node %p tree %p", node, tree);

    if (node->leaf_link)
    {
        BplusNode* left_leaf  = node->leaf_link->left;
        BplusNode* right_leaf = node->leaf_link->right;

        /* Update leaf link list if needed */
        if (left_leaf)
        {
            left_leaf->leaf_link->right = right_leaf;

            if (tree->last == node)
                tree->last = left_leaf;

        }
        if (right_leaf)
        {
            right_leaf->leaf_link->left = left_leaf;

            if (tree->first == node)
                tree->first = right_leaf;

        }

        g_slice_free(BplusLeafLink, node->leaf_link);
    }

    g_slice_free(BplusNode, node);
}

static uint8_t bplus_node_is_leaf(BplusNode const* node)
{
    g_return_val_if_fail(node != NULL, 0);

    /* Leaf nodes do have a leaf link */
    return node->leaf_link != NULL;
}

static void bplus_node_move_and_resize_data(BplusNode* node, size_t const to_offset, size_t const from_offset)
{
    g_return_if_fail(node != NULL);
    g_return_if_fail(from_offset <= node->size);

    /* Move data if needed and if the source offset is inside the current data */
    if ((from_offset != to_offset) && (from_offset < node->size))
        memmove(node->items + to_offset, node->items + from_offset, (node->size - from_offset) * sizeof(*node->items));

    /* Update the node size accordingly */
    if (from_offset < to_offset)
        node->size += to_offset - from_offset;

    else if (from_offset > to_offset)
        node->size -= from_offset - to_offset;

    g_warn_if_fail(node->size <= BPLUS_TREE_ORDER);
}

static void bplus_node_copy_data(BplusNode* to_node, size_t const to_offset, BplusNode* from_node, size_t const from_offset, size_t count)
{
    g_return_if_fail(to_node != from_node);

    /* Make room in the destination node */
    bplus_node_move_and_resize_data(to_node, to_offset + count, to_offset);

    /* Copy data from source node to destination node */
    memcpy(to_node->items + to_offset, from_node->items + from_offset, count * sizeof(*from_node->items));

    /* Shrink the source node */
    bplus_node_move_and_resize_data(from_node, from_offset, from_offset + count);

    /* Update the destination node's child parent links */
    bplus_node_update_parent(to_node);
}

static size_t bplus_node_get_key_index_first_or_before(BplusNode const* node, BplusKey const key)
{
    g_return_val_if_fail(node != NULL, -1);

    /* Few obvious cases that need to be resolved so we can start iterating at the middle */
    if (node->items[0].key >= key)
        return 0;

    if (node->items[node->size - 1].key < key)
        return node->size - 1;

    /* Find the index of the first item that has the given key, or of the last one with a smaller key */
    BplusItem const* const begin = node->items;
    BplusItem const* const end   = begin + node->size;
    BplusItem const*       item  = begin;
    while (item < end)
    {
        if (item->key >= key)
            break;

        ++item;
    }

    if ((item == end) || ((item > begin) && (item->key > key)))
        --item;

    // g_debug("find first or before %lu at %zu in node %p (k: %lu, size: %lu, keys: %lu,%lu,%lu,%lu)",
    //         key, item - begin, node, node->key, node->size,
    //         node->items[0].key, node->items[1].key, node->items[2].key, node->items[3].key);
    return item - begin;
}

static size_t bplus_node_get_key_index_after_last(BplusNode const* node, BplusKey const key)
{
    g_return_val_if_fail(node != NULL, -1);

    /* Few obvious cases that need to be resolved so we can start iterating at the middle */
    if (node->items[0].key > key)
        return 0;

    if (node->items[node->size - 1].key <= key)
        return node->size;

    /* Find the index of the first item that has a key greater than the given key */
    BplusItem const* const begin = node->items;
    BplusItem const* const end   = begin + node->size;
    BplusItem const*       item  = begin;
    while (item < end)
    {
        if (item->key > key)
            break;

        ++item;
    }

    // g_debug("find after last %lu at %zu in node %p (k: %lu, size: %lu, keys: %lu,%lu,%lu,%lu)",
    //         key, item - begin, node, node->key, node->size,
    //         node->items[0].key, node->items[1].key, node->items[2].key, node->items[3].key);
    return item - begin;
}

static size_t bplus_tree_leaf_find_key(BplusNode const* node, BplusKey const key)
{
    g_return_val_if_fail(node != NULL, -1);

    /* Return the index of the first item with the given key, or node->size if key is not found */
    size_t const index = bplus_node_get_key_index_first_or_before(node, key);
    if (node->items[index].key == key)
        return index;

    return node->size;
}

static size_t bplus_tree_leaf_find_key_data(BplusNode const* node, BplusKey const key, void const* data)
{
    g_return_val_if_fail(node != NULL, -1);

    /* Return the index of the first item with the given key and given data, or node->size if key is not found */
    BplusItem const* const begin = node->items;
    BplusItem const* const end   = begin + node->size;
    BplusItem const*       item  = begin + bplus_tree_leaf_find_key(node, key);
    while (item < end && item->key == key && item->data != data)
        ++item;

    if ((item < end) && (item->key == key))
    {
        // g_debug("find %lu=>%p at %zu in node %p (k: %lu, size: %lu, keys: %lu,%lu,%lu,%lu)",
        //         key, data, item - begin, node, node->key, node->size,
        //         node->items[0].key, node->items[1].key, node->items[2].key, node->items[3].key);
        return item - begin;
    }

    return node->size;
}
