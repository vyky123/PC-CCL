#include "ccl.h"

int list_contains(linked_list *head, int value) {
    linked_list *it = head;
    while(it) {
        if (it->value == value) {
            return TRUE;
        }
        it = it->next;
    }
    return FALSE;
}

int list_add(linked_list **head, int value) {
    linked_list *temp;

    if (!head) {
        return FALSE;
    }

    temp = malloc(sizeof(linked_list));
    if (!temp) {
        printf("ERR#3: Memory allocation was unsuccesful!\n");
        exit(ERR_3);
    }


    temp->value = value;
    temp->next = *head;
    *head = temp;

    return TRUE;
}