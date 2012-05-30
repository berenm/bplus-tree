#include "bplus_insert.h"

#include "bplus_search.h"
#include "bplus_rebalance.h"
#include "bplus_leaf.h"

static void bplus_node_insert_after(BplusTree* tree, BplusNode* parent, BplusKey const right_key, BplusNode* right_node, BplusNode* left_node)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(parent != NULL);
    g_return_if_fail(right_node != NULL);
    g_return_if_fail(left_node != NULL);

    // g_debug("insert %lu=>%p after %lu=>%p to node %p (k: %lu, size: %lu, keys: %lu,%lu,%lu,%lu)",
    //         right_key, right_node, left_node->key, left_node, parent, parent->key, parent->size,
    //         parent->items[0].key, parent->items[1].key, parent->items[2].key, parent->items[3].key);

    /* Inserts the given right_node immediately after the position of (left_node->key, left_node) */
    size_t index = bplus_tree_leaf_find_key_data(parent, left_node->key, left_node);
    g_return_if_fail(index < parent->size);
    index++;

    bplus_node_move_and_resize_data(parent, index + 1, index);
    parent->items[index].key  = right_key;
    parent->items[index].data = right_node;

    if (parent->size >= BPLUS_TREE_ORDER)
        bplus_node_split(tree, parent);

}

static void bplus_node_insert(BplusTree* tree, BplusNode* node, BplusKey const key, void* data)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(node != NULL);

    // g_debug("insert %lu=>%p to node %p (k: %lu, size: %lu, keys: %lu,%lu,%lu,%lu)", key, data, node, node->key, node->size,
    //         node->items[0].key, node->items[1].key, node->items[2].key, node->items[3].key);

    /* Inserts the given (key, data) after the last item with a key lesser or equals to key */
    size_t const index = bplus_node_get_key_index_after_last(node, key);
    bplus_node_move_and_resize_data(node, index + 1, index);
    node->items[index].key  = key;
    node->items[index].data = data;

    if (node->size >= BPLUS_TREE_ORDER)
        bplus_node_split(tree, node);

}

static void bplus_tree_insert(BplusTree* tree, BplusKey const key, void* data)
{
    g_return_if_fail(tree != NULL);

    bplus_node_insert(tree, bplus_tree_get_leaf(tree, key), key, data);
}
