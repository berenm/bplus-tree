/**
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "bplus_foreach.h"

#include "bplus_node.h"

void bplus_foreach_item_in_node(BplusTree* tree, BplusNode* node, BplusForeachItemFunc* foreach, void* argument)
{
    if (!node->is_leaf)
        for (size_t i = 0; i < node->length; ++i)
            bplus_foreach_item_in_node(tree, bplus_node_at(node, i), foreach, argument);

    else
        for (size_t i = 0; i < node->length; ++i)
            foreach(tree, node->items + i, argument);

}

void bplus_foreach_item_in_tree(BplusTree* tree, BplusForeachItemFunc* foreach, void* argument)
{
    bplus_foreach_item_in_node(tree, tree->root, foreach, argument);
}

void bplus_foreach_node_in_node(BplusTree* tree, BplusNode* node, BplusForeachNodeFunc* foreach, void* argument)
{
    if (!node->is_leaf)
        for (size_t i = 0; i < node->length; ++i)
            bplus_foreach_node_in_node(tree, bplus_node_at(node, i), foreach, argument);

    foreach(tree, node, argument);
}

void bplus_foreach_node_in_tree(BplusTree* tree, BplusForeachNodeFunc* foreach, void* argument)
{
    bplus_foreach_node_in_node(tree, tree->root, foreach, argument);
}
