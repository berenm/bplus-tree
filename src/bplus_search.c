#define bplus_tree_search_key_index(operator_m)                                                     \
    static size_t bplus_tree_search_key_index_ ## operator_m(BplusTree const* tree,                 \
                                                             size_t const length,                   \
                                                             BplusKey const keys[BPLUS_TREE_ORDER], \
                                                             BplusKey const key)                    \
    {                                                                                               \
        if (length <= 1 || bplus_key_ ## operator_m(tree, keys[0], key))                            \
            return 0;                                                                               \
        else if (!bplus_key_ ## operator_m(tree, keys[length - 1], key))                            \
            return length - 1;                                                                      \
                                                                                                    \
        size_t index = 0;                                                                           \
        size_t step  = BPLUS_TREE_ORDER;                                                            \
        while (step >>= 1)                                                                          \
        {                                                                                           \
            if (index >= length)                                                                    \
                index -= step;                                                                      \
            else if (bplus_key_ ## operator_m(tree, keys[index], key))                              \
                index -= step;                                                                      \
            else                                                                                    \
                index += step;                                                                      \
        }                                                                                           \
                                                                                                    \
        if (bplus_key_ ## operator_m(tree, keys[index], key))                                       \
            index--;                                                                                \
                                                                                                    \
        return index;                                                                               \
    }

bplus_tree_search_key_index(gt)
bplus_tree_search_key_index(gte)

static size_t bplus_node_get_key_index(BplusTree const* const tree, BplusNode const* const node, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, 0);
    g_return_val_if_fail(node != NULL, 0);

    return bplus_tree_search_key_index_gt(tree, node->length, node->keys, key);
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
