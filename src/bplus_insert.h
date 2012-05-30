#ifndef __BPLUS_INSERT_H__
#define __BPLUS_INSERT_H__

#include "bplus_tree.h"

#include "bplus_node.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

static void bplus_node_insert_after(BplusTree* tree, BplusNode* parent, BplusKey const right_key, BplusNode* right_node, BplusNode* left_node);
static void bplus_node_insert(BplusTree* tree, BplusNode* node, BplusKey const key, void* data);
static void bplus_tree_insert(BplusTree* tree, BplusKey const key, void* data);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_INSERT_H__
