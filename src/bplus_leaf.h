#ifndef __BPLUS_LEAF_H__
#define __BPLUS_LEAF_H__

#include "bplus_tree.h"

#include "bplus_node.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

static size_t     bplus_tree_leaf_find_key(BplusNode const* node, BplusKey const key);
static size_t     bplus_tree_leaf_find_key_data(BplusNode const* node, BplusKey const key, void const* data);
static BplusNode* bplus_tree_leaf_get_not_empty(BplusNode* leaf);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_LEAF_H__
