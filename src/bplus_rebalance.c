#include "bplus_rebalance.h"

#include "bplus_tree_private.h"
#include "bplus_insert.h"
#include "bplus_remove.h"
#include "bplus_search.h"

static void bplus_node_propagate_minimal_key(BplusNode* node)
{
    /* Update the node's children lesser key to the node's key if needed, this is used to keep the tree consistent after a removal */
    if (!bplus_node_is_leaf(node) && (node->size > 0))
    {
        node->items[0].key = node->key;
        BplusNode* first_child = (BplusNode*) node->items[0].data;
        first_child->key = node->key;

        bplus_node_propagate_minimal_key(first_child);
    }
}

static void bplus_node_split(BplusTree* tree, BplusNode* left_node)
{
    g_return_if_fail(tree != NULL);
    g_return_if_fail(left_node != NULL);
    // g_debug("split node %p", left_node);

    size_t const total_size = left_node->size;

    /* Create the new right sibling */
    BplusNode* right_node = bplus_node_create(tree, bplus_node_is_leaf(left_node), left_node);

    /* Compute the partition of the current node's data */
    BplusKey const first_key = left_node->key;
    BplusKey const last_key  = bplus_node_last(left_node)->key;
    size_t         right_node_offset;
    if (first_key == last_key)
    {
        right_node->key   = first_key;
        right_node_offset = total_size / 2;
    }
    else
    {
        right_node->key   = first_key + (last_key - first_key) / 2;
        right_node_offset = bplus_node_get_key_index_after_last(left_node, right_node->key);
        right_node->key++;
    }

    /* Move the right part of the data partition of new sibling */
    bplus_node_copy_data(right_node, 0, left_node, right_node_offset, total_size - right_node_offset);

    /* If the new sibling is not a leaf, the sibling key has to be the minimal key of its children */
    if (!bplus_node_is_leaf(left_node))
        right_node->key = right_node->items[0].key;

    BplusNode* parent = left_node->parent;
    if (!parent)
    {
        /* If the current node does not have a parent, create a new one, it's the new tree root */
        parent            = bplus_node_create(tree, 0, 0);
        parent->key       = left_node->key;
        tree->root        = parent;
        left_node->parent = parent;

        parent->items[0].data = left_node;
        parent->items[0].key  = left_node->key;
        parent->size          = 1;
    }

    right_node->parent = parent;
    bplus_node_insert_after(tree, parent, right_node->key, right_node, left_node);

    /* If new sibling is full, split it again, this may seem weird, but this is required because we allow multiple entries with the same key */
    if (right_node->size >= BPLUS_TREE_ORDER)
        bplus_node_split(tree, right_node);

} /* bplus_node_split */

static void bplus_node_update_parent(BplusNode* node)
{
    /* Update the current node children' parent link */
    if (!bplus_node_is_leaf(node))
        for (size_t i = 0; i < node->size; ++i)
            ((BplusNode*) node->items[i].data)->parent = node;

}

static void bplus_node_collapse_root(BplusTree* tree)
{
    g_return_if_fail(tree != NULL);

    /* If the root only has one child, we can collapse it, until it is also a leaf node */
    while (tree->root->size <= 1 && !bplus_node_is_leaf(tree->root))
    {
        BplusNode* node = tree->root;
        tree->root = (BplusNode*) node->items[0].data;
        bplus_node_destroy(tree, node);
    }

    tree->root->parent = 0;
}
