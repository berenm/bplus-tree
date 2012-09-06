/**
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __BPLUS_TREE_PRIVATE_H__
#define __BPLUS_TREE_PRIVATE_H__

#include "bplus_tree.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

#ifndef BPLUS_TREE_ORDER
# define BPLUS_TREE_ORDER 32
#endif /* ifndef BPLUS_TREE_ORDER */

typedef struct _BplusNode BplusNode;
typedef struct _BplusLeaf BplusLeaf;
typedef struct _BplusPath BplusPath;

struct _BplusPath
{
    size_t     length;
    size_t     index[16];
    BplusNode* leaf;
};

struct _BplusTree
{
    BplusNode* root;

    BplusLeaf* first;
    BplusLeaf* last;

    size_t   height;
    gboolean allow_duplicate_keys;

#ifdef BPLUS_TREE_GATHER_STATS
    size_t node_count;
    size_t leaf_count;
    size_t underflow_count;
    size_t overflow_count;
#endif /* ifdef BPLUS_TREE_GATHER_STATS */
};

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_TREE_PRIVATE_H__
