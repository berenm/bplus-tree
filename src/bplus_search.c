typedef struct _BplusPath BplusPath;
struct _BplusPath
{
    size_t     length;
    size_t     index[16];
    BplusNode* leaf;
};

#define bplus_tree_search_key_index(operator_m, tree_m, node_m, key_m) \
    do                                                                 \
    {                                                                  \
        size_t index = 1;                                              \
        while (index < (node_m)->length)                               \
        {                                                              \
            if (operator_m(tree, bplus_key_at(node_m, index), key))    \
                break;                                                 \
            ++index;                                                   \
        }                                                              \
                                                                       \
        return --index;                                                \
    }                                                                  \
    while (0)

static size_t bplus_node_get_key_index(BplusTree const* tree, BplusNode const* node, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, 0);
    g_return_val_if_fail(node != NULL, 0);

    bplus_tree_search_key_index(bplus_key_gt, tree, node, key);
}

static size_t bplus_node_get_key_index_before(BplusTree const* tree, BplusNode const* node, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, 0);
    g_return_val_if_fail(node != NULL, 0);

    bplus_tree_search_key_index(bplus_key_gte, tree, node, key);
}

static void bplus_tree_get_path_to_key(BplusTree const* tree, BplusKey const key, BplusPath* path_out)
{
    g_return_if_fail(tree != NULL);
    g_assert(tree->height < sizeof(path_out->index) / sizeof(*path_out->index));

    if (__builtin_expect((tree->root->length == 0), 0))
    {
        path_out->length   = 1;
        path_out->index[0] = 0;
        path_out->leaf     = tree->root;
        return;
    }

    BplusNode const* node   = tree->root;
    size_t const     length = tree->height;
    for (size_t i = length; i > 0; --i)
    {
        for (int i = 0; i < BPLUS_TREE_ORDER / 8; ++i)
            __builtin_prefetch(node->keys + i * 8);

        size_t const index = bplus_node_get_key_index(tree, node, key);
        path_out->index[i - 1] = index;

        if (i == 1)
            break;

        node = bplus_node_at(node, index);
    }

    path_out->length = length;
    path_out->leaf   = (BplusNode*) node;
}
