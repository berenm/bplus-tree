#ifndef __BPLUS_TREE_PRIVATE_H__
#define __BPLUS_TREE_PRIVATE_H__

#include "bplus_tree.h"

#include "bplus_node.h"
#include "bplus_iterator.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

struct _BplusTree
{
    BplusNode* root;
    BplusNode* first;
    BplusNode* last;

    BplusIterator iterator;
};

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_TREE_PRIVATE_H__
