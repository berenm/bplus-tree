/**
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "bplus_iterator.h"

#include "bplus_leaf.h"
#include "bplus_search.h"

static BplusIterator* bplus_iterator_new_full(BplusTree const* tree,
                                              BplusLeaf const* leaf_from, BplusItem const* item_from,
                                              BplusLeaf const* leaf_to, BplusItem const* item_to)
{
    BplusIterator* iterator = g_slice_new(BplusIterator);

    iterator->leaf_from = leaf_from;
    iterator->item_from = item_from;
    iterator->leaf_to   = leaf_to;
    iterator->item_to   = item_to;
    iterator->item      = item_from;
    iterator->leaf      = leaf_from;

    return iterator;
}

static BplusIterator* bplus_iterator_new_to_last(BplusTree const* tree, BplusLeaf const* leaf_from, BplusItem const* item_from)
{
    return bplus_iterator_new_full(tree, leaf_from, item_from, tree->last, tree->last->node.items + tree->last->node.length);
}

static BplusIterator* bplus_iterator_new_from_first(BplusTree const* tree, BplusLeaf const* leaf_to, BplusItem const* item_to)
{
    return bplus_iterator_new_full(tree, tree->first, tree->first->node.items, leaf_to, item_to);
}

BplusIterator* bplus_iterator_new(BplusTree const* tree)
{
    return bplus_iterator_new_to_last(tree, tree->first, tree->first->node.items);
}

void bplus_iterator_destroy(BplusIterator* iterator)
{
    g_return_if_fail(iterator != NULL);

    g_slice_free(BplusIterator, iterator);
}

gboolean bplus_iterator_next(BplusIterator* iterator)
{
    g_return_val_if_fail(iterator != NULL, FALSE);

    BplusItem const* const next = iterator->item + 1;
    BplusLeaf const* const leaf = iterator->leaf;
    if (next == iterator->item_to)
        return FALSE;

    if (next - leaf->node.items < leaf->node.length)
    {
        ++iterator->item;
    }
    else
    {
        if (leaf->right == NULL)
            return FALSE;

        iterator->leaf = leaf->right;
        iterator->item = leaf->right->node.items;
    }

    return TRUE;
}

gboolean bplus_iterator_previous(BplusIterator* iterator)
{
    g_return_val_if_fail(iterator != NULL, FALSE);

    BplusItem const* const item = iterator->item;
    BplusLeaf const* const leaf = iterator->leaf;

    if (item == iterator->item_from)
        return FALSE;

    if (item - leaf->node.items == 0)
    {
        if (leaf->left == NULL)
            return FALSE;

        iterator->leaf = leaf->left;
        iterator->item = leaf->left->node.items + leaf->left->node.length;
    }

    --iterator->item;

    return TRUE;
}

BplusItem const* bplus_iterator_get_item(BplusIterator const* iterator)
{
    g_return_val_if_fail(iterator != NULL, NULL);

    if (iterator->item_from == iterator->item_to)
        return NULL;

    return iterator->item;
}

BplusIterator* bplus_tree_first(BplusTree const* tree)
{
    g_return_val_if_fail(tree != NULL, NULL);

    return bplus_iterator_new(tree);
}

BplusIterator* bplus_iterator_from_key(BplusTree const* tree, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, NULL);

    BplusPath path_from;
    BplusPath path_to;
    bplus_tree_get_paths_to_key_range(tree, key, key, &path_from, &path_to);

    return bplus_iterator_new_to_last(tree, (BplusLeaf*) path_from.leaf, path_from.leaf->items + path_from.index[0]);
}

BplusIterator* bplus_iterator_to_key(BplusTree const* tree, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, NULL);

    BplusPath path_from;
    BplusPath path_to;
    bplus_tree_get_paths_to_key_range(tree, key, key, &path_from, &path_to);

    return bplus_iterator_new_from_first(tree, (BplusLeaf*) path_to.leaf, path_to.leaf->items + path_to.index[0]);
}

BplusIterator* bplus_iterator_for_key(BplusTree const* tree, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, NULL);

    BplusPath path_from;
    BplusPath path_to;
    bplus_tree_get_paths_to_key_range(tree, key, key, &path_from, &path_to);

    return bplus_iterator_new_full(tree,
                                   (BplusLeaf*) path_from.leaf, path_from.leaf->items + path_from.index[0],
                                   (BplusLeaf*) path_to.leaf, path_to.leaf->items + path_to.index[0]);
}

BplusIterator* bplus_iterator_for_key_range(BplusTree const* tree, BplusKey const key_from, BplusKey const key_to)
{
    g_return_val_if_fail(tree != NULL, NULL);

    BplusPath path_from;
    BplusPath path_to;
    bplus_tree_get_paths_to_key_range(tree, key_from, key_to, &path_from, &path_to);

    return bplus_iterator_new_full(tree,
                                   (BplusLeaf*) path_from.leaf, path_from.leaf->items + path_from.index[0],
                                   (BplusLeaf*) path_to.leaf, path_to.leaf->items + path_to.index[0]);
}
