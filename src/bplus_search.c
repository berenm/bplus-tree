#include "bplus_search.h"

#include "bplus_tree_private.h"
#include "bplus_leaf.h"

static BplusNode* bplus_tree_leaf_get_not_empty(BplusNode* leaf)
{
    /* Iterate over the leaf link list until leaf is null or size is not 0 */
    while (leaf && leaf->size == 0)
        leaf = leaf->leaf_link->right;

    return leaf;
}

static BplusNode* bplus_tree_get_leaf(BplusTree const* tree, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, NULL);

    /* Returns the first leaf that may contain the given key */
    BplusNode* node = tree->root;
    while (node && !bplus_node_is_leaf(node))
    {
        size_t const index = bplus_node_get_key_index_first_or_before(node, key);
        node = (BplusNode*) node->items[index].data;
    }

    return node;
}

static BplusNode* bplus_tree_find_leaf_with_key(BplusTree const* tree, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, NULL);

    /* Returns the first leaf that actually contains some item with the given key */
    BplusNode* leaf = bplus_tree_leaf_get_not_empty(bplus_tree_get_leaf(tree, key));
    while (leaf)
    {
        if (leaf->items[0].key <= key)
        {
            if (bplus_tree_leaf_find_key(leaf, key) < leaf->size)
                return leaf;

        }
        else
        {
            break;
        } leaf = bplus_tree_leaf_get_not_empty(leaf->leaf_link->right);
    }

    return 0;
}

static BplusNode* bplus_tree_find_leaf_with_key_data(BplusTree const* tree, BplusKey const key, void* data)
{
    g_return_val_if_fail(tree != NULL, NULL);

    /* Returns the first leaf that actually contains some item with the given key and given data */
    BplusNode* leaf = bplus_tree_leaf_get_not_empty(bplus_tree_get_leaf(tree, key));
    while (leaf)
    {
        if (leaf->items[0].key <= key)
        {
            if (bplus_tree_leaf_find_key_data(leaf, key, data) < leaf->size)
                return leaf;

        }
        else
        {
            break;
        } leaf = bplus_tree_leaf_get_not_empty(leaf->leaf_link->right);
    }

    return 0;
}

static BplusItem const* bplus_tree_leaf_get_next_to_last_with_key(BplusTree const* tree, BplusKey const key)
{
    BplusItem const* end      = 0;
    BplusNode const* end_leaf = bplus_tree_get_leaf(tree, key);

    if (end_leaf)
    {
        size_t end_index = bplus_node_get_key_index_after_last(end_leaf, key);
        if (end_index < end_leaf->size)
        {
            end = end_leaf->items + end_index;
        }
        else
        {
            end_leaf = end_leaf->leaf_link->right;
            if (end_leaf)
                end = end_leaf->items;

        }
    }

    return end;
}

static BplusItem const* bplus_tree_at(BplusTree const* tree, size_t const index)
{
    g_return_val_if_fail(tree != NULL, NULL);

    /* Returns the item at the given index in the leaf linked list */
    size_t     data_index = index;
    BplusNode* leaf       = bplus_tree_leaf_get_not_empty(tree->first);
    while (leaf && data_index >= leaf->size)
    {
        data_index -= leaf->size;
        leaf        = bplus_tree_leaf_get_not_empty(leaf->leaf_link->right);
    }

    if (!leaf)
        return 0;

    else
        return leaf->items + data_index;

}

static BplusItem const* bplus_tree_get(BplusTree const* tree, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, NULL);

    /* Try to find the first item with the given key */
    BplusNode* leaf = bplus_tree_leaf_get_not_empty(bplus_tree_get_leaf(tree, key));
    if (leaf)
    {
        size_t index = bplus_tree_leaf_find_key(leaf, key);

        if (index < leaf->size)
            return leaf->items + index;

    }

    return 0;
}
