#ifndef __BPLUS_PRINT_H__
#define __BPLUS_PRINT_H__

#include "bplus_tree.h"

#include "bplus_node.h"

#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

static int  bplus_tree_print(BplusTree const* tree, char const* format);
static void bplus_tree_debug(BplusTree const* tree, FILE* file);
static void bplus_tree_debug_to_file(BplusTree* tree);
static void bplus_node_debug(BplusNode* node, void* arguments);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_PRINT_H__
