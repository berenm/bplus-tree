/**
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __BPLUS_FOREACH_H__
#define __BPLUS_FOREACH_H__

#include "bplus_tree_private.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

void bplus_foreach_item_in_node(BplusTree* tree, BplusNode* node, BplusForeachItemFunc* foreach, void* argument);
void bplus_foreach_item_in_tree(BplusTree* tree, BplusForeachItemFunc* foreach, void* argument);

typedef void (BplusForeachNodeFunc)(BplusTree* tree, BplusNode* node, void* argument);
void bplus_foreach_node_in_node(BplusTree* tree, BplusNode* node, BplusForeachNodeFunc* foreach, void* argument);
void bplus_foreach_node_in_tree(BplusTree* tree, BplusForeachNodeFunc* foreach, void* argument);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_FOREACH_H__
