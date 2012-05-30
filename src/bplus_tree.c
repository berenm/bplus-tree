#include "bplus_tree_private.h"

#include "bplus_node.h"
#include "bplus_iterator.h"
#include "bplus_foreach.h"

static BplusTree* bplus_tree_create()
{
    BplusTree* tree = g_slice_new0(BplusTree);
    // g_debug("create tree %p", tree);

    tree->root      = bplus_node_create(tree, 1, 0);
    tree->root->key = 0;

    tree->first = tree->root;
    tree->last  = tree->root;

    return tree;
}

static void bplus_node_foreach_destroy(BplusNode* node, void* arguments)
{
    bplus_node_destroy((BplusTree*) arguments, node);
}

static void bplus_tree_destroy(BplusTree* tree)
{
    g_return_if_fail(tree != NULL);

    bplus_foreach_node_in_tree(tree, &bplus_node_foreach_destroy, tree);

    // g_debug("destroy tree %p", tree);
    g_slice_free(BplusTree, tree);
}

static void bplus_tree_destroy_each(BplusTree* tree, void (* function)(void const* tree, BplusKey key, void* data, void* argument), void* argument)
{
    BplusIterator* iterator = bplus_tree_first(tree);

    while (iterator)
    {
        function(tree, iterator->item->key, iterator->item->data, argument);
        iterator = bplus_iterator_next(iterator);
    }
    bplus_tree_destroy(tree);
}
