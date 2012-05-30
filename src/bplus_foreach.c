#include "bplus_foreach.h"

#include "bplus_tree_private.h"

static void bplus_foreach_node_in_node(BplusTree const* tree, BplusNode* node, BplusForeach* foreach, void* argument)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(node != NULL);
    g_return_if_fail(foreach != NULL);

    if (!bplus_node_is_leaf(node))
        for (size_t i = 0; i < node->size; ++i)
            if (node->items[i].data)
                bplus_foreach_node_in_node(tree, (BplusNode*) node->items[i].data, foreach, argument);

    foreach(node, argument);
}

static void bplus_foreach_node_in_tree(BplusTree const* tree, BplusForeach* foreach, void* argument)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(foreach != NULL);

    bplus_foreach_node_in_node(tree, tree->root, foreach, argument);
}
