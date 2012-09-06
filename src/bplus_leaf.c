/**
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "bplus_leaf.h"

BplusLeaf* bplus_leaf_new(BplusTree* tree)
{
    g_return_val_if_fail(tree != NULL, NULL);

    BplusLeaf* leaf = g_slice_new(BplusLeaf);

    bplus_node_init(&leaf->node, TRUE);
    leaf->left  = NULL;
    leaf->right = NULL;

#ifdef BPLUS_TREE_GATHER_STATS
    tree->node_count++;
    tree->leaf_count++;
    tree->underflow_count++;
#endif /* ifdef BPLUS_TREE_GATHER_STATS */

    return leaf;
}

BplusLeaf* bplus_leaf_new_right(BplusTree* tree, BplusLeaf* leaf_left)
{
    g_return_val_if_fail(tree != NULL, NULL);
    g_return_val_if_fail(leaf_left != NULL, NULL);

    BplusLeaf* leaf_right = bplus_leaf_new(tree);

    leaf_right->left  = leaf_left;
    leaf_right->right = leaf_left->right;
    leaf_left->right  = leaf_right;

    if (leaf_right->right != NULL)
        leaf_right->right->left = leaf_right;

    if (tree->last == leaf_left)
        tree->last = leaf_right;

    return leaf_right;
}

void bplus_leaf_destroy(BplusTree* tree, BplusLeaf* leaf)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(leaf != NULL);

    if (leaf->left != NULL)
        leaf->left->right = leaf->right;

    if (leaf->right != NULL)
        leaf->right->left = leaf->left;

    if (tree->first == leaf)
        tree->first = leaf->right;

    if (tree->last == leaf)
        tree->last = leaf->left;

#ifdef BPLUS_TREE_GATHER_STATS
    tree->node_count--;
    tree->leaf_count--;
    tree->underflow_count--;
#endif /* ifdef BPLUS_TREE_GATHER_STATS */

    g_slice_free(BplusLeaf, leaf);
}
