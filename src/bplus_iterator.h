#ifndef __BPLUS_ITERATOR_H__
#define __BPLUS_ITERATOR_H__

#include "bplus_tree.h"

#include "bplus_node.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

typedef struct _BplusIterator BplusIterator;
struct _BplusIterator
{
    BplusNode const* leaf;
    BplusItem const* leaf_end;
    BplusItem const* item;
    BplusItem const* end;
};

static BplusIterator* bplus_iterator_create(BplusTree const* tree, BplusNode const* leaf, BplusItem const* item, BplusItem const* end);
static void           bplus_iterator_destroy(BplusIterator* iterator);

static BplusIterator*   bplus_iterator_next(BplusIterator* iterator);
static void             bplus_iterator_end(BplusIterator* iterator);
static BplusItem const* bplus_iterator_get_item(BplusIterator const* iterator);

static BplusIterator* bplus_tree_first(BplusTree const* tree);
static BplusIterator* bplus_tree_first_with_key(BplusTree const* tree, BplusKey const key);
static BplusIterator* bplus_tree_range_first(BplusTree const* tree, BplusKey const key_from, BplusKey const key_to);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_ITERATOR_H__
