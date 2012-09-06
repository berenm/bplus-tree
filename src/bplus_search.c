/**
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "bplus_search.h"

#include "bplus_node.h"
#include "bplus_leaf.h"

#define bplus_node_get_key_index_op(operator_m, tree_m, node_m, key_m)      \
    do                                                                      \
    {                                                                       \
        size_t index = 1;                                                   \
        while (index < (node_m)->length)                                    \
        {                                                                   \
            if (operator_m((tree_m), bplus_key_at(node_m, index), (key_m))) \
                break;                                                      \
            ++index;                                                        \
        }                                                                   \
                                                                            \
        return --index;                                                     \
    }                                                                       \
    while (0)

static size_t bplus_node_get_key_index(BplusTree const* tree, BplusNode const* node, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, 0);
    g_return_val_if_fail(node != NULL, 0);

    bplus_node_get_key_index_op(bplus_key_gt, tree, node, key);
}

static size_t bplus_node_get_key_index_before(BplusTree const* tree, BplusNode const* node, BplusKey const key)
{
    g_return_val_if_fail(tree != NULL, 0);
    g_return_val_if_fail(node != NULL, 0);

    bplus_node_get_key_index_op(bplus_key_gte, tree, node, key);
}

#define bplus_tree_get_path_to_key_op(operator_m, tree_m, key_m, path_m)           \
    do                                                                             \
    {                                                                              \
        if (__builtin_expect(((tree_m)->root->length == 0), 0))                    \
        {                                                                          \
            (path_m)->length   = 1;                                                \
            (path_m)->index[0] = 0;                                                \
            (path_m)->leaf     = tree->root;                                       \
            return;                                                                \
        }                                                                          \
                                                                                   \
        BplusNode const* node   = (tree_m)->root;                                  \
        size_t const     length = (tree_m)->height;                                \
        for (size_t i = length; i > 0; --i)                                        \
        {                                                                          \
            for (int i = 0; i < BPLUS_TREE_ORDER * sizeof(*node->items) / 64; ++i) \
            {                                                                      \
                __builtin_prefetch(node->items + i * 64 / sizeof(*node->items));   \
            }                                                                      \
                                                                                   \
            size_t const index = operator_m((tree_m), node, (key_m));              \
            (path_m)->index[i - 1] = index;                                        \
                                                                                   \
            if (i == 1)                                                            \
                break;                                                             \
                                                                                   \
            node = bplus_node_at(node, index);                                     \
        }                                                                          \
                                                                                   \
        (path_m)->length = length;                                                 \
        (path_m)->leaf   = (BplusNode*) node;                                      \
    }                                                                              \
    while (0)

void bplus_tree_get_path_to_key(BplusTree const* tree, BplusKey const key, BplusPath* path)
{
    g_return_if_fail(tree != NULL);
    g_assert(tree->height < sizeof((path)->index) / sizeof(*(path)->index));

    bplus_tree_get_path_to_key_op(bplus_node_get_key_index, tree, key, path);
}

static void bplus_tree_get_path_to_key_before(BplusTree const* tree, BplusKey const key, BplusPath* path)
{
    g_return_if_fail(tree != NULL);
    g_assert(tree->height < sizeof((path)->index) / sizeof(*(path)->index));

    bplus_tree_get_path_to_key_op(bplus_node_get_key_index_before, tree, key, path);
}

void bplus_tree_get_paths_to_key_range(BplusTree const* tree, BplusKey key_from, BplusKey key_to, BplusPath* path_from, BplusPath* path_to)
{
    g_return_if_fail(tree != NULL);
    g_assert(tree->height < sizeof((path_from)->index) / sizeof(*(path_from)->index));
    g_assert(tree->height < sizeof((path_to)->index) / sizeof(*(path_to)->index));

    if (bplus_key_gt(tree, key_from, key_to))
    {
        BplusKey tmp = key_to;
        key_to   = key_from;
        key_from = tmp;
    }

    bplus_tree_get_path_to_key(tree, key_to, path_to);
    if (!bplus_key_gt(tree, bplus_key_at(path_to->leaf, path_to->index[0]), key_to))
        path_to->index[0]++;

    bplus_tree_get_path_to_key_before(tree, key_from, path_from);
    if (!bplus_key_gte(tree, bplus_key_at(path_from->leaf, path_from->index[0]), key_from))
        path_from->index[0]++;

    BplusLeaf const* const leaf = (BplusLeaf*) path_from->leaf;
    if (path_from->index[0] == leaf->node.length)
    {
        if (leaf->right == NULL)
        {
            path_from->leaf     = path_to->leaf;
            path_from->index[0] = path_to->index[0];
        }
        else
        {
            path_from->leaf     = (BplusNode*) leaf->right;
            path_from->index[0] = 0;
        }
    }
}
