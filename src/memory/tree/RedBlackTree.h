// paciFIST studios. 2025. MIT License

/** Red Black Trees are data types which support self-balancing binary search.
 * Their claim to fame, is fast retrieval and storage of ordered information
 *
 *  Space Usage: O(n)
 *
 *  Search: O(log n) - worst case
 *  Insert: O(log n) - worst case
 *  Delete: O(log n) - worst case
 *
 *  Red Black Trees are better, in the worst case, than normal binary trees
 *
 *  Properties:
 *      1. every node is either red or black
 *      2. all null nodes are considered black
 *      3. a red node does NOT have a red child
 *      4. Every path from a node, to any of its leaf nodes, has the same number of black nodes
 *          a. (ie: is the same length, counting a null nodes)
 *
 *      5. (Ergo)
 *          - If a node N has 1 child, it's red   
 * 
 */

#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

// stdlib
// framework
// project
#include "../../core/common.h"


/** This enumeration helps to distinguish between the different colors nodes in the RB Tree
 * can take on.  These "colors" are just conventions, and could be anything which keeps "left"
 * distinct from "right"
 */
typedef enum ERedBlackTreeNodeColor_t {
    ERedBlackTreeNode_Color_UNDEFINED = 0,
    ERedBlackTreeNode_Color_RED = 1,
    ERedBlackTreeNode_Color_BLACK = 2,
} ERedBlackTreeNodeColor_t ;

/** While rotating a sub-tree, this enum is cast back to an int,
 *  so the direction can be determined mathematically, using addition,
 *  rather than logically, using program branching
 */
typedef enum ERedBlackTreeNodeDirection_t{
    ERedBlackTreeNode_Direction_LEFT = 0,
    ERedBlackTreeNode_Direction_RIGHT = 1,
} ERedBlackTreeNodeDirection_t;

// this macro determines if the tree should be rotated right, or rotated left
#define GET_TREE_DIRECTION(node) (node == node->parent->right ? ERedBlackTreeNode_Direction_RIGHT : ERedBlackTreeNode_Direction_LEFT)


/** 
 * 
 */
typedef struct RedBlackTreeNode_t {
    struct RedBlackTreeNode_t* parent;

    // This union allows access of left/right by using:
    //      node->left, node->right
    //      node->child[0], node->child[1]
    union {
        struct {
            struct RedBlackTreeNode_t* left;
            struct RedBlackTreeNode_t* right;
        };
        struct RedBlackTreeNode_t * child[2];
    };
    
    ERedBlackTreeNodeColor_t color;
    int key;
    void * data;
} RedBlackTreeNode_t;


typedef struct RedBlackTree_t {
    RedBlackTreeNode_t template_nil_node;
    RedBlackTreeNode_t* root;
} RedBlackTree_t;


/** Initializes a red black tree to a usable defualt state
 * 
 * @param tree - the tree to initialize
 * @return True, if initialization was successful, otherwise false
 */
bool32 red_black_tree_initialize(RedBlackTree_t* tree);


/** Rotates a subtree using some node from inside a RedBlackTree_t.
 * 
 * @param tree - the RedBlackTree which owns the subtree being rotated
 * @param subtree - the node, which is at the root of the subtree
 * @param direction - the direction of rotation right/left
 * @return RedBlackTreeNode_t - the new root node of the tree, after rotation
 */
RedBlackTreeNode_t* red_black_tree_rotate_subtree(RedBlackTree_t * tree, RedBlackTreeNode_t* subtree, ERedBlackTreeNodeDirection_t direction);


// https://youtu.be/TlfQOdeFy0Y 
RedBlackTreeNode_t* red_black_tree_rotate_left(RedBlackTree_t* tree, RedBlackTreeNode_t* subtree);
RedBlackTreeNode_t* red_black_tree_rotate_right(RedBlackTree_t* tree, RedBlackTreeNode_t* subtree);

#endif //REDBLACKTREE_H
