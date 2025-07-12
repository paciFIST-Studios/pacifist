// paciFIST studios. 2025. MIT License

#ifndef RED_BLACK_TREE_TEST_C
#define RED_BLACK_TREE_TEST_C

#include <check.h>

#include "../../src/memory/tree/RedBlackTree.h"


START_TEST(enum_has_correct_members__ERedBlackTreeNodeColor_t) {
    ck_assert_int_eq(ERedBlackTreeNode_Color_UNDEFINED, 0);
    ck_assert_int_eq(ERedBlackTreeNode_Color_RED, 1);
    ck_assert_int_eq(ERedBlackTreeNode_Color_BLACK, 2);
}END_TEST


START_TEST(enum_has_correct_members__ERedBlackTreeNodeDirection_t) {
    ck_assert_int_eq(ERedBlackTreeNode_Direction_LEFT, 0);    
    ck_assert_int_eq(ERedBlackTreeNode_Direction_RIGHT, 1);    
}END_TEST


START_TEST(struct_has_correct_members__RedBlackTree_t) {
    // I don't know if we have to initialize this or what
    RedBlackTree_t tree;
    tree.root = &tree.template_nil_node;
    tree.root->color = ERedBlackTreeNode_Color_RED;
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

START_TEST(struct_is_of_correct_size__RedBlackTree_t) {
    RedBlackTree_t tree;
    ck_assert(sizeof(tree) == 48);
}
END_TEST

START_TEST(struct_has_correct_members__RedBlackTreeNode_t) {
    RedBlackTreeNode_t node;
    node.left = NULL;
    node.right = NULL;
    node.parent = NULL;
    node.data = NULL;
    node.color = ERedBlackTreeNode_Color_UNDEFINED;
    node.key = 0;
    
    ck_assert_ptr_null(node.left);
    ck_assert_ptr_null(node.right);
    ck_assert_ptr_null(node.child[0]);
    ck_assert_ptr_null(node.child[1]);
    ck_assert_ptr_null(node.parent);
    ck_assert_ptr_null(node.data);
    ck_assert_int_eq(node.color, ERedBlackTreeNode_Color_UNDEFINED);
    ck_assert_int_eq(node.key, 0);
}
END_TEST




// test for this case: https://youtu.be/TlfQOdeFy0Y




#endif // RED_BLACK_TREE_TEST_C
