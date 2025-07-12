// paciFIST studios. 2025. MIT License

#ifndef LINKED_LIST_TEST_C
#define LINKED_LIST_TEST_C

#include <check.h>

#include "../../src/memory/list/LinkedList.h"


START_TEST(struct_has_correct_members__LinkedListNode_t) {
   LinkedListNode_t node;
   node.data = NULL;
   node.next = NULL;
   node.data_type = 0;

   ck_assert_ptr_null(node.data);
   ck_assert_ptr_null(node.next);
   ck_assert_int_eq(node.data_type, 0);
}
END_TEST

START_TEST(struct_is_of_correct_size__LinkedListNode_t) {
   LinkedListNode_t node;
   ck_assert_int_eq(sizeof(node), 24);
}
END_TEST


#endif // LINKED_LIST_TEST_C  
