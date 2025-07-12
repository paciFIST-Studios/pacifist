// paciFIST studios. 2025. MIT License

/**  Linked List, Wikipedia
 *
 *       "In computer science, a linked list, is a linear collection of 
 *        data elements, whose order is not given by their physical
 *        placement in memory.
 *
 *        [...] This structure allows for efficient insertion or removal
 *        of elements, from any position in the sequence
 *
 *        [...] The principle benefit over a conventional array, is that
 *        list elements can be easily inserted or removed, without 
 *        reallocation or reorganization of of the entire structure
 *
 *
 *    Storage:   O(n)
 *    Search:    O(n)
 *    Edit:      O(n)
 *
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "../../core/common.h"


typedef struct LinkedListNode_t {
    struct LinkedListNode_t* next;

    void* data;
    int data_type;
} LinkedListNode_t;


LinkedListNode_t* linked_list_push(LinkedListNode_t* head, void* data, int data_type);

#endif //LINKEDLIST_H
