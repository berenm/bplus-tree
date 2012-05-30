#ifndef __BPLUS_SEARCH_H__
#define __BPLUS_SEARCH_H__

#include "bplus_tree.h"

#include "bplus_node.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

static size_t bplus_node_get_key_index_first_or_before(BplusNode const* node, BplusKey const key);
static size_t bplus_node_get_key_index_after_last(BplusNode const* node, BplusKey const key);

static BplusItem const* bplus_tree_at(BplusTree const* tree, size_t const index);
static BplusItem const* bplus_tree_get(BplusTree const* tree, BplusKey const key);

static BplusNode* bplus_tree_get_leaf(BplusTree const* tree, BplusKey const key);

static BplusNode*       bplus_tree_find_leaf_with_key(BplusTree const* tree, BplusKey const key);
static BplusNode*       bplus_tree_find_leaf_with_key_data(BplusTree const* tree, BplusKey const key, void* data);
static BplusItem const* bplus_tree_leaf_get_next_to_last_with_key(BplusTree const* tree, BplusKey const key);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_SEARCH_H__
