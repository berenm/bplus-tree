#include "bplus_iterator.h"

#include "bplus_tree_private.h"
#include "bplus_search.h"
#include "bplus_leaf.h"

static BplusIterator* bplus_iterator_create(BplusTree const* tree, BplusNode const* leaf, BplusItem const* item, BplusItem const* end)
{
    BplusIterator* iterator = (BplusIterator*) &(tree->iterator);
    g_return_val_if_fail(iterator != NULL, NULL);
    g_return_val_if_fail(iterator != NULL, NULL);
    g_warn_if_fail(iterator->leaf == NULL);

    iterator->leaf     = leaf;
    iterator->leaf_end = leaf->items + leaf->size;
    iterator->item     = item;
    iterator->end      = end;

    return iterator;
}

static void bplus_iterator_destroy(BplusIterator* iterator)
{
    g_return_if_fail(iterator != NULL);

    iterator->leaf     = 0;
    iterator->leaf_end = 0;
    iterator->item     = 0;
    iterator->end      = 0;
}

static BplusIterator* bplus_iterator_next(BplusIterator* iterator)
{
    g_return_val_if_fail(iterator != NULL, NULL);

    iterator->item++;
    if (iterator->leaf_end <= iterator->item)
    {
        BplusNode* const leaf = bplus_tree_leaf_get_not_empty(iterator->leaf->leaf_link->right);
        if (leaf)
        {
            iterator->item     = leaf->items;
            iterator->leaf_end = leaf->items + leaf->size;
            iterator->leaf     = leaf;
        }
        else
        {
            bplus_iterator_end(iterator);
            return NULL;
        }
    }

    if (iterator->item == iterator->end)
    {
        bplus_iterator_end(iterator);
        return NULL;
    }

    return iterator;
}

static void bplus_iterator_end(BplusIterator* iterator)
{
    g_return_if_fail(iterator != NULL);

    if (iterator)
        bplus_iterator_destroy(iterator);
}

static BplusItem const* bplus_iterator_get_item(BplusIterator const* iterator)
{
    g_return_val_if_fail(iterator != NULL, NULL);

    return iterator->item;
}

static BplusIterator* bplus_tree_first(BplusTree const* tree)
{
    g_return_val_if_fail(tree != NULL, NULL);

    BplusNode* leaf = bplus_tree_leaf_get_not_empty(tree->first);
    if (leaf)
        return bplus_iterator_create(tree, leaf, leaf->items, 0);

    return NULL;
}

static BplusIterator* bplus_tree_first_with_key(BplusTree const* tree, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, NULL);

    BplusNode* leaf = bplus_tree_leaf_get_not_empty(bplus_tree_get_leaf(tree, key));
    if (leaf)
    {
        size_t index = bplus_tree_leaf_find_key(leaf, key);

        if (index < leaf->size)
            return bplus_iterator_create(tree, leaf, leaf->items + index, bplus_tree_leaf_get_next_to_last_with_key(tree, key));

    }

    return NULL;
}

static BplusIterator* bplus_tree_range_first(BplusTree const* tree, BplusKey const key_from, BplusKey const key_to)
{
    g_return_val_if_fail(tree != NULL, NULL);

    BplusNode* leaf = bplus_tree_leaf_get_not_empty(bplus_tree_get_leaf(tree, key_from));
    if (leaf)
    {
        size_t index = bplus_tree_leaf_find_key(leaf, key_from);

        if (index < leaf->size)
            return bplus_iterator_create(tree, leaf, leaf->items + index, bplus_tree_leaf_get_next_to_last_with_key(tree, key_to));

    }

    return NULL;
}
