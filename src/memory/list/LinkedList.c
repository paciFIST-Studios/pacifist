// paciFIST studios. 2025. MIT License

// header
#include "../list/LinkedList.h"
// stdlib
#include <stdlib.h>
// framework
// project

LinkedListNode_t* linked_list_push(LinkedListNode_t* head, void* data, int data_type) {
    LinkedListNode_t* tmp = malloc(sizeof(LinkedListNode_t));
    tmp->data = data;
    tmp->data_type = data_type;
    tmp->next = NULL;

    if (head == NULL) {
        // linked list is empty
        head = tmp;
    }
    else {
        // need to traverse list, looking for the last node, which has a null next ptr
        LinkedListNode_t * p = head;
        while (p->next != NULL) {
            p = p->next;
        }
        
        // add the tmp node to the end
        p->next = tmp;
    }

    return head;
}