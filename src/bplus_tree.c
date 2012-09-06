/**
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "bplus_tree_private.h"

#include "bplus_leaf.h"
#include "bplus_search.h"
#include "bplus_iterator.h"
#include "bplus_foreach.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

int bplus_tree_print(BplusTree const* const tree, char const* format, ...) __attribute__((format(printf, 2, 3)));

BplusTree* bplus_tree_new()
{
    return bplus_tree_new_full(1);
}

BplusTree* bplus_tree_new_full(gboolean allow_duplicate_keys)
{
    BplusTree* tree = malloc(sizeof(BplusTree));

    tree->first = bplus_leaf_new(tree);
    tree->last  = tree->first;
    tree->root  = (BplusNode*) tree->first;

    tree->height               = 1;
    tree->allow_duplicate_keys = allow_duplicate_keys;

    return tree;
}

void bplus_tree_print_stats(BplusTree* tree);

static void bplus_foreach_node_destroy(BplusTree* tree, BplusNode* node, void* argument)
{
    bplus_node_destroy(tree, node);
}

void bplus_tree_destroy(BplusTree* tree)
{
    g_return_if_fail(tree != NULL);

#ifdef BPLUS_TREE_GATHER_STATS
    bplus_tree_print_stats(tree);
#endif /* ifdef BPLUS_TREE_GATHER_STATS */

    bplus_foreach_node_in_tree(tree, &bplus_foreach_node_destroy, NULL);
    free(tree);
}

void bplus_tree_destroy_each(BplusTree* tree, BplusForeachItemFunc* foreach, void* argument)
{
    bplus_foreach_item_in_tree(tree, foreach, argument);
    bplus_tree_destroy(tree);
}

#ifdef BPLUS_TREE_GATHER_STATS

void bplus_tree_print_stats(BplusTree* tree)
{
    printf("tree height: %zu\n", tree->height);
    printf("node count: %zu\n", tree->node_count);
    printf("leaf count: %zu\n", tree->leaf_count);
    printf("underflow count: %zu\n", tree->underflow_count);
    printf("overflow count: %zu\n", tree->overflow_count);
}

#endif /* ifdef BPLUS_TREE_GATHER_STATS */

BplusValue bplus_tree_get(BplusTree* tree, BplusKey key)
{
    BplusPath path;
    bplus_tree_get_path_to_key(tree, key, &path);

    size_t const     index = path.index[0];
    BplusNode const* node  = path.leaf;
    BplusValue       value = NULL;

    if ((index < node->length) && bplus_key_eq(tree, bplus_key_at(node, index), key))
        value = bplus_value_at(node, index);

    return value;
}

BplusValue bplus_tree_get_first(BplusTree const* tree)
{
    if (tree->first->node.length == 0)
        return NULL;

    return tree->first->node.items[0].value;
}

BplusValue bplus_tree_get_nth(BplusTree const* tree, size_t position)
{
    BplusLeaf* leaf = tree->first;
    if (leaf->node.length == 0)
        return NULL;

    while (leaf != NULL && position >= leaf->node.length)
    {
        position -= leaf->node.length;
        leaf      = leaf->right;
    }

    if (leaf != NULL)
        return leaf->node.items[position].value;

    return NULL;
}

void bplus_value_print(BplusNode* node, size_t const index, BplusKey key, BplusValue value, int depth)
{
    static char const* indent = "                                                                   ";

    fprintf(stderr, "%*.s n%p_%zu[label=\"%lu\",fontcolor=\"#000099\"];", 0, indent, node, index, key);
    fprintf(stderr, "%*.s n%p->n%p_%zu;", 0, indent, node, node, index);
}

void bplus_node_print(BplusNode* parent, BplusKey key, BplusNode* node, int depth)
{
    static char const* indent = "                                                                   ";

    fprintf(stderr, "%*.s n%p[label=\"%lu\"];", 0, indent, node, key);
    fprintf(stderr, "%*.s n%p->n%p;", 0, indent, parent, node);

    if (node->is_leaf)
    {
        if (((BplusLeaf*) node)->right != NULL)
            fprintf(stderr, "n%p->n%p[constraint=false];", node, ((BplusLeaf*) node)->right);
        if (((BplusLeaf*) node)->left != NULL)
            fprintf(stderr, "n%p->n%p[constraint=false];", node, ((BplusLeaf*) node)->left);
    }

    for (size_t i = 0; i < node->length; ++i)
    {
        if (node->is_leaf)
            bplus_value_print(node, i, bplus_key_at(node, i), bplus_value_at(node, i), 2);
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
    fprintf(stderr, "edge[arrowsize=0.1,fontsize=6];");

    BplusNode* node = tree->root;
    fprintf(stderr, "n%p[label=\"0\"];", node);
    if (node->is_leaf)
    {
        if (((BplusLeaf*) node)->right != NULL)
            fprintf(stderr, "n%p->n%p[constraint=false];", node, ((BplusLeaf*) node)->right);
        if (((BplusLeaf*) node)->left != NULL)
            fprintf(stderr, "n%p->n%p[constraint=false];", node, ((BplusLeaf*) node)->left);
    }

    for (size_t i = 0; i < node->length; ++i)
    {
        if (node->is_leaf)
            bplus_value_print(node, i, bplus_key_at(node, i), bplus_value_at(node, i), 2);
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

int bplus_iterator_print(BplusTree const* tree, BplusIterator const* iterator)
{
    bplus_tree_print(tree,
                     "label=\"iterator\"; i[fontcolor=\"#009900\",label=\"i\"];"
                     "n%p_%zu->i[label=\"from\"];"
                     "n%p_%zu->i[color=\"#009900\"];"
                     "n%p_%zu->i[label=\"to\"];",
                     iterator->leaf_from, iterator->item_from - iterator->leaf_from->node.items,
                     iterator->leaf, iterator->item - iterator->leaf->node.items,
                     iterator->leaf_to, iterator->item_to - iterator->leaf_to->node.items - 1);

    return 0;
}
