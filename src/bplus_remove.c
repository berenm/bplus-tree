static void bplus_node_remove_at(BplusTree const* tree, BplusNode* node, size_t const index, size_t const length)
{
    g_return_if_fail(node != NULL);
    g_return_if_fail(index < node->length || bplus_tree_print(tree, "f[fixedsize=false,color=green,label=\"remove_at(%lu, %lu)\"];f->n%p;", index, length, node));
    g_return_if_fail(index + length <= node->length);

    bplus_node_move_and_resize_data(tree, node, index, index + length);
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
