#ifndef BPLUS_TREE_ORDER
# define BPLUS_TREE_ORDER 4
#endif /* ifndef BPLUS_TREE_ORDER */

#include "bplus_tree.h"
#include "bplus_tree_private.h"
#include "bplus_foreach.h"
#include "bplus_insert.h"
#include "bplus_iterator.h"
#include "bplus_leaf.h"
#include "bplus_node.h"
#include "bplus_print.h"
#include "bplus_rebalance.h"
#include "bplus_remove.h"
#include "bplus_search.h"

void test_search_key_index(void)
{
    // BplusTree tree;
    // BplusNode node;

    // node.size = 0;
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 0) == 0);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 0xFF) == 0);

    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 0) == 0);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 0xFF) == 0);

    // node.keys[0] = 1;
    // node.size++;
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 0) == 0);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 1) == 0);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 2) == 0);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 0xFF) == 0);

    // node.keys[0] = 1;
    // node.keys[1] = 1;
    // node.size++;
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 0) == 0);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 1) == 1);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 2) == 1);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 0xFF) == 1);

    // node.keys[0] = 1;
    // node.keys[1] = 1;
    // node.keys[2] = 2;
    // node.size++;
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 0) == 0);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 1) == 1);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 2) == 2);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 3) == 2);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 0xFF) == 2);

    // node.keys[0] = 1;
    // node.keys[1] = 1;
    // node.keys[2] = 3;
    // node.keys[3] = 3;
    // node.size++;
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 0) == 0);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 1) == 1);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 2) == 1);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 3) == 3);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 4) == 3);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 0xFF) == 3);

    // node.keys[0] = 1;
    // node.keys[1] = 1;
    // node.keys[2] = 1;
    // node.keys[3] = 1;
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 0) == 0);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 1) == 3);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 2) == 3);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 3) == 3);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 4) == 3);
    // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 0xFF) == 3);

    // // node.keys[0] = 1;
    // // node.keys[1] = 2;
    // // node.keys[2] = 3;
    // // node.keys[3] = 4;
    // // node.keys[4] = 4;
    // // node.keys[5] = 4;
    // // node.keys[6] = 6;
    // // node.keys[7] = 6;
    // // node.keys[8] = 9;
    // // node.size  = 9;
    // // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 0) == 0);
    // // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 1) == 0);
    // // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 2) == 1);
    // // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 3) == 2);
    // // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 4) == 5);
    // // g_assert(bplus_tree_search_key_index_gt(&tree, node.size, node.keys, 0xFF) == 8);
} /* test_search_key_index */

void test_search_bplus_tree_new(void)
{
    BplusTree* tree = bplus_tree_create();

    g_assert(tree != NULL);
    g_assert(tree->root != NULL);
    g_assert((void*) tree->root == (void*) tree->first);
    g_assert((void*) tree->root == (void*) tree->last);

    g_assert(bplus_node_is_leaf(tree->root));

    // g_assert(tree->root->parent == NULL);
    g_assert(tree->root->size == 0);

    g_assert(tree->first->leaf_link->left == NULL);
    g_assert(tree->first->leaf_link->right == NULL);

    bplus_tree_destroy(tree);
}

void test_search_bplus_node_insert_at(void)
{
    // BplusTree* tree = bplus_tree_create();

    // bplus_node_insert_at(tree, tree->root, 0, 0, (void*) 1);

    // g_assert(bplus_node_is_leaf(tree->root));
    // g_assert(tree->root->parent == NULL);
    // g_assert(tree->root-.size == 1);

    // g_assert(bplus_key_first(tree->root) == 0);
    // g_assert(bplus_value_first(tree->root) == (void*) 1);

    // bplus_node_insert_at(tree, tree->root, 0, 0, (void*) 2);
    // bplus_node_insert_at(tree, tree->root, 0, 0, (void*) 3);

    // g_assert(bplus_value_first(tree->root) == (void*) 3);
    // g_assert(bplus_value_at(tree->root, 1) == (void*) 2);
    // g_assert(bplus_value_at(tree->root, 2) == (void*) 1);

    // bplus_node_insert_at(tree, tree->root, 0, 0, (void*) 4);

    // /* We should have splitted here */
    // g_assert(bplus_node_is_leaf(!tree->root) || bplus_tree_print(tree));
    // g_assert(tree->root->parent == NULL);
    // g_assert(tree->root-.size == 2);

    // g_assert(bplus_key_first(tree->root) == 0);
    // BplusNode* left = (BplusNode*) bplus_value_first(tree->root);
    // g_assert(left != NULL);

    // g_assert(bplus_key_at(tree->root, 1) == 0);
    // BplusNode* right = (BplusNode*) bplus_value_at(tree->root, 1);
    // g_assert(right != NULL);

    // g_assert(bplus_node_is_leaf(left));
    // g_assert(left->parent == tree->root);
    // g_assert(left-.size == 2);
    // g_assert(bplus_key_first(left) == 0);
    // g_assert(bplus_value_first(left) == (void*) 4);
    // g_assert(bplus_key_at(left, 1) == 0);
    // g_assert(bplus_value_at(left, 1) == (void*) 3);

    // g_assert(bplus_node_is_leaf(right));
    // g_assert(right->parent == tree->root);
    // g_assert(right-.size == 2);
    // g_assert(bplus_key_first(right) == 0);
    // g_assert(bplus_value_first(right) == (void*) 2);
    // g_assert(bplus_key_at(right, 1) == 0);
    // g_assert(bplus_value_at(right, 1) == (void*) 1);

    // bplus_node_insert_at(tree, left, 2, 0, (void*) 1);
    // bplus_node_insert_at(tree, left, 2, 0, (void*) 2);

    // g_assert(tree->root-.size == 3);

    // g_assert(bplus_key_at(tree->root, 1) == 0);
    // BplusNode* middle = (BplusNode*) bplus_value_at(tree->root, 1);
    // g_assert(middle != NULL);
    // g_assert(middle != left);
    // g_assert(middle != right);

    // g_assert(bplus_key_first(tree->root) == 0);
    // g_assert(bplus_value_first(tree->root) == left);
    // g_assert(bplus_key_at(tree->root, 1) == 0);
    // g_assert(bplus_value_at(tree->root, 1) == middle);
    // g_assert(bplus_key_at(tree->root, 2) == 0);
    // g_assert(bplus_value_at(tree->root, 2) == right);

    // g_assert(bplus_node_is_leaf(middle));
    // g_assert(middle->parent == tree->root);
    // g_assert(middle-.size == 2);
    // g_assert(bplus_key_first(middle) == 0);
    // g_assert(bplus_value_first(middle) == (void*) 2);
    // g_assert(bplus_key_at(middle, 1) == 0);
    // g_assert(bplus_value_at(middle, 1) == (void*) 1);

    // g_assert(tree->first == (BplusNode*) left);
    // g_assert(tree->last == (BplusNode*) right);
    // g_assert(((BplusNode*) left)->right == (BplusNode*) middle);
    // g_assert(((BplusNode*) right)->left == (BplusNode*) middle);
    // g_assert(((BplusNode*) left)->left == NULL);
    // g_assert(((BplusNode*) right)->right == NULL);
    // g_assert(((BplusNode*) middle)->left == (BplusNode*) left);
    // g_assert(((BplusNode*) middle)->right == (BplusNode*) right);

    // bplus_tree_destroy(tree);
} /* test_search_bplus_node_insert_at */

void test_search_bplus_tree_insert_k0(void)
{
    BplusTree* tree = bplus_tree_create();

    bplus_tree_insert(tree, 0, (void*) 1);

    g_assert(bplus_node_is_leaf(tree->root));

    // g_assert(tree->root->parent == NULL);
    g_assert(tree->root->size == 1);

    g_assert(bplus_key_first(tree->root) == 0);
    g_assert(bplus_value_first(tree->root) == (void*) 1);

    bplus_tree_insert(tree, 0, (void*) 2);
    bplus_tree_insert(tree, 0, (void*) 3);

    g_assert(bplus_value_first(tree->root) == (void*) 1);
    g_assert(bplus_value_at(tree->root, 1) == (void*) 2);
    g_assert(bplus_value_at(tree->root, 2) == (void*) 3);

    bplus_tree_insert(tree, 0, (void*) 4);

    /* We should have splitted here */
    g_assert(!bplus_node_is_leaf(tree->root) || bplus_tree_print(tree, ""));

    // g_assert(tree->root->parent == NULL);
    g_assert(tree->root->size == 2);

    g_assert(bplus_key_first(tree->root) == 0);
    BplusNode* left = (BplusNode*) bplus_value_first(tree->root);
    g_assert(left != NULL);

    g_assert(bplus_key_at(tree->root, 1) == 0);
    BplusNode* right = (BplusNode*) bplus_value_at(tree->root, 1);
    g_assert(right != NULL);

    g_assert(bplus_node_is_leaf(left));

    // g_assert(left->parent == tree->root);
    g_assert(left->size == 2);
    g_assert(bplus_key_first(left) == 0);
    g_assert(bplus_value_first(left) == (void*) 1);
    g_assert(bplus_key_at(left, 1) == 0);
    g_assert(bplus_value_at(left, 1) == (void*) 2);

    g_assert(bplus_node_is_leaf(right));

    // g_assert(right->parent == tree->root);
    g_assert(right->size == 2);
    g_assert(bplus_key_first(right) == 0);
    g_assert(bplus_value_first(right) == (void*) 3);
    g_assert(bplus_key_at(right, 1) == 0);
    g_assert(bplus_value_at(right, 1) == (void*) 4);

    bplus_tree_insert(tree, 0, (void*) 5);
    bplus_tree_insert(tree, 0, (void*) 6);

    g_assert(tree->root->size == 3);

    g_assert(bplus_key_at(tree->root, 2) == 0);
    BplusNode* middle = right;
    right = (BplusNode*) bplus_value_at(tree->root, 2);
    g_assert(middle != NULL);
    g_assert(middle != left);
    g_assert(middle != right);

    g_assert(bplus_key_first(tree->root) == 0);
    g_assert(bplus_value_first(tree->root) == left);
    g_assert(bplus_key_at(tree->root, 1) == 0);
    g_assert(bplus_value_at(tree->root, 1) == middle);
    g_assert(bplus_key_at(tree->root, 2) == 0);
    g_assert(bplus_value_at(tree->root, 2) == right);

    g_assert(bplus_node_is_leaf(right));

    // g_assert(right->parent == tree->root);
    g_assert(right->size == 2);
    g_assert(bplus_key_first(right) == 0);
    g_assert(bplus_value_first(right) == (void*) 5);
    g_assert(bplus_key_at(right, 1) == 0);
    g_assert(bplus_value_at(right, 1) == (void*) 6);

    g_assert(tree->first == (BplusNode*) left);
    g_assert(tree->last == (BplusNode*) right);
    g_assert(((BplusNode*) left)->leaf_link->right == (BplusNode*) middle);
    g_assert(((BplusNode*) right)->leaf_link->left == (BplusNode*) middle);
    g_assert(((BplusNode*) left)->leaf_link->left == NULL);
    g_assert(((BplusNode*) right)->leaf_link->right == NULL);
    g_assert(((BplusNode*) middle)->leaf_link->left == (BplusNode*) left);
    g_assert(((BplusNode*) middle)->leaf_link->right == (BplusNode*) right);

    bplus_tree_destroy(tree);
} /* test_search_bplus_tree_insert_k0 */

void test_search_bplus_tree_insert_k(void)
{
    BplusTree* tree = bplus_tree_create();

    bplus_tree_print(tree, "label=\"insert 3: 1\";");
    bplus_tree_insert(tree, 3, (void*) 1);

    g_assert(bplus_node_is_leaf(tree->root));

    // g_assert(tree->root->parent == NULL);
    g_assert(tree->root->size == 1);

    g_assert(bplus_key_first(tree->root) == 3);
    g_assert(bplus_value_first(tree->root) == (void*) 1);

    bplus_tree_print(tree, "label=\"insert 1: 2\";");
    bplus_tree_insert(tree, 1, (void*) 2);
    g_assert(bplus_value_first(tree->root) == (void*) 2);
    g_assert(bplus_value_at(tree->root, 1) == (void*) 1);

    bplus_tree_print(tree, "fabel=\"insert 2: 3\";");
    bplus_tree_insert(tree, 2, (void*) 3);

    g_assert(bplus_value_first(tree->root) == (void*) 2);
    g_assert(bplus_value_at(tree->root, 1) == (void*) 3);
    g_assert(bplus_value_at(tree->root, 2) == (void*) 1);

    bplus_tree_print(tree, "label=\"insert 2: 4\";");
    bplus_tree_insert(tree, 2, (void*) 4);

    bplus_tree_print(tree, "");

    /* We should have splitted here */
    g_assert(!bplus_node_is_leaf(tree->root));

    // g_assert(tree->root->parent == NULL);
    g_assert(tree->root->size == 2);

    g_assert(bplus_key_first(tree->root) == 1);
    BplusNode* left = (BplusNode*) bplus_value_first(tree->root);
    g_assert(left != NULL);

    g_assert(bplus_key_at(tree->root, 1) == 2 || bplus_tree_print(tree, ""));
    BplusNode* right = (BplusNode*) bplus_value_at(tree->root, 1);
    g_assert(right != NULL);

    g_assert(bplus_node_is_leaf(left));

    // g_assert(left->parent == tree->root);
    g_assert(left->size == 2);
    g_assert(bplus_key_first(left) == 1);
    g_assert(bplus_value_first(left) == (void*) 2);
    g_assert(bplus_key_at(left, 1) == 2);
    g_assert(bplus_value_at(left, 1) == (void*) 3);

    g_assert(bplus_node_is_leaf(right));

    // g_assert(right->parent == tree->root);
    g_assert(right->size == 2);
    g_assert(bplus_key_first(right) == 2);
    g_assert(bplus_value_first(right) == (void*) 4);
    g_assert(bplus_key_at(right, 1) == 3);
    g_assert(bplus_value_at(right, 1) == (void*) 1);

    g_assert(bplus_key_first(tree->root) == 1);
    g_assert(bplus_key_at(tree->root, 1) == 2);

    bplus_tree_insert(tree, 3, (void*) 5);
    bplus_tree_insert(tree, 3, (void*) 6);
    g_assert(tree->root->size == 3);

    /* Should not have splitted */
    g_assert(bplus_value_first(tree->root) == (void*) left);
    g_assert(bplus_value_at(tree->root, 1) == (void*) right);

    bplus_tree_insert(tree, 2, (void*) 7);
    g_assert(tree->root->size == 3);
    g_assert(bplus_key_first(tree->root) == 1);
    g_assert(bplus_key_at(tree->root, 1) == 2);
    g_assert(bplus_key_at(tree->root, 2) == 3);

    // /* Should split root again */
    // bplus_tree_insert(tree, 4, (void*) 8);
    // g_assert(tree->root-.size == 3);
    // g_assert(bplus_value_first(tree->root) != (void*) left || bplus_tree_print(tree));
    // g_assert(bplus_value_at(tree->root, 1) != (void*) right);

    // g_assert(bplus_key_first(tree->root) == 1);
    // g_assert(bplus_key_at(tree->root, 1) == 3);

    /* This should propagate 0 up to root */
    bplus_tree_insert(tree, 0, (void*) 9);
    g_assert((bplus_key_first(tree->root) == 0) || bplus_tree_print(tree, ""));

    // for (int i = 0; i < 10000; ++i) {
    // bplus_tree_insert(tree, random(), (void*) i + 1);
    // }

    bplus_tree_destroy(tree);
} /* test_search_bplus_tree_insert_k */

void test_search_bplus_tree_remove_k(void)
{
    BplusTree* tree = bplus_tree_create();

    bplus_tree_insert(tree, 3, (void*) 1);
    bplus_tree_insert(tree, 1, (void*) 2);
    bplus_tree_insert(tree, 2, (void*) 3);
    bplus_tree_insert(tree, 2, (void*) 4);
    bplus_tree_insert(tree, 3, (void*) 5);
    bplus_tree_insert(tree, 3, (void*) 6);
    bplus_tree_insert(tree, 2, (void*) 7);
    bplus_tree_insert(tree, 4, (void*) 8);
    bplus_tree_insert(tree, 0, (void*) 9);

    bplus_tree_print(tree, "label=\"remove(0)\";");
    bplus_tree_remove_first(tree, 0);
    bplus_tree_print(tree, "label=\"remove(1)\";");
    bplus_tree_remove_first(tree, 1);
    bplus_tree_print(tree, "label=\"remove(4)\";");
    bplus_tree_remove_first(tree, 4);
    bplus_tree_print(tree, "label=\"remove(3)\";");
    bplus_tree_remove_first(tree, 3);
    bplus_tree_print(tree, "label=\"remove(2)\";");
    bplus_tree_remove_first(tree, 2);
    bplus_tree_print(tree, "label=\"remove(3)\";");
    bplus_tree_remove_first(tree, 3);
    bplus_tree_print(tree, "label=\"remove(3)\";");
    bplus_tree_remove_first(tree, 3);
    bplus_tree_print(tree, "label=\"remove(2)\";");
    bplus_tree_remove_first(tree, 2);
    bplus_tree_print(tree, "label=\"remove(2)\";");
    bplus_tree_remove_first(tree, 2);
    bplus_tree_print(tree, "");

    bplus_tree_destroy(tree);
} /* test_search_bplus_tree_insert_k */

int main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/tree/search_key_index", test_search_key_index);
    g_test_add_func("/tree/new", test_search_bplus_tree_new);
    g_test_add_func("/tree/insert_at", test_search_bplus_node_insert_at);
    g_test_add_func("/tree/insert_k0", test_search_bplus_tree_insert_k0);
    g_test_add_func("/tree/insert_k", test_search_bplus_tree_insert_k);
    g_test_add_func("/tree/remove_k", test_search_bplus_tree_remove_k);

    g_test_run();
    return 0;
}
