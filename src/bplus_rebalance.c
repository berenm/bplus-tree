static void bplus_node_rebalance_propagate(BplusTree const* const tree, BplusNode* node, BplusKey const previous_key)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(node != NULL);

    while (node->parent != NULL)
    {
        BplusKey const key = bplus_key_first(node);
        if (bplus_key_eq(tree, key, previous_key))
            break;

        size_t const index = bplus_node_get_value_index(tree, node->parent, previous_key, (BplusValue*) node);
        bplus_key_at(node->parent, index) = key;
        if (index > 0)
            break;

        node = node->parent;
    }
}

static int64_t bplus_node_get_rebalance_amount(BplusTree const* const tree, BplusNode const* const node_left, BplusNode const* const node_right)
{
    g_return_val_if_fail(tree != NULL, 0);
    g_return_val_if_fail(node_left != NULL, 0);
    g_return_val_if_fail(node_right != NULL, 0);

    size_t const total = node_left->length + node_right->length;

    /* If the nodes can be merged without overfilling */
    if (total <= (BPLUS_TREE_ORDER / 3))
        return -node_right->length;

    /* If the data can be balanced over the two without overfilling */
    else if (total < (BPLUS_TREE_ORDER * 5 / 3))
        return node_left->length - (total + 1) / 2;

    return 0;
}

static gboolean bplus_node_find_merge_candidate(BplusTree* tree, BplusNode* node, BplusNode** node_left, BplusNode** node_right)
{
    g_return_val_if_fail(tree != NULL, FALSE);
    g_return_val_if_fail(node != NULL, FALSE);

    *node_left  = node;
    *node_right = node;
    if ((node->parent == NULL) || (node->parent->length <= 1))
        return FALSE;

    size_t const index = bplus_node_get_index_in_parent(tree, node);
    if (index == 0)
    {
        *node_right = bplus_node_at(node->parent, index + 1);
        if (bplus_node_get_rebalance_amount(tree, node, *node_right) == 0)
            *node_right = node;

    }
    else if (index == node->parent->length - 1)
    {
        *node_left = bplus_node_at(node->parent, index - 1);
        if (bplus_node_get_rebalance_amount(tree, *node_left, node) == 0)
            *node_left = node;

    }
    else
    {
        *node_left  = bplus_node_at(node->parent, index - 1);
        *node_right = bplus_node_at(node->parent, index + 1);

        int64_t const merge_amount_right = bplus_node_get_rebalance_amount(tree, node, *node_right);
        int64_t const merge_amount_left  = bplus_node_get_rebalance_amount(tree, *node_left, node);

        if (merge_amount_left < 0)
        {
            /* Merge with left copies data to the left.
             * Prefer a merge with right only if it copies less data from the right.
             */
            if (merge_amount_right >= 0)
                *node_right = node;
            else if (merge_amount_right < merge_amount_left)
                *node_right = node;
            else
                *node_left = node;
        }
        else if (merge_amount_left > 0)
        {
            /* Merge with left copies data from the left.
             * Prefer a merge with right if it copies less data to the right or data from right.
             */
            if (merge_amount_right == 0)
                *node_right = node;
            else if (merge_amount_right > merge_amount_left)
                *node_right = node;
            else
                *node_left = node;
        }
        else if (merge_amount_right != 0)
        {
            /* Merge with left is impossible.
             * Prefer a merge with right.
             */
            *node_left = node;
        }
        else
        {
            *node_left  = node;
            *node_right = node;
        }
    }

    return *node_left != *node_right;
} /* bplus_node_find_merge_candidate */

static size_t bplus_node_rebalance_data(BplusTree* tree, BplusNode* node_left, BplusNode* node_right)
{
    g_return_val_if_fail(tree != NULL, 0);
    g_return_val_if_fail(node_left != NULL, 0);
    g_return_val_if_fail(node_right != NULL, 0);

    int64_t const amount = bplus_node_get_rebalance_amount(tree, node_left, node_right);
    g_return_val_if_fail(amount <= 0 || amount < node_left->length, 0);
    g_return_val_if_fail(amount >= 0 || -amount <= node_right->length, 0);

    if (amount > 0)
    {
        size_t const      index  = node_left->length - amount;
        BplusKey const*   keys   = node_left->keys + index;
        BplusValue const* values = node_left->values + index;

        bplus_node_insert_at(tree, node_right, 0, amount, keys, values);
        bplus_node_remove_at(tree, node_left, index, amount);

    }
    else if (amount < 0)
    {
        size_t const      index  = node_left->length;
        BplusKey const*   keys   = node_right->keys;
        BplusValue const* values = node_right->values;

        bplus_node_insert_at(tree, node_left, index, -amount, keys, values);
        bplus_node_remove_at(tree, node_right, 0, -amount);
    }

    return 0;
} /* bplus_node_rebalance_data */

static void bplus_node_rebalance_shrink(BplusTree* tree)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(tree->root != NULL);

    while ((tree->root->length == 1) && (!tree->root->is_leaf))
    {
        BplusNode* node = tree->root;
        tree->root         = bplus_node_first(tree->root);
        tree->root->parent = NULL;
        node->length       = 0;
        bplus_node_destroy(tree, node);

#ifdef BPLUS_TREE_GATHER_STATS
        tree->height--;
#endif /* ifdef BPLUS_TREE_GATHER_STATS */
    }
}

static void bplus_node_rebalance(BplusTree* tree, BplusNode* node)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(node != NULL);

    BplusNode* node_left;
    BplusNode* node_right;
    while (bplus_node_overfilled(node) || bplus_node_underfilled(node))
    {
        BplusNode* parent = node->parent;
        if (parent == NULL)
        {
            if (bplus_node_overfilled(node))
            {
                tree->root                    = bplus_node_new(tree);
                tree->root->length            = 1;
                bplus_key_first(tree->root)   = bplus_key_first(node);
                bplus_value_first(tree->root) = node;
                node->parent                  = tree->root;

                parent = node->parent;

#ifdef BPLUS_TREE_GATHER_STATS
                tree->height++;
#endif /* ifdef BPLUS_TREE_GATHER_STATS */
            }
            else
            {
                break;
            }
        }

        if (bplus_node_overfilled(node))
        {
            node_right = bplus_node_new_right(tree, node);
            bplus_node_rebalance_data(tree, node, node_right);

            size_t const index = bplus_node_get_index_in_parent(tree, node);
            BplusKey     key   = bplus_key_first(node_right);
            BplusValue   value = node_right;
            bplus_node_insert_at(tree, parent, index + 1, 1, &key, &value);
        }
        else if (bplus_node_find_merge_candidate(tree, node, &node_left, &node_right))
        {
            BplusKey const key_left  = bplus_key_first(node_left);
            BplusKey const key_right = bplus_key_first(node_right);

            bplus_node_rebalance_data(tree, node_left, node_right);
            bplus_node_rebalance_propagate(tree, node_left, key_left);

            if (node_right->length == 0)
            {
                size_t const index = bplus_node_get_index_in_parent(tree, node_right);
                bplus_node_remove_at(tree, parent, index, 1);
                bplus_node_destroy(tree, node_right);
            }
            else
            {
                bplus_node_rebalance_propagate(tree, node_right, key_right);
            }
        }
        else if (node->length == 0)
        {
            size_t const index = bplus_node_get_index_in_parent(tree, node);
            bplus_node_remove_at(tree, parent, index, 1);
            bplus_node_destroy(tree, node);

        }

        node = parent;
    }

    bplus_node_rebalance_shrink(tree);
} /* bplus_node_rebalance */
