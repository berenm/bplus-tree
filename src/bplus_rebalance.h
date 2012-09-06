/**
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __BPLUS_REBALANCE_H__
#define __BPLUS_REBALANCE_H__

#include "bplus_tree_private.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

void bplus_rebalance_propagate(BplusTree const* tree, BplusPath* path);
void bplus_rebalance_overfilled(BplusTree* tree, BplusPath const* path);
void bplus_rebalance_underfilled(BplusTree* tree, BplusPath const* path);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_REBALANCE_H__
