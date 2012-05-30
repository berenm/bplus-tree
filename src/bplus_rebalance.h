#ifndef __BPLUS_REBALANCE_H__
#define __BPLUS_REBALANCE_H__

#include "bplus_tree.h"

#include "bplus_node.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

static void bplus_node_split(BplusTree* tree, BplusNode* left_node);
static void bplus_node_collapse_root(BplusTree* tree);

static void bplus_node_update_parent(BplusNode* node);
static void bplus_node_propagate_minimal_key(BplusNode* node);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_REBALANCE_H__
