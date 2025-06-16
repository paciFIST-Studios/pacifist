// paciFIST studios. 2025. MIT License

#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <stddef.h>

#include "../core/common.h"


enum ERedBlackTreeNodeType {
    UNDEFINED = 0,
    RED = 1,
    BLACK = 2,
};

struct RedBlackTreeNode_t {

    struct RedBlackTreeNode_t* left;
    struct RedBlackTreeNode_t* right;
    struct RedBlackTreeNode_t* parent;
    
    enum ERedBlackTreeNodeType color;

    void * data;
};


struct RedBlackTree_t {
    struct RedBlackTreeNode_t template_nil_node;
    struct RedBlackTreeNode_t* root;
};



inline bool32 red_black_tree_initialize(struct RedBlackTree_t* tree) {
    ASSERT_MESSAGE((tree != NULL), "RedBlackTreeInitialize cannot initialize a NULL pointer!");

    if (tree != NULL) {
        tree->root = &tree->template_nil_node;
        return TRUE;
    }

    return FALSE;
}


inline void red_black_tree_rotate_left(struct RedBlackTreeNode_t* a) {
    struct RedBlackTreeNode_t* b = a->right;

    a->right = b->left;

}



#endif //REDBLACKTREE_H
