/**
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __BPLUS_ITERATOR_H__
#define __BPLUS_ITERATOR_H__

#include "bplus_tree_private.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

struct _BplusIterator
{
    BplusItem const* item;
    BplusLeaf const* leaf;

    BplusLeaf const* leaf_from;
    BplusItem const* item_from;
    BplusLeaf const* leaf_to;
    BplusItem const* item_to;
};

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_ITERATOR_H__
