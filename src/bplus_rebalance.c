/**
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "bplus_rebalance.h"

#include "bplus_node.h"
#include "bplus_search.h"
#include "bplus_insert.h"
#include "bplus_remove.h"

void bplus_rebalance_propagate(BplusTree const* tree, BplusPath* path)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(path != NULL);

    BplusNode* node = path->leaf;
    for (size_t i = 1; i < path->length; ++i)
    {
        size_t const   index = path->index[i];
        BplusKey const key   = bplus_key_first(node);

        if (bplus_key_gt(tree, bplus_key_at(node->parent, index), key))
            bplus_key_at(node->parent, index) = key;
        else
            break;

        node = node->parent;
    }
}

static int64_t bplus_node_get_rebalance_amount(BplusTree const* tree, BplusNode const* node_left, BplusNode const* node_right)
{
    g_return_val_if_fail(tree != NULL, 0);
    g_return_val_if_fail(node_left != NULL, 0);
    g_return_val_if_fail(node_right != NULL, 0);

    size_t const total = node_left->length + node_right->length;

    /* If the nodes can be merged without overfilling */
    if (total <= (BPLUS_TREE_ORDER / 3))
        return -node_right->length;

    /* If the data can be balanced over the two without overfilling */
    else if (total < (BPLUS_TREE_ORDER * 5 / 3))
        return node_left->length - (total + 1) / 2;

    return 0;
}

static gboolean bplus_node_find_merge_candidate(BplusTree const* tree, size_t const index, BplusNode* node,
                                                BplusNode** node_left, BplusNode** node_right)
{
    g_return_val_if_fail(tree != NULL, FALSE);
    g_return_val_if_fail(node != NULL, FALSE);

    *node_left  = node;
    *node_right = node;
    if ((node->parent == NULL) || (node->parent->length <= 1))
        return FALSE;

    if (index == 0)
    {
        *node_right = bplus_node_at(node->parent, index + 1);
        if (bplus_node_get_rebalance_amount(tree, node, *node_right) == 0)
            *node_right = node;

    }
    else if (index == node->parent->length - 1)
    {
        *node_left = bplus_node_at(node->parent, index - 1);
        if (bplus_node_get_rebalance_amount(tree, *node_left, node) == 0)
            *node_left = node;

    }
    else
    {
        *node_left  = bplus_node_at(node->parent, index - 1);
        *node_right = bplus_node_at(node->parent, index + 1);

        int64_t const merge_amount_right = bplus_node_get_rebalance_amount(tree, node, *node_right);
        int64_t const merge_amount_left  = bplus_node_get_rebalance_amount(tree, *node_left, node);

        if (merge_amount_left < 0)
        {
            /* Merge with left copies data to the left.
             * Prefer a merge with right only if it copies less data from the right.
             */
            if (merge_amount_right >= 0)
                *node_right = node;
            else if (merge_amount_right < merge_amount_left)
                *node_right = node;
            else
                *node_left = node;
        }
        else if (merge_amount_left > 0)
        {
            /* Merge with left copies data from the left.
             * Prefer a merge with right if it copies less data to the right or data from right.
             */
            if (merge_amount_right == 0)
                *node_right = node;
            else if (merge_amount_right > merge_amount_left)
                *node_right = node;
            else
                *node_left = node;
        }
        else if (merge_amount_right != 0)
        {
            /* Merge with left is impossible.
             * Prefer a merge with right.
             */
            *node_left = node;
        }
        else
        {
            *node_left  = node;
            *node_right = node;
        }
    }

    return *node_left != *node_right;
} /* bplus_node_find_merge_candidate */

static size_t bplus_rebalance_data(BplusTree const* tree, BplusNode* node_left, BplusNode* node_right)
{
    g_return_val_if_fail(tree != NULL, 0);
    g_return_val_if_fail(node_left != NULL, 0);
    g_return_val_if_fail(node_right != NULL, 0);

    int64_t const amount = bplus_node_get_rebalance_amount(tree, node_left, node_right);
    g_return_val_if_fail(amount <= 0 || amount < node_left->length, 0);
    g_return_val_if_fail(amount >= 0 || -amount <= node_right->length, 0);

    if (amount > 0)
    {
        size_t const     index = node_left->length - amount;
        BplusItem const* items = node_left->items + index;

        bplus_node_insert_at(tree, node_right, 0, amount, items);
        bplus_node_remove_at(tree, node_left, index, amount);

    }
    else if (amount < 0)
    {
        size_t const     index = node_left->length;
        BplusItem const* items = node_right->items;

        bplus_node_insert_at(tree, node_left, index, -amount, items);
        bplus_node_remove_at(tree, node_right, 0, -amount);
    }

    return 0;
} /* bplus_rebalance_data */

static void bplus_rebalance_split_node(BplusTree* tree, BplusNode* node_left, size_t const index)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(node_left != NULL);

    BplusNode* const node_right = bplus_node_new_right(tree, node_left);
    bplus_rebalance_data(tree, node_left, node_right);

    BplusItem const item = { .key = bplus_key_first(node_right), .value = node_right };
    bplus_node_insert_at(tree, node_left->parent, index + 1, 1, &item);
}

static void bplus_rebalance_new_root(BplusTree* tree)
{
    g_return_if_fail(tree != NULL);

    BplusNode* const node = tree->root;
    BplusNode* const root = bplus_node_new(tree);
    bplus_key_first(root)   = bplus_key_first(node);
    bplus_value_first(root) = node;
    root->length            = 1;
    node->parent            = root;
    tree->root              = root;
    tree->height++;
}

static int bplus_rebalance_try_merge(BplusTree* tree, BplusNode* node, size_t const index)
{
    g_return_val_if_fail(tree != NULL, 0);
    g_return_val_if_fail(node != NULL, 0);

    BplusNode* node_left;
    BplusNode* node_right;
    if (!bplus_node_find_merge_candidate(tree, index, node, &node_left, &node_right))
        return 0;

    size_t const index_right = (node == node_left) ? index + 1 : index;
    bplus_rebalance_data(tree, node_left, node_right);

    if (node_right->length == 0)
    {
        bplus_node_remove_at(tree, node->parent, index_right, 1);
        bplus_node_destroy(tree, node_right);
    }
    else
    {
        bplus_key_at(node->parent, index_right) = bplus_key_first(node_right);
    }

    return 1;
}

void bplus_rebalance_overfilled(BplusTree* tree, BplusPath const* path)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(path != NULL);

    BplusNode* node = (BplusNode*) path->leaf;
    for (size_t i = 1; i < path->length; ++i)
    {
        if (!bplus_node_overfilled(node))
            break;

        size_t const index = path->index[i];
        if (!bplus_rebalance_try_merge(tree, node, index))
            bplus_rebalance_split_node(tree, node, index);

        node = node->parent;
    }

    if (bplus_node_overfilled(node))
    {
        bplus_rebalance_new_root(tree);
        bplus_rebalance_split_node(tree, node, 0);
    }
}

static void bplus_rebalance_shrink_tree(BplusTree* tree)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(tree->root != NULL);

    size_t i = 0;
    for (; i < tree->height; ++i)
    {
        if (tree->root->length != 1)
            break;
        if (tree->root->is_leaf)
            break;

        BplusNode* node = tree->root;
        tree->root         = bplus_node_first(tree->root);
        tree->root->parent = NULL;
        node->length       = 0;
        bplus_node_destroy(tree, node);
    }

    tree->height -= i;
}

void bplus_rebalance_underfilled(BplusTree* tree, BplusPath const* path)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(path != NULL);

    BplusNode* node = (BplusNode*) path->leaf;
    for (size_t i = 1; i < path->length; ++i)
    {
        if (!bplus_node_underfilled(node))
            break;

        size_t const index = path->index[i];
        if (!bplus_rebalance_try_merge(tree, node, index) && (node->length == 0))
        {
            bplus_node_remove_at(tree, node->parent, index, 1);
            bplus_node_destroy(tree, node);
        }

        node = node->parent;
    }

    bplus_rebalance_shrink_tree(tree);
}
