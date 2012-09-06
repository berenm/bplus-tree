/**
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "bplus_node.h"

#include "bplus_leaf.h"

#include <string.h>

BplusNode* bplus_node_new(BplusTree* tree)
{
    g_return_val_if_fail(tree != NULL, NULL);

    BplusNode* node = g_slice_new(BplusNode);

    bplus_node_init(node, FALSE);

#ifdef BPLUS_TREE_GATHER_STATS
    tree->node_count++;
    tree->underflow_count++;
#endif /* ifdef BPLUS_TREE_GATHER_STATS */

    return node;
}

BplusNode* bplus_node_new_right(BplusTree* tree, BplusNode* node_left)
{
    g_return_val_if_fail(tree != NULL, NULL);
    g_return_val_if_fail(node_left != NULL, NULL);

    BplusNode* node_right;

    if (node_left->is_leaf)
        node_right = (BplusNode*) bplus_leaf_new_right(tree, (BplusLeaf*) node_left);
    else
        node_right = bplus_node_new(tree);

    node_right->parent = node_left->parent;

    return node_right;
}

void bplus_node_init(BplusNode* node, gboolean is_leaf)
{
    g_return_if_fail(node != NULL);

    node->parent  = NULL;
    node->is_leaf = is_leaf;

    node->length = 0;
}

void bplus_node_destroy(BplusTree* tree, BplusNode* node)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(node != NULL);

    if (node->is_leaf)
    {
        bplus_leaf_destroy(tree, (BplusLeaf*) node);
    }
    else
    {
#ifdef BPLUS_TREE_GATHER_STATS
        tree->node_count--;
        tree->underflow_count--;
#endif /* ifdef BPLUS_TREE_GATHER_STATS */

        g_slice_free(BplusNode, node);
    }
}

void bplus_node_move_and_resize_data(BplusTree const* tree, BplusNode* node, size_t const index_to, size_t const index_from)
{
    g_return_if_fail(node != NULL);
    g_return_if_fail(index_from <= node->length);

#ifdef BPLUS_TREE_GATHER_STATS
    int was_underfilled = bplus_node_underfilled(node);
    int was_overfilled  = bplus_node_overfilled(node);
#endif /* ifdef BPLUS_TREE_GATHER_STATS */

    int64_t const resize_length = index_to - index_from;
    if (resize_length == 0)
        return;

    int64_t const move_length = node->length - index_from;
    if (move_length > 0)
        memmove(node->items + index_to, node->items + index_from, move_length * sizeof(BplusItem));

    if (resize_length > 0)
        node->length += resize_length;

    else if (resize_length < 0)
        node->length -= -resize_length;

#ifdef BPLUS_TREE_GATHER_STATS
    if (!bplus_node_underfilled(node) && was_underfilled)
        tree->underflow_count--;
    else if (bplus_node_underfilled(node) && !was_underfilled)
        tree->underflow_count++;

    if (bplus_node_overfilled(node) && !was_overfilled)
        tree->overflow_count++;
    else if (!bplus_node_overfilled(node) && was_overfilled)
        tree->overflow_count--;
#endif /* ifdef BPLUS_TREE_GATHER_STATS */

}
