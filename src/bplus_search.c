#define bplus_tree_search_key_index(operator_m, tree_m, node_m, key_m, index_m)              \
    do                                                                                       \
    {                                                                                        \
        if ((node_m)->length <= 1 || operator_m((tree_m), bplus_key_first(node_m), (key_m))) \
        {                                                                                    \
            (index_m) = 0;                                                                   \
        }                                                                                    \
        else if (!operator_m((tree_m), bplus_key_last(node_m), (key_m)))                     \
        {                                                                                    \
            (index_m) = (node_m)->length - 1;                                                \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            size_t step = BPLUS_TREE_ORDER;                                                  \
            while (step >>= 1)                                                               \
            {                                                                                \
                if ((index_m) >= (node_m)->length)                                           \
                    (index_m) -= step;                                                       \
                else if (operator_m((tree_m), bplus_key_at(node_m, (index_m)), (key_m)))     \
                    (index_m) -= step;                                                       \
                else                                                                         \
                    (index_m) += step;                                                       \
            }                                                                                \
                                                                                             \
            if (operator_m((tree_m), bplus_key_at(node_m, (index_m)), (key_m)))              \
                (index_m)--;                                                                 \
        }                                                                                    \
    }                                                                                        \
    while (0)

static size_t bplus_node_get_key_index(BplusTree const* tree, BplusNode const* node, BplusKey key)
{
    g_return_val_if_fail(tree != NULL, 0);
    g_return_val_if_fail(node != NULL, 0);
    size_t index = 0;

    bplus_tree_search_key_index(bplus_key_gt, tree, node, key, index);

    return index;
}

static size_t bplus_node_get_key_index_before(BplusTree const* const tree, BplusNode const* const node, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, 0);
    g_return_val_if_fail(node != NULL, 0);
    size_t index = 0;

    bplus_tree_search_key_index(bplus_key_gte, tree, node, key, index);

    return index;
}

static size_t bplus_node_get_value_index(BplusTree const* const tree, BplusNode const* const node, BplusKey const key, BplusValue const value)
{
    g_return_val_if_fail(tree != NULL, 0);
    g_return_val_if_fail(node != NULL, 0);

    size_t index = bplus_node_get_key_index(tree, node, key);
    while (index > 0)
    {
        if (bplus_key_eq(tree, bplus_key_at(node, index), key) && (bplus_value_at(node, index) == value))
            break;

        index--;
    }

    return index;
}

static size_t bplus_node_get_index_in_parent(BplusTree const* const tree, BplusNode const* const node)
{
    g_return_val_if_fail(tree != NULL, 0);
    g_return_val_if_fail(node != NULL, 0);
    g_return_val_if_fail(node->parent != NULL, 0);
    g_return_val_if_fail(node->parent != NULL || tree->root == node, 0);

    return bplus_node_get_value_index(tree, node->parent, bplus_key_first(node), (BplusValue*) node);
}

static BplusNode* bplus_node_get_node_for_key(BplusTree const* const tree, BplusNode const* const node, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, NULL);
    g_return_val_if_fail(node != NULL, NULL);
    g_return_val_if_fail(node->parent != NULL || tree->root == node, NULL);

    return bplus_node_at(node, bplus_node_get_key_index(tree, node, key));
}

static BplusLeaf* bplus_tree_get_leaf_for_key(BplusTree const* const tree, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, NULL);

    BplusNode* node = tree->root;
    while (!node->is_leaf)
        node = bplus_node_get_node_for_key(tree, node, key);

    return (BplusLeaf*) node;
}

static void bplus_leaf_get_key_location(BplusTree const* const tree, BplusKey const key, BplusLeaf** leaf_out, size_t* index_out)
{
    *leaf_out  = bplus_tree_get_leaf_for_key(tree, key);
    *index_out = bplus_node_get_key_index(tree, (BplusNode*) *leaf_out, key);
}

static void bplus_leaf_get_key_value_location(BplusTree const* const tree, BplusKey const key, BplusValue const value, BplusLeaf** leaf_out, size_t* index_out)
{
    BplusLeaf* leaf;
    size_t     index;

    bplus_leaf_get_key_location(tree, key, &leaf, &index);

    do
    {
        index = bplus_node_get_value_index(tree, (BplusNode*) leaf, key, value);

        if (bplus_key_neq(tree, bplus_key_at(leaf, index), key))
            break;

        if (bplus_value_at(leaf, index) != value)
            break;

        if (index == 0)
        {
            leaf = leaf->left;
            if (leaf != NULL)
                index = leaf->node.length;
        }
        index--;

    }
    while (leaf != NULL);

    *leaf_out  = leaf;
    *index_out = index;
}
