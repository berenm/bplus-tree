static void bplus_node_insert_at(BplusTree const* tree, BplusNode* node, size_t const index, size_t const length, BplusKey const* const keys, BplusValue const* const values)
{
    g_return_if_fail(node != NULL);
    g_return_if_fail(index <= node->length);
    g_return_if_fail(node->length + length <= BPLUS_TREE_ORDER);

#ifdef BPLUS_TREE_GATHER_STATS
    int was_underfilled = bplus_node_underfilled(node);
    int was_overfilled  = bplus_node_overfilled(node);
#endif /* ifdef BPLUS_TREE_GATHER_STATS */

    size_t const move_length = node->length - index;
    if (move_length > 0)
    {
        memmove(node->keys + index + length, node->keys + index, move_length * sizeof(BplusKey));
        memmove(node->values + index + length, node->values + index, move_length * sizeof(BplusValue));
    }

    node->length += length;
    memcpy(node->keys + index, keys, length * sizeof(BplusKey));
    memcpy(node->values + index, values, length * sizeof(BplusValue));

#ifdef BPLUS_TREE_GATHER_STATS
    if (!bplus_node_underfilled(node) && was_underfilled)
        tree->underflow_count--;

    if (bplus_node_overfilled(node) && !was_overfilled)
        tree->overflow_count++;
#endif /* ifdef BPLUS_TREE_GATHER_STATS */

    if (node->is_leaf)
        return;

    for (size_t i = index; i < index + length; ++i)
        bplus_node_at(node, i)->parent = node;

}

void bplus_tree_insert(BplusTree* tree, BplusKey const key, BplusValue const value)
{
    BplusPath path;
    bplus_tree_get_path_to_key(tree, key, &path);

    size_t     index = path.index[0];
    BplusNode* node  = (BplusNode*) path.leaf;

    /* If the node is not empty, we will insert it after the given index */
    if ((index < node->length) && bplus_key_lte(tree, bplus_key_at(node, index), key))
        index++;

    /* If we don't allow duplicate keys, simply replace the old value */
    if (!tree->allow_duplicate_keys && (index > 0) && bplus_key_eq(tree, bplus_key_at(node, index - 1), key))
    {
        bplus_value_at(node, index - 1) = value;
    }
    else
    {
        bplus_node_insert_at(tree, node, index, 1, &key, &value);
        if (index == 0)
            bplus_rebalance_propagate(tree, &path);

        if (bplus_node_overfilled(node))
            bplus_rebalance_overfilled(tree, &path);
    }
}
