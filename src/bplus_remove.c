/**
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "bplus_remove.h"

#include "bplus_node.h"
#include "bplus_search.h"
#include "bplus_rebalance.h"

void bplus_node_remove_at(BplusTree const* tree, BplusNode* node, size_t const index, size_t const length)
{
    g_return_if_fail(node != NULL);
    g_return_if_fail(index < node->length);
    g_return_if_fail(index + length <= node->length);

    bplus_node_move_and_resize_data(tree, node, index, index + length);
}

BplusValue bplus_tree_remove_first(BplusTree* tree)
{
    BplusPath    path  = { .leaf = (BplusNode*) tree->first };
    BplusNode*   node  = (BplusNode*) path.leaf;
    size_t const index = path.index[0];

    BplusValue value = bplus_value_at(node, 0);
    bplus_node_remove_at(tree, node, index, 1);
    if (index == 0)
        bplus_rebalance_propagate(tree, &path);

    if (bplus_node_underfilled(node))
        bplus_rebalance_underfilled(tree, &path);

    return value;
}

BplusValue bplus_tree_remove(BplusTree* tree, BplusKey const key)
{
    BplusPath path;
    bplus_tree_get_path_to_key(tree, key, &path);

    size_t const index = path.index[0];
    BplusNode*   node  = (BplusNode*) path.leaf;

    if (bplus_key_eq(tree, bplus_key_at(node, index), key))
    {
        BplusValue value = bplus_value_at(node, index);
        bplus_node_remove_at(tree, node, index, 1);
        if (index == 0)
            bplus_rebalance_propagate(tree, &path);

        if (bplus_node_underfilled(node))
            bplus_rebalance_underfilled(tree, &path);

        return value;
    }

    return NULL;
}
