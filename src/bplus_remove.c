#include "bplus_remove.h"

#include "bplus_tree_private.h"
#include "bplus_node.h"
#include "bplus_leaf.h"
#include "bplus_search.h"
#include "bplus_rebalance.h"

static void bplus_node_remove_range(BplusTree*   tree,
                             BplusNode*   node,
                             size_t const index_from,
                             size_t const index_to)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(node != NULL);
    g_return_if_fail(index_to > index_from);
    g_return_if_fail(index_to <= node->size);

    // g_debug("remove [%lu,%lu[ from node %p (k: %lu, size: %lu, keys: %lu,%lu,%lu,%lu)",
    //         index_from, index_to, node, node->key, node->size,
    //         node->items[0].key, node->items[1].key, node->items[2].key, node->items[3].key);

    /* Shrink the current' node data */
    bplus_node_move_and_resize_data(node, index_from, index_to);
    if (index_from == 0)
        /* If we've erased the current node first children, we need to update the lesser keys */
        bplus_node_propagate_minimal_key(node);

    if (node == tree->root)
        return;

    /* If the node is empty, try to collapse the tree */
    if (node->size == 0)
    {
        if (node->key > 0)
        {
            bplus_tree_leaf_remove_key_data(tree, node->parent, node->key, node);
            bplus_node_destroy(tree, node);
        }

        bplus_node_collapse_root(tree);
    }
}

static void* bplus_tree_leaf_remove_key_first(BplusTree* tree, BplusNode* node, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, NULL);
    g_return_val_if_fail(node != NULL, NULL);

    // g_debug("remove first %lu from node %p (k: %lu, size: %lu, keys: %lu,%lu,%lu,%lu)",
    //         key, node, node->key, node->size,
    //         node->items[0].key, node->items[1].key, node->items[2].key, node->items[3].key);

    /* Remove the first element with the given key */
    size_t index = bplus_tree_leaf_find_key(node, key);
    void*  data  = NULL;
    if (index < node->size)
    {
        data = node->items[index].data;
        bplus_node_remove_range(tree, node, index, index + 1);
    }

    return data;
}

static void bplus_tree_leaf_remove_key_data(BplusTree* tree, BplusNode* node, BplusKey const key, void const* data)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(node != NULL);

    // g_debug("remove %lu=>%p from node %p (k: %lu, size: %lu, keys: %lu,%lu,%lu,%lu)",
    //         key, data, node, node->key, node->size,
    //         node->items[0].key, node->items[1].key, node->items[2].key, node->items[3].key);

    /* Remove the first item with the given (key, data) */
    size_t index = bplus_tree_leaf_find_key_data(node, key, data);
    if (index < node->size)
        bplus_node_remove_range(tree, node, index, index + 1);

}

static void bplus_tree_remove(BplusTree* tree, BplusKey const key, void* data)
{
    g_return_if_fail(tree != NULL);

    /* Try to find the first item with the given (key, data) and remove it from the tree */
    BplusNode* leaf = bplus_tree_find_leaf_with_key_data(tree, key, data);
    if (leaf)
        bplus_tree_leaf_remove_key_data(tree, leaf, key, data);

}

static void* bplus_tree_remove_first(BplusTree* tree, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, NULL);

    /* Try to find the first item with the given key and remove it from the tree */
    BplusNode* leaf = bplus_tree_find_leaf_with_key(tree, key);
    if (leaf)
        return bplus_tree_leaf_remove_key_first(tree, leaf, key);

    return NULL;
}

static void bplus_tree_remove_all(BplusTree* tree, BplusKey const key)
{
    g_return_if_fail(tree != NULL);

    /* Try to find the all items with the given key and remove them from the tree */
    BplusNode* leaf = bplus_tree_find_leaf_with_key(tree, key);
    while (leaf && leaf->items[0].key <= key)
    {
        size_t index_from = bplus_tree_leaf_find_key(leaf, key);
        size_t index_to   = bplus_node_get_key_index_after_last(leaf, key);

        bplus_node_remove_range(tree, leaf, index_from, index_to);
        leaf = bplus_tree_leaf_get_not_empty(leaf->leaf_link->right);
    }
}
