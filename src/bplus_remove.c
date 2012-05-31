static void bplus_node_remove_at(BplusTree const* tree, BplusNode* node, size_t const index, size_t const length)
{
    g_return_if_fail(node != NULL);
    g_return_if_fail(index < node->length || bplus_tree_print(tree, "f[fixedsize=false,color=green,label=\"remove_at(%lu, %lu)\"];f->n%p;", index, length, node));
    g_return_if_fail(index + length <= node->length);

#ifdef BPLUS_TREE_GATHER_STATS
    int was_underfilled = bplus_node_underfilled(node);
    int was_overfilled  = bplus_node_overfilled(node);
#endif /* ifdef BPLUS_TREE_GATHER_STATS */

    node->length -= length;
    size_t move_length = node->length - index;
    if (move_length > 0)
    {
        memmove(node->keys + index, node->keys + index + length, move_length * sizeof(BplusKey));
        memmove(node->values + index, node->values + index + length, move_length * sizeof(BplusValue));
    }

#ifdef BPLUS_TREE_GATHER_STATS
    if (bplus_node_underfilled(node) && !was_underfilled)
        tree->underflow_count++;

    if (!bplus_node_overfilled(node) && was_overfilled)
        tree->overflow_count--;
#endif /* ifdef BPLUS_TREE_GATHER_STATS */
}

void bplus_tree_remove(BplusTree* tree, BplusKey const key)
{
    BplusPath path;
    bplus_tree_get_path_to_key(tree, key, &path);

    size_t const index = path.index[0];
    BplusNode*   node  = (BplusNode*) path.leaf;

    if (bplus_key_eq(tree, bplus_key_at(node, index), key))
    {
        bplus_node_remove_at(tree, node, index, 1);
        if (index == 0)
            bplus_rebalance_propagate(tree, &path);

        if (bplus_node_underfilled(node))
            bplus_rebalance_underfilled(tree, &path);
    }
}
