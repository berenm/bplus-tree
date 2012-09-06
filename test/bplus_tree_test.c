/**
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef BPLUS_TREE_ORDER
# define BPLUS_TREE_ORDER 4
#endif /* ifndef BPLUS_TREE_ORDER */

#include "bplus_tree_all.c"

void test_search_key_index(void)
{
    BplusTree tree;
    BplusNode node;

    node.length = 0;
    g_assert(bplus_node_get_key_index(&tree, &node, 0) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 0xFF) == 0);

    g_assert(bplus_node_get_key_index(&tree, &node, 0) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 0xFF) == 0);

    bplus_key_at(&node, 0) = 1;
    node.length++;
    g_assert(bplus_node_get_key_index(&tree, &node, 0) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 1) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 2) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 0xFF) == 0);

    bplus_key_at(&node, 0) = 1;
    bplus_key_at(&node, 1) = 1;
    node.length++;
    g_assert(bplus_node_get_key_index(&tree, &node, 0) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 1) == 1);
    g_assert(bplus_node_get_key_index(&tree, &node, 2) == 1);
    g_assert(bplus_node_get_key_index(&tree, &node, 0xFF) == 1);

    bplus_key_at(&node, 0) = 1;
    bplus_key_at(&node, 1) = 1;
    bplus_key_at(&node, 2) = 2;
    node.length++;
    g_assert(bplus_node_get_key_index(&tree, &node, 0) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 1) == 1);
    g_assert(bplus_node_get_key_index(&tree, &node, 2) == 2);
    g_assert(bplus_node_get_key_index(&tree, &node, 3) == 2);
    g_assert(bplus_node_get_key_index(&tree, &node, 0xFF) == 2);

    bplus_key_at(&node, 0) = 1;
    bplus_key_at(&node, 1) = 1;
    bplus_key_at(&node, 2) = 3;
    bplus_key_at(&node, 3) = 3;
    node.length++;
    g_assert(bplus_node_get_key_index(&tree, &node, 0) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 1) == 1);
    g_assert(bplus_node_get_key_index(&tree, &node, 2) == 1);
    g_assert(bplus_node_get_key_index(&tree, &node, 3) == 3);
    g_assert(bplus_node_get_key_index(&tree, &node, 4) == 3);
    g_assert(bplus_node_get_key_index(&tree, &node, 0xFF) == 3);

    bplus_key_at(&node, 0) = 1;
    bplus_key_at(&node, 1) = 1;
    bplus_key_at(&node, 2) = 1;
    bplus_key_at(&node, 3) = 1;
    g_assert(bplus_node_get_key_index(&tree, &node, 0) == 0);
    g_assert(bplus_node_get_key_index(&tree, &node, 1) == 3);
    g_assert(bplus_node_get_key_index(&tree, &node, 2) == 3);
    g_assert(bplus_node_get_key_index(&tree, &node, 3) == 3);
    g_assert(bplus_node_get_key_index(&tree, &node, 4) == 3);
    g_assert(bplus_node_get_key_index(&tree, &node, 0xFF) == 3);

    // bplus_key_at(&node, 0) = 1;
    // bplus_key_at(&node, 1) = 2;
    // bplus_key_at(&node, 2) = 3;
    // bplus_key_at(&node, 3) = 4;
    // bplus_key_at(&node, 4) = 4;
    // bplus_key_at(&node, 5) = 4;
    // bplus_key_at(&node, 6) = 6;
    // bplus_key_at(&node, 7) = 6;
    // bplus_key_at(&node, 8) = 9;
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
    BplusTree* tree = bplus_tree_new();

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
    // BplusTree* tree = bplus_tree_new();

    // bplus_node_insert_at(tree, tree->root, 0, 0, (void*) 1);

    // g_assert(tree->root->is_leaf);
    // g_assert(tree->root->parent == NULL);
    // g_assert(tree->root->length == 1);

    // g_assert(bplus_key_first(tree->root) == 0);
    // g_assert(bplus_value_first(tree->root) == (void*) 1);

    // bplus_node_insert_at(tree, tree->root, 0, 0, (void*) 2);
    // bplus_node_insert_at(tree, tree->root, 0, 0, (void*) 3);

    // g_assert(bplus_value_first(tree->root) == (void*) 3);
    // g_assert(bplus_value_at(tree->root, 1) == (void*) 2);
    // g_assert(bplus_value_at(tree->root, 2) == (void*) 1);

    // bplus_node_insert_at(tree, tree->root, 0, 0, (void*) 4);

    // /* We should have splitted here */
    // g_assert(!tree->root->is_leaf);
    // g_assert(tree->root->parent == NULL);
    // g_assert(tree->root->length == 2);

    // g_assert(bplus_key_first(tree->root) == 0);
    // BplusNode* left = (BplusNode*) bplus_value_first(tree->root);
    // g_assert(left != NULL);

    // g_assert(bplus_key_at(tree->root, 1) == 0);
    // BplusNode* right = (BplusNode*) bplus_value_at(tree->root, 1);
    // g_assert(right != NULL);

    // g_assert(left->is_leaf);
    // g_assert(left->parent == tree->root);
    // g_assert(left->length == 2);
    // g_assert(bplus_key_first(left) == 0);
    // g_assert(bplus_value_first(left) == (void*) 4);
    // g_assert(bplus_key_at(left, 1) == 0);
    // g_assert(bplus_value_at(left, 1) == (void*) 3);

    // g_assert(right->is_leaf);
    // g_assert(right->parent == tree->root);
    // g_assert(right->length == 2);
    // g_assert(bplus_key_first(right) == 0);
    // g_assert(bplus_value_first(right) == (void*) 2);
    // g_assert(bplus_key_at(right, 1) == 0);
    // g_assert(bplus_value_at(right, 1) == (void*) 1);

    // bplus_node_insert_at(tree, left, 2, 0, (void*) 1);
    // bplus_node_insert_at(tree, left, 2, 0, (void*) 2);

    // g_assert(tree->root->length == 3);

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

    // g_assert(middle->is_leaf);
    // g_assert(middle->parent == tree->root);
    // g_assert(middle->length == 2);
    // g_assert(bplus_key_first(middle) == 0);
    // g_assert(bplus_value_first(middle) == (void*) 2);
    // g_assert(bplus_key_at(middle, 1) == 0);
    // g_assert(bplus_value_at(middle, 1) == (void*) 1);

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
    BplusTree* tree = bplus_tree_new();

    bplus_tree_insert(tree, 0, (void*) 1);

    g_assert(tree->root->is_leaf);
    g_assert(tree->root->parent == NULL);
    g_assert(tree->root->length == 1);

    g_assert(bplus_key_first(tree->root) == 0);
    g_assert(bplus_value_first(tree->root) == (void*) 1);

    bplus_tree_insert(tree, 0, (void*) 2);
    bplus_tree_insert(tree, 0, (void*) 3);

    g_assert(bplus_value_first(tree->root) == (void*) 1);
    g_assert(bplus_value_at(tree->root, 1) == (void*) 2);
    g_assert(bplus_value_at(tree->root, 2) == (void*) 3);

    bplus_tree_insert(tree, 0, (void*) 4);

    /* We should have splitted here */
    g_assert(!tree->root->is_leaf);
    g_assert(tree->root->parent == NULL);
    g_assert(tree->root->length == 2);

    g_assert(bplus_key_first(tree->root) == 0);
    BplusNode* left = (BplusNode*) bplus_value_first(tree->root);
    g_assert(left != NULL);

    g_assert(bplus_key_at(tree->root, 1) == 0);
    BplusNode* right = (BplusNode*) bplus_value_at(tree->root, 1);
    g_assert(right != NULL);

    g_assert(left->is_leaf);
    g_assert(left->parent == tree->root);
    g_assert(left->length == 2);
    g_assert(bplus_key_first(left) == 0);
    g_assert(bplus_value_first(left) == (void*) 1);
    g_assert(bplus_key_at(left, 1) == 0);
    g_assert(bplus_value_at(left, 1) == (void*) 2);

    g_assert(right->is_leaf);
    g_assert(right->parent == tree->root);
    g_assert(right->length == 2);
    g_assert(bplus_key_first(right) == 0);
    g_assert(bplus_value_first(right) == (void*) 3);
    g_assert(bplus_key_at(right, 1) == 0);
    g_assert(bplus_value_at(right, 1) == (void*) 4);

    bplus_tree_insert(tree, 0, (void*) 5);
    bplus_tree_insert(tree, 0, (void*) 6);

    g_assert(tree->root->length == 3);

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

    g_assert(right->is_leaf);
    g_assert(right->parent == tree->root);
    g_assert(right->length == 2);
    g_assert(bplus_key_first(right) == 0);
    g_assert(bplus_value_first(right) == (void*) 5);
    g_assert(bplus_key_at(right, 1) == 0);
    g_assert(bplus_value_at(right, 1) == (void*) 6);

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
    BplusTree* tree = bplus_tree_new();

    bplus_tree_insert(tree, 3, (void*) 1);

    g_assert(tree->root->is_leaf);
    g_assert(tree->root->parent == NULL);
    g_assert(tree->root->length == 1);

    g_assert(bplus_key_first(tree->root) == 3);
    g_assert(bplus_value_first(tree->root) == (void*) 1);

    bplus_tree_insert(tree, 1, (void*) 2);
    g_assert(bplus_value_first(tree->root) == (void*) 2);
    g_assert(bplus_value_at(tree->root, 1) == (void*) 1);

    bplus_tree_insert(tree, 2, (void*) 3);

    g_assert(bplus_value_first(tree->root) == (void*) 2);
    g_assert(bplus_value_at(tree->root, 1) == (void*) 3);
    g_assert(bplus_value_at(tree->root, 2) == (void*) 1);

    bplus_tree_insert(tree, 2, (void*) 4);

    /* We should have splitted here */
    g_assert(!tree->root->is_leaf);
    g_assert(tree->root->parent == NULL);
    g_assert(tree->root->length == 2);

    g_assert(bplus_key_first(tree->root) == 1);
    BplusNode* left = (BplusNode*) bplus_value_first(tree->root);
    g_assert(left != NULL);

    g_assert(bplus_key_at(tree->root, 1) == 2);
    BplusNode* right = (BplusNode*) bplus_value_at(tree->root, 1);
    g_assert(right != NULL);

    g_assert(left->is_leaf);
    g_assert(left->parent == tree->root);
    g_assert(left->length == 2);
    g_assert(bplus_key_first(left) == 1);
    g_assert(bplus_value_first(left) == (void*) 2);
    g_assert(bplus_key_at(left, 1) == 2);
    g_assert(bplus_value_at(left, 1) == (void*) 3);

    g_assert(right->is_leaf);
    g_assert(right->parent == tree->root);
    g_assert(right->length == 2);
    g_assert(bplus_key_first(right) == 2);
    g_assert(bplus_value_first(right) == (void*) 4);
    g_assert(bplus_key_at(right, 1) == 3);
    g_assert(bplus_value_at(right, 1) == (void*) 1);

    g_assert(bplus_key_first(tree->root) == 1);
    g_assert(bplus_key_at(tree->root, 1) == 2);

    bplus_tree_insert(tree, 3, (void*) 5);
    bplus_tree_insert(tree, 3, (void*) 6);
    g_assert(tree->root->length == 3);

    /* Should not have splitted */
    g_assert(bplus_value_first(tree->root) == (void*) left);
    g_assert(bplus_value_at(tree->root, 1) == (void*) right);

    bplus_tree_insert(tree, 2, (void*) 7);
    g_assert(tree->root->length == 3);
    g_assert(bplus_key_first(tree->root) == 1);
    g_assert(bplus_key_at(tree->root, 1) == 2);
    g_assert(bplus_key_at(tree->root, 2) == 3);

    // /* Should split root again */
    // bplus_tree_insert(tree, 4, (void*) 8);
    // g_assert(tree->root->length == 3);
    // g_assert(bplus_value_first(tree->root) != (void*) left);
    // g_assert(bplus_value_at(tree->root, 1) != (void*) right);

    // g_assert(bplus_key_first(tree->root) == 1);
    // g_assert(bplus_key_at(tree->root, 1) == 3);

    /* This should propagate 0 up to root */
    bplus_tree_insert(tree, 0, (void*) 9);
    g_assert((bplus_key_first(tree->root) == 0));

    // for (int i = 0; i < 10000; ++i) {
    // bplus_tree_insert(tree, random(), (void*) i + 1);
    // }

    bplus_tree_destroy(tree);
} /* test_search_bplus_tree_insert_k */

void test_search_bplus_tree_remove_k(void)
{
    BplusTree* tree = bplus_tree_new();

    bplus_tree_insert(tree, 3, (void*) 1);
    bplus_tree_insert(tree, 1, (void*) 2);
    bplus_tree_insert(tree, 2, (void*) 3);
    bplus_tree_insert(tree, 2, (void*) 4);
    bplus_tree_insert(tree, 3, (void*) 5);
    bplus_tree_insert(tree, 3, (void*) 6);
    bplus_tree_insert(tree, 2, (void*) 7);
    bplus_tree_insert(tree, 4, (void*) 8);
    bplus_tree_insert(tree, 0, (void*) 9);

    // bplus_tree_print(tree, "label=\"remove(0)\";");
    bplus_tree_remove(tree, 0);

    // bplus_tree_print(tree, "label=\"remove(1)\";");
    bplus_tree_remove(tree, 1);

    // bplus_tree_print(tree, "label=\"remove(4)\";");
    bplus_tree_remove(tree, 4);

    // bplus_tree_print(tree, "label=\"remove(3)\";");
    bplus_tree_remove(tree, 3);

    // bplus_tree_print(tree, "label=\"remove(2)\";");
    bplus_tree_remove(tree, 2);

    // bplus_tree_print(tree, "label=\"remove(3)\";");
    bplus_tree_remove(tree, 3);

    // bplus_tree_print(tree, "label=\"remove(3)\";");
    bplus_tree_remove(tree, 3);

    // bplus_tree_print(tree, "label=\"remove(2)\";");
    bplus_tree_remove(tree, 2);

    // bplus_tree_print(tree, "label=\"remove(2)\";");
    bplus_tree_remove(tree, 2);

    // bplus_tree_print(tree, "");

    bplus_tree_destroy(tree);
} /* test_search_bplus_tree_insert_k */

void test_search_bplus_tree_iterator(void)
{
    BplusTree* tree = bplus_tree_new();

    bplus_tree_insert(tree, 3, (void*) 1);
    bplus_tree_insert(tree, 1, (void*) 2);
    bplus_tree_insert(tree, 2, (void*) 3);
    bplus_tree_insert(tree, 2, (void*) 4);
    bplus_tree_insert(tree, 3, (void*) 5);
    bplus_tree_insert(tree, 3, (void*) 6);
    bplus_tree_insert(tree, 2, (void*) 7);
    bplus_tree_insert(tree, 4, (void*) 8);
    bplus_tree_insert(tree, 0, (void*) 9);

    BplusKey   keys[]   = { 0, 1, 2, 2, 2, 3, 3, 3, 4 };
    BplusValue values[] = { (BplusValue*) 9, (BplusValue*) 2, (BplusValue*) 3, (BplusValue*) 4, (BplusValue*) 7, (BplusValue*) 1, (BplusValue*) 5, (BplusValue*) 6, (BplusValue*) 8 };

    BplusIterator* iterator = bplus_tree_first(tree);
    for (int i = 0; i < 9; ++i)
    {
        BplusItem const* item = bplus_iterator_get_item(iterator);
        g_assert(item->key == keys[i]);
        g_assert(item->value == values[i]);
        if (i < 8)
            bplus_iterator_next(iterator);
    }
    g_assert(!bplus_iterator_next(iterator));
    bplus_iterator_destroy(iterator);

    iterator = bplus_iterator_to_key(tree, 2);
    for (int i = 0; i < 5; ++i)
    {
        BplusItem const* item = bplus_iterator_get_item(iterator);
        g_assert(item->key == keys[i]);
        g_assert(item->value == values[i]);
        if (i < 4)
            bplus_iterator_next(iterator);
    }
    g_assert(!bplus_iterator_next(iterator));
    for (int i = 4; i >= 0; --i)
    {
        BplusItem const* item = bplus_iterator_get_item(iterator);
        g_assert(item->key == keys[i]);
        g_assert(item->value == values[i]);
        if (i > 0)
            bplus_iterator_previous(iterator);
    }
    g_assert(!bplus_iterator_previous(iterator));
    bplus_iterator_destroy(iterator);

    iterator = bplus_iterator_for_key(tree, 2);
    for (int i = 0; i < 3; ++i)
    {
        BplusItem const* item = bplus_iterator_get_item(iterator);
        g_assert(item->key == keys[i + 2]);
        g_assert(item->value == values[i + 2]);
        if (i < 2)
            bplus_iterator_next(iterator);
    }
    g_assert(!bplus_iterator_next(iterator));
    bplus_iterator_destroy(iterator);

    iterator = bplus_iterator_from_key(tree, 2);
    for (int i = 0; i < 7; ++i)
    {
        BplusItem const* item = bplus_iterator_get_item(iterator);
        g_assert(item->key == keys[i + 2]);
        g_assert(item->value == values[i + 2]);
        if (i < 6)
            bplus_iterator_next(iterator);
    }
    g_assert(!bplus_iterator_next(iterator));
    bplus_iterator_destroy(iterator);

    iterator = bplus_iterator_for_key_range(tree, 2, 3);
    for (int i = 0; i < 6; ++i)
    {
        BplusItem const* item = bplus_iterator_get_item(iterator);
        g_assert(item->key == keys[i + 2]);
        g_assert(item->value == values[i + 2]);
        if (i < 5)
            bplus_iterator_next(iterator);
    }
    g_assert(!bplus_iterator_next(iterator));
    bplus_iterator_destroy(iterator);

    iterator = bplus_iterator_for_key_range(tree, 3, 2);
    for (int i = 0; i < 6; ++i)
    {
        BplusItem const* item = bplus_iterator_get_item(iterator);
        g_assert(item->key == keys[i + 2]);
        g_assert(item->value == values[i + 2]);
        if (i < 5)
            bplus_iterator_next(iterator);
    }
    g_assert(!bplus_iterator_next(iterator));
    bplus_iterator_destroy(iterator);

    iterator = bplus_iterator_for_key_range(tree, 4, 5);
    for (int i = 0; i < 1; ++i)
    {
        BplusItem const* item = bplus_iterator_get_item(iterator);
        g_assert(item->key == keys[i + 8]);
        g_assert(item->value == values[i + 8]);
    }
    g_assert(!bplus_iterator_next(iterator));
    bplus_iterator_destroy(iterator);

    iterator = bplus_iterator_for_key_range(tree, 5, 5);
    g_assert(bplus_iterator_get_item(iterator) == NULL);
    g_assert(!bplus_iterator_next(iterator));
    bplus_iterator_destroy(iterator);

    iterator = bplus_iterator_for_key_range(tree, 5, 6);
    g_assert(bplus_iterator_get_item(iterator) == NULL);
    g_assert(!bplus_iterator_next(iterator));
    bplus_iterator_destroy(iterator);

    bplus_tree_destroy(tree);
} /* test_search_bplus_tree_iterator */

int main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/tree/search_key_index", test_search_key_index);
    g_test_add_func("/tree/new", test_search_bplus_tree_new);
    g_test_add_func("/tree/insert_at", test_search_bplus_node_insert_at);
    g_test_add_func("/tree/insert_k0", test_search_bplus_tree_insert_k0);
    g_test_add_func("/tree/insert_k", test_search_bplus_tree_insert_k);
    g_test_add_func("/tree/remove_k", test_search_bplus_tree_remove_k);
    g_test_add_func("/tree/iterator", test_search_bplus_tree_iterator);

    g_test_run();
    return 0;
}
