#ifndef __BPLUS_NODE_H__
#define __BPLUS_NODE_H__

#include "bplus_tree.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

typedef struct _BplusLeafLink BplusLeafLink;
typedef struct _BplusNode     BplusNode;

struct _BplusLeafLink
{
    BplusNode* right;
    BplusNode* left;
};

struct _BplusNode
{
    BplusNode*     parent;
    BplusLeafLink* leaf_link;

    BplusKey key;

    size_t    size;
    BplusItem items[BPLUS_TREE_ORDER];
};

#define bplus_node_first(node)     (node->items)
#define bplus_node_at(node, index) (node->items + index)
#define bplus_node_end(node)       (node->items + node->size)
#define bplus_node_last(node)      (bplus_node_end(node) - 1)

#define bplus_key_first(node)     bplus_node_first(node)->key
#define bplus_key_at(node, index) bplus_node_at(node, index)->key
#define bplus_key_last(node)      bplus_node_last(node)->key

#define bplus_value_first(node)     bplus_node_first(node)->data
#define bplus_value_at(node, index) bplus_node_at(node, index)->data
#define bplus_value_last(node)      bplus_node_last(node)->data

static BplusNode* bplus_node_create(BplusTree* tree, uint8_t const create_leaf, BplusNode* left_leaf);
static void       bplus_node_destroy(BplusTree* tree, BplusNode* node);

static uint8_t     bplus_node_is_leaf(BplusNode const* node);
static void bplus_node_move_and_resize_data(BplusNode* node, size_t const to_offset, size_t const from_offset);
static void bplus_node_copy_data(BplusNode* to_node, size_t const to_offset, BplusNode* from_node, size_t const from_offset, size_t count);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef __BPLUS_NODE_H__
