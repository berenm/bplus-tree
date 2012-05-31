struct _BplusIterator
{
    BplusItem const* item;
    BplusLeaf const* leaf;

    BplusLeaf const* leaf_from;
    BplusItem const* item_from;
    BplusLeaf const* leaf_to;
    BplusItem const* item_to;
};

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

static BplusIterator* bplus_iterator_new(BplusTree const* tree)
{
    return bplus_iterator_new_to_last(tree, tree->first, tree->first->node.items);
}

static void bplus_iterator_destroy(BplusIterator* iterator)
{
    g_return_if_fail(iterator != NULL);

    g_slice_free(BplusIterator, iterator);
}

static gboolean bplus_iterator_next(BplusIterator* iterator)
{
    g_return_val_if_fail(iterator != NULL, FALSE);

    BplusItem const* const item = iterator->item + 1;
    BplusLeaf const* const leaf = iterator->leaf;
    if (item == iterator->item_to)
        return FALSE;

    if (item - leaf->node.items >= leaf->node.length)
    {
        if (leaf->right == NULL)
        {
            return FALSE;
        }
        else
        {
            iterator->leaf = leaf->right;
            iterator->item = leaf->right->node.items;
        }
    }
    else
    {
        ++iterator->item;
    }

    return TRUE;
}

static gboolean bplus_iterator_previous(BplusIterator* iterator)
{
    g_return_val_if_fail(iterator != NULL, FALSE);

    BplusItem const* const item = iterator->item;
    BplusLeaf const* const leaf = iterator->leaf;
    if (item == iterator->item_from)
        return FALSE;

    if (item - leaf->node.items == 0)
    {
        if (leaf->left == NULL)
        {
            return FALSE;
        }
        else
        {
            iterator->leaf = leaf->left;
            iterator->item = leaf->left->node.items + leaf->left->node.length;
        }
    }

    --iterator->item;
    return TRUE;
}

static BplusItem const* bplus_iterator_get_item(BplusIterator const* iterator)
{
    g_return_val_if_fail(iterator != NULL, NULL);

    if (iterator->item_from == iterator->item_to)
        return NULL;

    return iterator->item;
}

static BplusIterator* bplus_tree_first(BplusTree const* tree)
{
    g_return_val_if_fail(tree != NULL, NULL);

    return bplus_iterator_new(tree);
}

static BplusIterator* bplus_iterator_from_key(BplusTree const* tree, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, NULL);

    BplusPath path_from;
    BplusPath path_to;
    bplus_tree_get_paths_to_key_range(tree, key, key, &path_from, &path_to);

    return bplus_iterator_new_to_last(tree, (BplusLeaf*) path_from.leaf, path_from.leaf->items + path_from.index[0]);
}

static BplusIterator* bplus_iterator_to_key(BplusTree const* tree, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, NULL);

    BplusPath path_from;
    BplusPath path_to;
    bplus_tree_get_paths_to_key_range(tree, key, key, &path_from, &path_to);

    return bplus_iterator_new_from_first(tree, (BplusLeaf*) path_to.leaf, path_to.leaf->items + path_to.index[0]);
}

static BplusIterator* bplus_iterator_for_key(BplusTree const* tree, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, NULL);

    BplusPath path_from;
    BplusPath path_to;
    bplus_tree_get_paths_to_key_range(tree, key, key, &path_from, &path_to);

    return bplus_iterator_new_full(tree,
                                   (BplusLeaf*) path_from.leaf, path_from.leaf->items + path_from.index[0],
                                   (BplusLeaf*) path_to.leaf, path_to.leaf->items + path_to.index[0]);
}

static BplusIterator* bplus_iterator_for_key_range(BplusTree const* tree, BplusKey const key_from, BplusKey const key_to)
{
    g_return_val_if_fail(tree != NULL, NULL);

    BplusPath path_from;
    BplusPath path_to;
    bplus_tree_get_paths_to_key_range(tree, key_from, key_to, &path_from, &path_to);

    return bplus_iterator_new_full(tree,
                                   (BplusLeaf*) path_from.leaf, path_from.leaf->items + path_from.index[0],
                                   (BplusLeaf*) path_to.leaf, path_to.leaf->items + path_to.index[0]);
}
