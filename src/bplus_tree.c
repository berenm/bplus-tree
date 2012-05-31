#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "bplus_tree.h"

BplusTree* bplus_tree_ref(BplusTree* tree);
void       bplus_tree_unref(BplusTree* tree);

static BplusNode* bplus_node_new(BplusTree* tree);
static BplusNode* bplus_node_new_right(BplusTree* tree, BplusNode* bplus_node_left);
static void       bplus_node_init(BplusNode* node, gboolean is_leaf);
static void       bplus_node_destroy(BplusTree* tree, BplusNode* node);

static void bplus_node_insert_at(BplusTree const* tree, BplusNode* node, size_t const index, size_t const length, BplusKey const* keys, BplusValue const* values);
static void bplus_node_remove_at(BplusTree const* tree, BplusNode* node, size_t const index, size_t const length);

static BplusLeaf* bplus_leaf_new(BplusTree* tree);
static BplusLeaf* bplus_leaf_new_right(BplusTree* tree, BplusLeaf* bplus_leaf_left);
static void       bplus_leaf_destroy(BplusTree* tree, BplusLeaf* bplus_leaf);

int __attribute__((format(printf, 2, 3))) bplus_tree_print(BplusTree const* const tree, char const* format, ...);

struct _BplusTree
{
    BplusNode* root;

    BplusLeaf* first;
    BplusLeaf* last;

    size_t   height;
    gint     ref_count;
    gboolean allow_duplicate_keys;

#ifdef BPLUS_TREE_GATHER_STATS
    size_t node_count;
    size_t leaf_count;
    size_t underflow_count;
    size_t overflow_count;
#endif /* ifdef BPLUS_TREE_GATHER_STATS */

#ifdef BPLUS_TREE_GENERIC
    GCompareFunc compare_keys;
#endif /* ifdef BPLUS_TREE_GENERIC */
};

BplusTree* bplus_tree_ref(BplusTree* tree)
{
    g_return_val_if_fail(tree != NULL, NULL);

    g_atomic_int_inc(&tree->ref_count);

    return tree;
}

void bplus_tree_unref(BplusTree* tree)
{
    g_return_if_fail(tree != NULL);

    if (g_atomic_int_dec_and_test(&tree->ref_count))
    {
        bplus_node_destroy(tree, tree->root);

        g_slice_free(BplusTree, tree);
    }
}

BplusTree* bplus_tree_new(gboolean allow_duplicate_keys
#ifdef BPLUS_TREE_GENERIC
                          , GCompareFunc key_compare_func
#endif /* ifndef BPLUS_TREE_GENERIC */
                          )
{

    BplusTree* tree = g_slice_new(BplusTree);

    tree->first = bplus_leaf_new(tree);
    tree->last  = tree->first;
    tree->root  = (BplusNode*) tree->first;

    tree->height               = 1;
    tree->ref_count            = 1;
    tree->allow_duplicate_keys = allow_duplicate_keys;

#ifdef BPLUS_TREE_GENERIC
    tree->compare_keys = key_compare_func;
#endif /* ifndef BPLUS_TREE_GENERIC */

    return tree;
}

void bplus_tree_print_stats(BplusTree* tree);

void bplus_tree_destroy(BplusTree* tree)
{
    g_return_if_fail(tree != NULL);

#ifdef BPLUS_TREE_GATHER_STATS
    bplus_tree_print_stats(tree);
#endif /* ifdef BPLUS_TREE_GATHER_STATS */

    // bplus_tree_remove_all(tree);
    bplus_tree_unref(tree);
}

#include "bplus_node.c"
#include "bplus_leaf.c"
#include "bplus_search.c"
#include "bplus_rebalance.c"
#include "bplus_insert.c"
#include "bplus_remove.c"

#ifdef BPLUS_TREE_GATHER_STATS

void bplus_tree_print_stats(BplusTree* tree)
{
    g_print("tree height: %zu\n", tree->height);
    g_print("node count: %zu\n", tree->node_count);
    g_print("leaf count: %zu\n", tree->leaf_count);
    g_print("underflow count: %zu\n", tree->underflow_count);
    g_print("overflow count: %zu\n", tree->overflow_count);
}

#endif /* ifdef BPLUS_TREE_GATHER_STATS */

BplusValue bplus_tree_get(BplusTree* tree, BplusKey key)
{
    BplusPath path;
    bplus_tree_get_path_to_key(tree, key, &path);

    size_t const     index = path.index[0];
    BplusNode const* node  = path.leaf;
    BplusValue       value = NULL;

    if (bplus_key_eq(tree, bplus_key_at(node, index), key))
        value = bplus_value_at(node, index);

    return value;
}

void bplus_value_print(BplusNode* node, BplusKey key, BplusValue value, int depth)
{
    static char const* indent = "                                                                   ";

    fprintf(stderr, "%*.s n%lu[label=\"%lu\",fontcolor=\"#000099\"];", 0, indent, key, key);
    fprintf(stderr, "%*.s n%p->n%lu;", 0, indent, node, key);
}

void bplus_node_print(BplusNode* parent, BplusKey key, BplusNode* node, int depth)
{
    static char const* indent = "                                                                   ";

    fprintf(stderr, "%*.s n%p[label=\"%lu\"];", 0, indent, node, key);
    fprintf(stderr, "%*.s n%p->n%p;", 0, indent, parent, node);
    for (size_t i = 0; i < node->length; ++i)
    {
        if (node->is_leaf)
            bplus_value_print(node, bplus_key_at(node, i), bplus_value_at(node, i), depth + 2);

        else
            bplus_node_print(node, bplus_key_at(node, i), bplus_node_at(node, i), depth + 2);

    }
}

int bplus_tree_print(BplusTree const* const tree, char const* format, ...)
{
    static int count = 0;

    fprintf(stderr, "echo 'digraph {");
    fprintf(stderr, "graph[ordering=\"out\"];\n");
    fprintf(stderr, "node[width=0.2,height=0.2,fixedsize=true,fontsize=6,fontcolor=\"#990000\",shape=plaintext];");
    fprintf(stderr, "edge[arrowsize=0.1];");

    BplusNode* node = tree->root;
    fprintf(stderr, "n%p[label=\"0\"];", node);
    for (size_t i = 0; i < node->length; ++i)
    {
        if (node->is_leaf)
            bplus_value_print(node, bplus_key_at(node, i), bplus_value_at(node, i), 2);

        else
            bplus_node_print(node, bplus_key_at(node, i), bplus_node_at(node, i), 2);

    }

    va_list vargs;
    va_start(vargs, format);
    vfprintf(stderr, format, vargs);
    va_end(vargs);

    fprintf(stderr, "}'| dot -T png -o tree-%d.png\n", count);
    count++;
    return 0;
}
