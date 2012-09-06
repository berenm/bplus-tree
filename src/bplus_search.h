/**
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __BPLUS_SEARCH_H__
#define __BPLUS_SEARCH_H__

#include "bplus_tree_private.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

void bplus_tree_get_path_to_key(BplusTree const* tree, BplusKey const key, BplusPath* path);
void bplus_tree_get_paths_to_key_range(BplusTree const* tree, BplusKey key_from, BplusKey key_to, BplusPath* path_from, BplusPath* path_to);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_SEARCH_H__
