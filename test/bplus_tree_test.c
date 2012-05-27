#ifndef BPLUS_TREE_ORDER
# define BPLUS_TREE_ORDER 4
#endif /* ifndef BPLUS_TREE_ORDER */

#include "bplus_tree.c"

void test_search_key_index(void)
{
    BplusTree tree;
    BplusNode node;

    node.length = 0;
    g_assert(bplus_node_get_key_index(&tree, &node, 0) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 0xFF) == 0);

    g_assert(bplus_node_get_key_index(&tree, &node, 0) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 0xFF) == 0);

    node.keys[0] = 1;
    node.length++;
    g_assert(bplus_node_get_key_index(&tree, &node, 0) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 1) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 2) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 0xFF) == 0);

    node.keys[0] = 1;
    node.keys[1] = 1;
    node.length++;
    g_assert(bplus_node_get_key_index(&tree, &node, 0) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 1) == 1);
    g_assert(bplus_node_get_key_index(&tree, &node, 2) == 1);
    g_assert(bplus_node_get_key_index(&tree, &node, 0xFF) == 1);

    node.keys[0] = 1;
    node.keys[1] = 1;
    node.keys[2] = 2;
    node.length++;
    g_assert(bplus_node_get_key_index(&tree, &node, 0) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 1) == 1);
    g_assert(bplus_node_get_key_index(&tree, &node, 2) == 2);
    g_assert(bplus_node_get_key_index(&tree, &node, 3) == 2);
    g_assert(bplus_node_get_key_index(&tree, &node, 0xFF) == 2);

    node.keys[0] = 1;
    node.keys[1] = 1;
    node.keys[2] = 3;
    node.keys[3] = 3;
    node.length++;
    g_assert(bplus_node_get_key_index(&tree, &node, 0) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 1) == 1);
    g_assert(bplus_node_get_key_index(&tree, &node, 2) == 1);
    g_assert(bplus_node_get_key_index(&tree, &node, 3) == 3);
    g_assert(bplus_node_get_key_index(&tree, &node, 4) == 3);
    g_assert(bplus_node_get_key_index(&tree, &node, 0xFF) == 3);

    node.keys[0] = 1;
    node.keys[1] = 1;
    node.keys[2] = 1;
    node.keys[3] = 1;
    g_assert(bplus_node_get_key_index(&tree, &node, 0) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 1) == 3);
    g_assert(bplus_node_get_key_index(&tree, &node, 2) == 3);
    g_assert(bplus_node_get_key_index(&tree, &node, 3) == 3);
    g_assert(bplus_node_get_key_index(&tree, &node, 4) == 3);
    g_assert(bplus_node_get_key_index(&tree, &node, 0xFF) == 3);

    // node.keys[0] = 1;
    // node.keys[1] = 2;
    // node.keys[2] = 3;
    // node.keys[3] = 4;
    // node.keys[4] = 4;
    // node.keys[5] = 4;
    // node.keys[6] = 6;
    // node.keys[7] = 6;
    // node.keys[8] = 9;
    // node.length  = 9;
    // g_assert(bplus_node_get_key_index(&tree, &node, 0) == 0);
    // g_assert(bplus_node_get_key_index(&tree, &node, 1) == 0);
    // g_assert(bplus_node_get_key_index(&tree, &node, 2) == 1);
    // g_assert(bplus_node_get_key_index(&tree, &node, 3) == 2);
    // g_assert(bplus_node_get_key_index(&tree, &node, 4) == 5);
    // g_assert(bplus_node_get_key_index(&tree, &node, 0xFF) == 8);
} /* test_search_key_index */

void test_search_bplus_tree_new(void)
{
    BplusTree* tree = bplus_tree_new(TRUE);

    g_assert(tree != NULL);
    g_assert(tree->root != NULL);
    g_assert((void*) tree->root == (void*) tree->first);
    g_assert((void*) tree->root == (void*) tree->last);

    g_assert(tree->root->is_leaf);
    g_assert(tree->root->parent == NULL);
    g_assert(tree->root->length == 0);

    g_assert(tree->first->left == NULL);
    g_assert(tree->first->right == NULL);

    bplus_tree_destroy(tree);
}

void test_search_bplus_node_insert_at(void)
{
    // BplusTree* tree = bplus_tree_new(TRUE);

    // bplus_node_insert_at(tree, tree->root, 0, 0, (void*) 1);

    // g_assert(tree->root->is_leaf);
    // g_assert(tree->root->parent == NULL);
    // g_assert(tree->root->length == 1);

    // g_assert(bplus_key_first(tree->root) == 0);
    // g_assert(bplus_value_first(tree->root) == (void*) 1);

    // bplus_node_insert_at(tree, tree->root, 0, 0, (void*) 2);
    // bplus_node_insert_at(tree, tree->root, 0, 0, (void*) 3);

    // g_assert(bplus_value_first(tree->root) == (void*) 3);
    // g_assert(tree->root->values[1] == (void*) 2);
    // g_assert(tree->root->values[2] == (void*) 1);

    // bplus_node_insert_at(tree, tree->root, 0, 0, (void*) 4);

    // /* We should have splitted here */
    // g_assert(!tree->root->is_leaf || bplus_tree_print(tree));
    // g_assert(tree->root->parent == NULL);
    // g_assert(tree->root->length == 2);

    // g_assert(bplus_key_first(tree->root) == 0);
    // BplusNode* left = (BplusNode*) bplus_value_first(tree->root);
    // g_assert(left != NULL);

    // g_assert(tree->root->keys[1] == 0);
    // BplusNode* right = (BplusNode*) tree->root->values[1];
    // g_assert(right != NULL);

    // g_assert(left->is_leaf);
    // g_assert(left->parent == tree->root);
    // g_assert(left->length == 2);
    // g_assert(bplus_key_first(left) == 0);
    // g_assert(bplus_value_first(left) == (void*) 4);
    // g_assert(left->keys[1] == 0);
    // g_assert(left->values[1] == (void*) 3);

    // g_assert(right->is_leaf);
    // g_assert(right->parent == tree->root);
    // g_assert(right->length == 2);
    // g_assert(bplus_key_first(right) == 0);
    // g_assert(bplus_value_first(right) == (void*) 2);
    // g_assert(right->keys[1] == 0);
    // g_assert(right->values[1] == (void*) 1);

    // bplus_node_insert_at(tree, left, 2, 0, (void*) 1);
    // bplus_node_insert_at(tree, left, 2, 0, (void*) 2);

    // g_assert(tree->root->length == 3);

    // g_assert(tree->root->keys[1] == 0);
    // BplusNode* middle = (BplusNode*) tree->root->values[1];
    // g_assert(middle != NULL);
    // g_assert(middle != left);
    // g_assert(middle != right);

    // g_assert(bplus_key_first(tree->root) == 0);
    // g_assert(bplus_value_first(tree->root) == left);
    // g_assert(tree->root->keys[1] == 0);
    // g_assert(tree->root->values[1] == middle);
    // g_assert(tree->root->keys[2] == 0);
    // g_assert(tree->root->values[2] == right);

    // g_assert(middle->is_leaf);
    // g_assert(middle->parent == tree->root);
    // g_assert(middle->length == 2);
    // g_assert(bplus_key_first(middle) == 0);
    // g_assert(bplus_value_first(middle) == (void*) 2);
    // g_assert(middle->keys[1] == 0);
    // g_assert(middle->values[1] == (void*) 1);

    // g_assert(tree->first == (BplusLeaf*) left);
    // g_assert(tree->last == (BplusLeaf*) right);
    // g_assert(((BplusLeaf*) left)->right == (BplusLeaf*) middle);
    // g_assert(((BplusLeaf*) right)->left == (BplusLeaf*) middle);
    // g_assert(((BplusLeaf*) left)->left == NULL);
    // g_assert(((BplusLeaf*) right)->right == NULL);
    // g_assert(((BplusLeaf*) middle)->left == (BplusLeaf*) left);
    // g_assert(((BplusLeaf*) middle)->right == (BplusLeaf*) right);

    // bplus_tree_destroy(tree);
} /* test_search_bplus_node_insert_at */

void test_search_bplus_tree_insert_k0(void)
{
    BplusTree* tree = bplus_tree_new(TRUE);

    bplus_tree_insert(tree, 0, (void*) 1);

    g_assert(tree->root->is_leaf);
    g_assert(tree->root->parent == NULL);
    g_assert(tree->root->length == 1);

    g_assert(bplus_key_first(tree->root) == 0);
    g_assert(bplus_value_first(tree->root) == (void*) 1);

    bplus_tree_insert(tree, 0, (void*) 2);
    bplus_tree_insert(tree, 0, (void*) 3);

    g_assert(bplus_value_first(tree->root) == (void*) 1);
    g_assert(tree->root->values[1] == (void*) 2);
    g_assert(tree->root->values[2] == (void*) 3);

    bplus_tree_insert(tree, 0, (void*) 4);

    /* We should have splitted here */
    g_assert(!tree->root->is_leaf);
    g_assert(tree->root->parent == NULL);
    g_assert(tree->root->length == 2);

    g_assert(bplus_key_first(tree->root) == 0);
    BplusNode* left = (BplusNode*) bplus_value_first(tree->root);
    g_assert(left != NULL);

    g_assert(tree->root->keys[1] == 0);
    BplusNode* right = (BplusNode*) tree->root->values[1];
    g_assert(right != NULL);

    g_assert(left->is_leaf);
    g_assert(left->parent == tree->root);
    g_assert(left->length == 2);
    g_assert(bplus_key_first(left) == 0);
    g_assert(bplus_value_first(left) == (void*) 1);
    g_assert(left->keys[1] == 0);
    g_assert(left->values[1] == (void*) 2);

    g_assert(right->is_leaf);
    g_assert(right->parent == tree->root);
    g_assert(right->length == 2);
    g_assert(bplus_key_first(right) == 0);
    g_assert(bplus_value_first(right) == (void*) 3);
    g_assert(right->keys[1] == 0);
    g_assert(right->values[1] == (void*) 4);

    bplus_tree_insert(tree, 0, (void*) 5);
    bplus_tree_insert(tree, 0, (void*) 6);

    g_assert(tree->root->length == 3);

    g_assert(tree->root->keys[2] == 0);
    BplusNode* middle = right;
    right = (BplusNode*) tree->root->values[2];
    g_assert(middle != NULL);
    g_assert(middle != left);
    g_assert(middle != right);

    g_assert(bplus_key_first(tree->root) == 0);
    g_assert(bplus_value_first(tree->root) == left);
    g_assert(tree->root->keys[1] == 0);
    g_assert(tree->root->values[1] == middle);
    g_assert(tree->root->keys[2] == 0);
    g_assert(tree->root->values[2] == right);

    g_assert(right->is_leaf);
    g_assert(right->parent == tree->root);
    g_assert(right->length == 2);
    g_assert(bplus_key_first(right) == 0);
    g_assert(bplus_value_first(right) == (void*) 5);
    g_assert(right->keys[1] == 0);
    g_assert(right->values[1] == (void*) 6);

    g_assert(tree->first == (BplusLeaf*) left);
    g_assert(tree->last == (BplusLeaf*) right);
    g_assert(((BplusLeaf*) left)->right == (BplusLeaf*) middle);
    g_assert(((BplusLeaf*) right)->left == (BplusLeaf*) middle);
    g_assert(((BplusLeaf*) left)->left == NULL);
    g_assert(((BplusLeaf*) right)->right == NULL);
    g_assert(((BplusLeaf*) middle)->left == (BplusLeaf*) left);
    g_assert(((BplusLeaf*) middle)->right == (BplusLeaf*) right);

    bplus_tree_destroy(tree);
} /* test_search_bplus_tree_insert_k0 */

void test_search_bplus_tree_insert_k(void)
{
    BplusTree* tree = bplus_tree_new(TRUE);

    bplus_tree_insert(tree, 3, (void*) 1);

    g_assert(tree->root->is_leaf);
    g_assert(tree->root->parent == NULL);
    g_assert(tree->root->length == 1);

    g_assert(bplus_key_first(tree->root) == 3);
    g_assert(bplus_value_first(tree->root) == (void*) 1);

    bplus_tree_insert(tree, 1, (void*) 2);
    g_assert(bplus_value_first(tree->root) == (void*) 2);
    g_assert(tree->root->values[1] == (void*) 1);

    bplus_tree_insert(tree, 2, (void*) 3);

    g_assert(bplus_value_first(tree->root) == (void*) 2);
    g_assert(tree->root->values[1] == (void*) 3);
    g_assert(tree->root->values[2] == (void*) 1);

    bplus_tree_insert(tree, 2, (void*) 4);

    /* We should have splitted here */
    g_assert(!tree->root->is_leaf);
    g_assert(tree->root->parent == NULL);
    g_assert(tree->root->length == 2);

    g_assert(bplus_key_first(tree->root) == 1);
    BplusNode* left = (BplusNode*) bplus_value_first(tree->root);
    g_assert(left != NULL);

    g_assert(tree->root->keys[1] == 2 || bplus_tree_print(tree, ""));
    BplusNode* right = (BplusNode*) tree->root->values[1];
    g_assert(right != NULL);

    g_assert(left->is_leaf);
    g_assert(left->parent == tree->root);
    g_assert(left->length == 2);
    g_assert(bplus_key_first(left) == 1);
    g_assert(bplus_value_first(left) == (void*) 2);
    g_assert(left->keys[1] == 2);
    g_assert(left->values[1] == (void*) 3);

    g_assert(right->is_leaf);
    g_assert(right->parent == tree->root);
    g_assert(right->length == 2);
    g_assert(bplus_key_first(right) == 2);
    g_assert(bplus_value_first(right) == (void*) 4);
    g_assert(right->keys[1] == 3);
    g_assert(right->values[1] == (void*) 1);

    g_assert(bplus_key_first(tree->root) == 1);
    g_assert(tree->root->keys[1] == 2);

    bplus_tree_insert(tree, 3, (void*) 5);
    bplus_tree_insert(tree, 3, (void*) 6);
    g_assert(tree->root->length == 3);

    /* Should not have splitted */
    g_assert(bplus_value_first(tree->root) == (void*) left);
    g_assert(tree->root->values[1] == (void*) right);

    bplus_tree_insert(tree, 2, (void*) 7);
    g_assert(tree->root->length == 3);
    g_assert(bplus_key_first(tree->root) == 1);
    g_assert(tree->root->keys[1] == 2);
    g_assert(tree->root->keys[2] == 3);

    // /* Should split root again */
    // bplus_tree_insert(tree, 4, (void*) 8);
    // g_assert(tree->root->length == 3);
    // g_assert(bplus_value_first(tree->root) != (void*) left || bplus_tree_print(tree));
    // g_assert(tree->root->values[1] != (void*) right);

    // g_assert(bplus_key_first(tree->root) == 1);
    // g_assert(tree->root->keys[1] == 3);

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
    BplusTree* tree = bplus_tree_new(TRUE);

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
    bplus_tree_remove(tree, 0);
    bplus_tree_print(tree, "label=\"remove(1)\";");
    bplus_tree_remove(tree, 1);
    bplus_tree_print(tree, "label=\"remove(4)\";");
    bplus_tree_remove(tree, 4);
    bplus_tree_print(tree, "label=\"remove(3)\";");
    bplus_tree_remove(tree, 3);
    bplus_tree_print(tree, "label=\"remove(2)\";");
    bplus_tree_remove(tree, 2);
    bplus_tree_print(tree, "label=\"remove(3)\";");
    bplus_tree_remove(tree, 3);
    bplus_tree_print(tree, "label=\"remove(3)\";");
    bplus_tree_remove(tree, 3);
    bplus_tree_print(tree, "label=\"remove(2)\";");
    bplus_tree_remove(tree, 2);
    bplus_tree_print(tree, "label=\"remove(2)\";");
    bplus_tree_remove(tree, 2);
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
