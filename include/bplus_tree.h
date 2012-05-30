#ifndef __BPLUS_TREE_H__
#define __BPLUS_TREE_H__

#include <stdint.h>
#include <glib.h>

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

#ifndef BPLUS_TREE_ORDER
# define BPLUS_TREE_ORDER 32
#endif /* ifndef BPLUS_TREE_ORDER */

typedef uint64_t BplusKey;
typedef void*    BplusValue;

typedef struct _BplusItem BplusItem;
struct _BplusItem
{
    BplusKey   key;
    BplusValue data;
};

typedef struct _BplusTree BplusTree;

static BplusTree* bplus_tree_create();
static void       bplus_tree_destroy(BplusTree* tree);
static void       bplus_tree_destroy_each(BplusTree* tree, void (* function)(void const* tree, BplusKey key, void* data, void* argument), void* argument);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_TREE_H__
