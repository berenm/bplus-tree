#ifndef __BPLUS_FOREACH_H__
#define __BPLUS_FOREACH_H__

#include "bplus_tree.h"
#include "bplus_node.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

typedef void (BplusForeach)(BplusNode* node, void* argument);

static void bplus_foreach_node_in_node(BplusTree const* tree, BplusNode* node, BplusForeach* foreach, void* argument);
static void bplus_foreach_node_in_tree(BplusTree const* tree, BplusForeach* foreach, void* argument);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_FOREACH_H__
