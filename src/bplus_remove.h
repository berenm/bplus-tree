#ifndef __BPLUS_REMOVE_H__
#define __BPLUS_REMOVE_H__

#include "bplus_tree.h"

#include "bplus_node.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

static void bplus_node_remove_range(BplusTree* tree, BplusNode* node, size_t const index_from, size_t const index_to);

static void  bplus_tree_leaf_remove_key_data(BplusTree* tree, BplusNode* node, BplusKey const key, void const* data);
static void* bplus_tree_leaf_remove_key_first(BplusTree* tree, BplusNode* node, BplusKey const key);

static void  bplus_tree_remove(BplusTree* tree, BplusKey const key, void* data);
static void* bplus_tree_remove_first(BplusTree* tree, BplusKey const key);
static void  bplus_tree_remove_all(BplusTree* tree, BplusKey const key);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_REMOVE_H__
