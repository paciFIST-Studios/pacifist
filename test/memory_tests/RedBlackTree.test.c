// paciFIST studios. 2025. MIT License

#ifndef RED_BLACK_TREE_TEST_C
#define RED_BLACK_TREE_TEST_C

#include <check.h>

#include "../../src/memory/RedBlackTree.h"

START_TEST(struct_has_correct_members_RedBlackTree_t) {
    // I don't know if we have to initialize this or what
    struct RedBlackTree_t tree;
    tree.root = &tree.template_nil_node;
    tree.root->color = RED;
    tree.root->left = NULL;
    tree.root->right = NULL;
    tree.root->parent = NULL;
    tree.root->data = NULL;
    
    ck_assert(tree.root != NULL);
    ck_assert(tree.root->color == tree.template_nil_node.color);
    ck_assert_ptr_null(tree.root->left);
    ck_assert_ptr_null(tree.root->right);
    ck_assert_ptr_null(tree.root->parent);
    ck_assert_ptr_null(tree.root->data);

    ck_assert_ptr_null(tree.template_nil_node.data);
    ck_assert_ptr_null(tree.template_nil_node.left);
    ck_assert_ptr_null(tree.template_nil_node.right);
    ck_assert_ptr_null(tree.template_nil_node.parent);
}
END_TEST

START_TEST(struct_is_of_correct_size_RedBlackTree_t) {
    struct RedBlackTree_t tree;
    ck_assert(sizeof(tree) == 48);
}
END_TEST


#endif // RED_BLACK_TREE_TEST_C
