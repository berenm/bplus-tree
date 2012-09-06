/**
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __BPLUS_LEAF_H__
#define __BPLUS_LEAF_H__

#include "bplus_tree_private.h"

#include "bplus_node.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

struct _BplusLeaf
{
    BplusNode node;

    BplusLeaf* left;
    BplusLeaf* right;
};

BplusLeaf* bplus_leaf_new(BplusTree* tree);
BplusLeaf* bplus_leaf_new_right(BplusTree* tree, BplusLeaf* leaf_left);
void       bplus_leaf_destroy(BplusTree* tree, BplusLeaf* leaf);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_LEAF_H__
