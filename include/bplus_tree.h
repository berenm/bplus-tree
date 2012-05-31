#ifndef __BPLUS_TREE_H__
#define __BPLUS_TREE_H__

#include <glib.h>

#ifndef BPLUS_TREE_ORDER
# define BPLUS_TREE_ORDER_P 5
# define BPLUS_TREE_ORDER   (1 << BPLUS_TREE_ORDER_P)
#endif /* ifndef BPLUS_TREE_ORDER */

#ifndef BPLUS_TREE_GENERIC

typedef uint64_t BplusKey;
#else /* ifndef BPLUS_TREE_GENERIC */

typedef gpointer BplusKey;
#endif /* ifndef BPLUS_TREE_GENERIC */

typedef void*             BplusValue;
typedef void const*       BplusConstValue;
typedef struct _BplusTree BplusTree;
typedef struct _BplusNode BplusNode;
typedef struct _BplusLeaf BplusLeaf;
typedef struct _BplusItem BplusItem;

#ifndef BPLUS_TREE_GENERIC
BplusTree* bplus_tree_new(gboolean allow_duplicate_keys);
#else /* ifndef BPLUS_TREE_GENERIC */
BplusTree* bplus_tree_new(gboolean allow_duplicate_keys, GCompareFunc key_compare_func);
#endif /* ifdef BPLUS_TREE_GENERIC */

void bplus_tree_destroy(BplusTree* bplus_tree);

void       bplus_tree_insert(BplusTree* bplus_tree, BplusKey const key, BplusValue const value);
void       bplus_tree_remove(BplusTree* bplus_tree, BplusKey const key);
void       bplus_tree_remove_value(BplusTree* bplus_tree, BplusKey const key, BplusValue const value);
BplusValue bplus_tree_get(BplusTree* bplus_tree, BplusKey const key);

#endif // ifndef __BPLUS_TREE_H__
