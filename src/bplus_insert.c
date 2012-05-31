static void bplus_leaf_insert_at(BplusTree const* tree, BplusNode* node, size_t const index, BplusKey const key, BplusValue const value)
{
    g_return_if_fail(node != NULL);
    g_return_if_fail(index <= node->length);
    g_return_if_fail(node->length + length <= BPLUS_TREE_ORDER);

    bplus_node_move_and_resize_data(tree, node, index + 1, index);
    bplus_key_at(node, index)   = key;
    bplus_value_at(node, index) = value;
}

static void bplus_node_insert_at(BplusTree const* tree, BplusNode* node, size_t const index, size_t const length, BplusKey const* const keys, BplusValue const* const values)
{
    g_return_if_fail(node != NULL);
    g_return_if_fail(index <= node->length);
    g_return_if_fail(node->length + length <= BPLUS_TREE_ORDER);

    bplus_node_move_and_resize_data(tree, node, index + length, index);
    memcpy(node->keys + index, keys, length * sizeof(BplusKey));
    memcpy(node->values + index, values, length * sizeof(BplusValue));

    for (size_t i = index; i < index + length; ++i)
    {
        bplus_node_at(node, i)->parent = node;
    }
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

    bplus_leaf_insert_at(tree, node, index, key, value);
    if (index == 0)
        bplus_rebalance_propagate(tree, &path);

    if (bplus_node_overfilled(node))
        bplus_rebalance_overfilled(tree, &path);
}
