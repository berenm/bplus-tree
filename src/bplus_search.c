typedef struct _BplusPath BplusPath;
struct _BplusPath
{
    size_t     length;
    size_t     index[16];
    BplusNode* leaf;
};

#define bplus_tree_search_key_index(operator_m, tree_m, node_m, key_m)                       \
    do                                                                                       \
    {                                                                                        \
        if ((node_m)->length <= 1 || operator_m((tree_m), bplus_key_first(node_m), (key_m))) \
        {                                                                                    \
            return 0;                                                                        \
        }                                                                                    \
        else if (!operator_m((tree_m), bplus_key_last(node_m), (key_m)))                     \
        {                                                                                    \
            return (node_m)->length - 1;                                                     \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            size_t index = 0;                                                                \
            size_t step  = BPLUS_TREE_ORDER;                                                 \
            while (step >>= 1)                                                               \
            {                                                                                \
                if (index >= (node_m)->length)                                               \
                    index -= step;                                                           \
                else if (operator_m((tree_m), bplus_key_at(node_m, index), (key_m)))         \
                    index -= step;                                                           \
                else                                                                         \
                    index += step;                                                           \
            }                                                                                \
                                                                                             \
            if (operator_m((tree_m), bplus_key_at(node_m, index), (key_m)))                  \
                index--;                                                                     \
                                                                                             \
            return index;                                                                    \
        }                                                                                    \
    }                                                                                        \
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

    BplusNode const* node   = tree->root;
    size_t const     length = tree->height;

    for (size_t i = length; i > 0; --i)
    {
        size_t const index = bplus_node_get_key_index(tree, node, key);
        path_out->index[i - 1] = index;
        if (i == 1)
            break;

        node = bplus_node_at(node, index);
    }

    path_out->length = length;
    path_out->leaf   = (BplusNode*) node;
}
