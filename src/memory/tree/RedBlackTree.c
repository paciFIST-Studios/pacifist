// paciFIST studios. 2025. MIT License


#include "../tree/RedBlackTree.h"

// stdlib
#include <stddef.h>
// framework
// project




//RedBlackTreeNode_t* red_black_tree_rotate_subtree(
//    RedBlackTree_t* tree,
//    RedBlackTreeNode_t* subtree,
//    ERedBlackTreeNodeDirection_t direction
//) {
//    RedBlackTreeNode_t* subtree_parent = subtree->parent;
//    int rotation_direction = 1-direction;
//    RedBlackTreeNode_t* new_subtree_root = subtree->child[rotation_direction];
//    RedBlackTreeNode_t* new_subtree_child = new_subtree_root->child[direction];
//
//    subtree->child[rotation_direction] = new_subtree_child;
//
//    if (new_subtree_child != NULL) {
//        new_subtree_child->parent = subtree;
//    }
//
//    new_subtree_root->child[direction] = subtree;
//    new_subtree_child->parent = subtree_parent;
//    subtree->parent = new_subtree_root;
//
//    if (subtree_parent != NULL) {
//        subtree_parent->child[(subtree == subtree_parent->right)] = new_subtree_root; 
//    }
//    else {
//        tree->root = new_subtree_root;
//    }
//
//    return new_subtree_root;
//}


RedBlackTreeNode_t* red_black_tree_rotate_left(RedBlackTree_t* tree, RedBlackTreeNode_t* x) {
    RedBlackTreeNode_t* y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        tree->root = y;
    }
    else {
        if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
    }
    y->left = x;
    x->parent = y;

    return NULL;
}


RedBlackTreeNode_t* red_black_tree_rotate_right(RedBlackTree_t* tree, RedBlackTreeNode_t* y) {
    RedBlackTreeNode_t* x = y->left;
    y->left = x->right;
    if (x->right != NULL) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NULL) {
        tree->root = x;
    }
    else {
        if (y == y->parent->right) {
            y->parent->right = x;
        }
        else {
            y->parent->left = x;
        }
    }
    x->right = y;
    y->parent = x;

    return NULL;
}



