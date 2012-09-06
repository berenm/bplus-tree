/**
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __BPLUS_TREE_H__
#define __BPLUS_TREE_H__

#include <stdint.h>
#include <stdlib.h>

#include <glib.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef uint64_t          BplusKey;
typedef void*             BplusValue;
typedef struct _BplusItem BplusItem;

struct _BplusItem
{
    BplusKey   key;
    BplusValue value;
};

typedef struct _BplusTree     BplusTree;
typedef struct _BplusIterator BplusIterator;

typedef void (BplusForeachItemFunc)(BplusTree const* tree, BplusItem* item, void* argument);

BplusTree* bplus_tree_new();
BplusTree* bplus_tree_new_full(gboolean allow_duplicate_keys);
void       bplus_tree_destroy(BplusTree* tree);
void       bplus_tree_destroy_each(BplusTree* tree, BplusForeachItemFunc* foreach, void* argument);

void       bplus_tree_insert(BplusTree* tree, BplusKey const key, BplusValue const value);
BplusValue bplus_tree_remove(BplusTree* tree, BplusKey const key);
BplusValue bplus_tree_remove_first(BplusTree* tree);
void       bplus_tree_remove_value(BplusTree* tree, BplusKey const key, BplusValue const value);
BplusValue bplus_tree_get(BplusTree* tree, BplusKey const key);

BplusValue bplus_tree_get_first(BplusTree const* tree);
BplusValue bplus_tree_get_nth(BplusTree const* tree, size_t position);

BplusIterator*   bplus_iterator_new(BplusTree const* tree);
void             bplus_iterator_destroy(BplusIterator* iterator);
gboolean         bplus_iterator_next(BplusIterator* iterator);
gboolean         bplus_iterator_previous(BplusIterator* iterator);
BplusItem const* bplus_iterator_get_item(BplusIterator const* iterator);
BplusIterator*   bplus_tree_first(BplusTree const* tree);
BplusIterator*   bplus_iterator_from_key(BplusTree const* tree, BplusKey const key);
BplusIterator*   bplus_iterator_to_key(BplusTree const* tree, BplusKey const key);
BplusIterator*   bplus_iterator_for_key(BplusTree const* tree, BplusKey const key);
BplusIterator*   bplus_iterator_for_key_range(BplusTree const* tree, BplusKey const key_from, BplusKey const key_to);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // ifndef __BPLUS_TREE_H__
