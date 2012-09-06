/**
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __BPLUS_NODE_H__
#define __BPLUS_NODE_H__

#include "bplus_tree_private.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

struct _BplusNode
{
    size_t    length;
    BplusItem items[BPLUS_TREE_ORDER];

    uint8_t    is_leaf;
    BplusNode* parent;
};

#define bplus_key_gt(bplus_tree_m, k1, k2)  ((k1) > (k2))
#define bplus_key_gte(bplus_tree_m, k1, k2) ((k1) >= (k2))
#define bplus_key_lt(bplus_tree_m, k1, k2)  ((k1) < (k2))
#define bplus_key_lte(bplus_tree_m, k1, k2) ((k1) <= (k2))
#define bplus_key_eq(bplus_tree_m, k1, k2)  ((k1) == (k2))
#define bplus_key_neq(bplus_tree_m, k1, k2) ((k1) != (k2))

#define bplus_key_at(node_m, index_m)   (((BplusNode*) node_m)->items[(index_m)].key)
#define bplus_key_first(node_m)         bplus_key_at(node_m, 0)
#define bplus_key_last(node_m)          bplus_key_at(node_m, (node_m)->length - 1)
#define bplus_value_at(node_m, index_m) (((BplusNode*) node_m)->items[(index_m)].value)
#define bplus_value_first(node_m)       bplus_value_at(node_m, 0)
#define bplus_value_last(node_m)        bplus_value_at(node_m, (node_m)->length - 1)
#define bplus_node_at(node_m, index_m)  ((BplusNode*) bplus_value_at(node_m, index_m))
#define bplus_node_first(node_m)        ((BplusNode*) bplus_value_first(node_m))
#define bplus_node_last(node_m)         ((BplusNode*) bplus_value_last(node_m))

#define bplus_node_overfilled(node_m)  ((node_m)->length > (BPLUS_TREE_ORDER - 1))
#define bplus_node_underfilled(node_m) ((node_m)->length <= 1)

BplusNode* bplus_node_new(BplusTree* tree);
BplusNode* bplus_node_new_right(BplusTree* tree, BplusNode* node_left);
void       bplus_node_destroy(BplusTree* tree, BplusNode* node);
void       bplus_node_init(BplusNode* node, gboolean is_leaf);
void       bplus_node_move_and_resize_data(BplusTree const* tree, BplusNode* node, size_t const index_to, size_t const index_from);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_NODE_H__
