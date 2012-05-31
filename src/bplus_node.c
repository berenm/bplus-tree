struct _BplusNode
{
    size_t     length;
    BplusKey   keys[BPLUS_TREE_ORDER];
    BplusValue values[BPLUS_TREE_ORDER];

    gboolean   is_leaf;
    BplusNode* parent;
};

#ifndef BPLUS_TREE_GENERIC
# define bplus_key_gt(bplus_tree_m, k1, k2)  ((k1) > (k2))
# define bplus_key_gte(bplus_tree_m, k1, k2) ((k1) >= (k2))
# define bplus_key_lt(bplus_tree_m, k1, k2)  ((k1) < (k2))
# define bplus_key_lte(bplus_tree_m, k1, k2) ((k1) <= (k2))
# define bplus_key_eq(bplus_tree_m, k1, k2)  ((k1) == (k2))
# define bplus_key_neq(bplus_tree_m, k1, k2) ((k1) != (k2))

#else /* ifndef BPLUS_TREE_GENERIC */
# define bplus_key_gt(bplus_tree_m, k1, k2)  ((bplus_tree_m)->compare_keys((k1), (k2)) > 0)
# define bplus_key_gte(bplus_tree_m, k1, k2) ((bplus_tree_m)->compare_keys((k1), (k2)) >= 0)
# define bplus_key_lt(bplus_tree_m, k1, k2)  ((bplus_tree_m)->compare_keys((k1), (k2)) < 0)
# define bplus_key_lte(bplus_tree_m, k1, k2) ((bplus_tree_m)->compare_keys((k1), (k2)) <= 0)
# define bplus_key_eq(bplus_tree_m, k1, k2)  ((bplus_tree_m)->compare_keys((k1), (k2)) == 0)
# define bplus_key_neq(bplus_tree_m, k1, k2) ((bplus_tree_m)->compare_keys((k1), (k2)) != 0)

#endif /* ifndef BPLUS_TREE_GENERIC */

#define bplus_key_at(node_m, index_m)   (((BplusNode*) node_m)->keys[(index_m)])
#define bplus_key_first(node_m)         bplus_key_at(node_m, 0)
#define bplus_key_last(node_m)          bplus_key_at(node_m, (node_m)->length - 1)
#define bplus_value_at(node_m, index_m) (((BplusNode*) node_m)->values[(index_m)])
#define bplus_value_first(node_m)       bplus_value_at(node_m, 0)
#define bplus_value_last(node_m)        bplus_value_at(node_m, (node_m)->length - 1)
#define bplus_node_at(node_m, index_m)  ((BplusNode*) bplus_value_at(node_m, index_m))
#define bplus_node_first(node_m)        ((BplusNode*) bplus_value_first(node_m))
#define bplus_node_last(node_m)         ((BplusNode*) bplus_value_last(node_m))

#define bplus_node_overfilled(node_m)  ((node_m)->length > (BPLUS_TREE_ORDER - 1))
#define bplus_node_underfilled(node_m) ((node_m)->length < 1)

static BplusNode* bplus_node_new(BplusTree* tree)
{
    g_return_val_if_fail(tree != NULL, NULL);

    BplusNode* node = g_slice_new(BplusNode);

    bplus_node_init(node, FALSE);

#ifdef BPLUS_TREE_GATHER_STATS
    tree->node_count++;
    tree->underflow_count++;
#endif /* ifdef BPLUS_TREE_GATHER_STATS */

    return node;
}

static BplusNode* bplus_node_new_right(BplusTree* tree, BplusNode* node_left)
{
    g_return_val_if_fail(tree != NULL, NULL);
    g_return_val_if_fail(node_left != NULL, NULL);

    BplusNode* node_right;

    if (node_left->is_leaf)
        node_right = (BplusNode*) bplus_leaf_new_right(tree, (BplusLeaf*) node_left);
    else
        node_right = bplus_node_new(tree);

    node_right->parent = node_left->parent;

    return node_right;
}

static void bplus_node_init(BplusNode* node, gboolean is_leaf)
{
    g_return_if_fail(node != NULL);

    node->parent  = NULL;
    node->is_leaf = is_leaf;

    node->length = 0;
}

static void bplus_node_destroy(BplusTree* tree, BplusNode* node)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(node != NULL);

    if (node->is_leaf)
    {
        bplus_leaf_destroy(tree, (BplusLeaf*) node);
    }
    else
    {
        for (size_t i = 0; i < node->length; ++i)
        {
            bplus_node_destroy(tree, bplus_node_at(node, i));
        }

#ifdef BPLUS_TREE_GATHER_STATS
        tree->node_count--;
        tree->underflow_count--;
#endif /* ifdef BPLUS_TREE_GATHER_STATS */
    }

    g_slice_free(BplusNode, node);
}
