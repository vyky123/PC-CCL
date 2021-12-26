#include "ccl.h"
int add_equivalency2(linked_list **head, int value, int value2) {
    linked_list *temp = malloc(sizeof(linked_list));
    int temp_value;

    if (!temp) {
        printf("ERR#3: Memory allocation was unsuccesful!\n");
        exit(ERR_3);
    }

    if (value < value2) {
        temp_value = value;
        value = value2;
        value2 = temp_value;
    }

    temp->value = value;
    temp->value2 = value2;
    temp->next = *head;
    *head = temp;

    return TRUE;
}
int list_sort(linked_list **head) {
    int swapped;
    linked_list *temp;
    linked_list *temp2 = NULL;
    do {
        swapped = FALSE;
        temp = *head;

        while (temp->next != temp2) {
            if (temp->value < temp->next->value) {
                swap(temp, temp->next);
                swapped = TRUE;
            }
            temp = temp->next;
        }
        temp2 = temp;
    }
    while (swapped);
    return TRUE;
}


void swap(linked_list *first, linked_list *second) {
    int temp = first->value;

    first->value = second->value;
    second->value = temp;
    temp = first->value2;
    first->value2 = second->value2;
    second->value2 = temp;
}

int add_equivalency(linked_list **head, int value, int value2) {
    linked_list *temp = malloc(sizeof(linked_list));
    if (!temp) {
        printf("ERR#3: Memory allocation was unsuccesful!\n");
        exit(ERR_3);
    }
    temp->value = value;
    temp->value2 = value2;
    temp->next = *head;
    *head = temp;

    //   printf("New equivalency: %d -> %d \n", temp->value, temp->value2);
    return TRUE;
}

int update_equivalencies(linked_list **head, int value, int value2) {
    linked_list *it, *it2;
    int temp;

    if (value == value2) {
        return TRUE;
    }
    if (value < value2) {
        temp = value;
        value = value2;
        value2 = temp;
    }
    it = *head;
    /* Goes through the linked list and tries to find equal value to the new value */
    while(it) {
        /* The first value is in the list */
        if(it->value == value) {

            /* Both values are already present in the list */
            if (it->value2 == value2) {
                return TRUE;
            }

            /* The second value of equivalency list is different and might need to be updated */
            else {
                if (!update_equivalencies(head, value2, it->value2)) {
                    printf("Novinka: %d -> %d\n", value2, it->value2);
                    return FALSE;
                }

                it2 = *head;
////                while (it2) {
//////                    if (it2->value == value) {
//////                        it2->value = value2;
////                    }
////                  if (it2->value2 == it->value2) {
////                        it2->value2 = value2;
////                    }
////
////                    it2 = it2->next;
////                }
              //  it->value = value;
                it->value2 = value2;
                return TRUE;
            }

        }
        it = it->next;
    }

//    it = head;
//    while(it) {
//        /* The first value is in the list */
//        if (!(it->value == value && it->value2 == value2)) {
//            add_equivalency2(&head, value, value2);
//        }
//        it = it->next;
//    }
    add_equivalency(head, value, value2);
    return FALSE;
}
int list_contains(linked_list *head, int value) {
    linked_list *it;

    if (!head) {
        return FALSE;
    }

    it = head;
    while(it) {
        if (!it->value) {
            return FALSE;
        }
        if (it->value == value) {
            return TRUE;
        }
        it = it->next;
    }
    return FALSE;
}

int list_add(linked_list **head, int value, int value2) {
    linked_list *temp;

    if (!head) {
        return FALSE;
    }
    if (list_contains(*head, value)) {
        return FALSE;
    }
    temp = malloc(sizeof(linked_list));
    if (!temp) {
        printf("ERR#3: Memory allocation was unsuccesful!\n");
        exit(ERR_3);
    }

    temp->value = value;
    temp->value2 = value2;
    temp->next = *head;
    *head = temp;

    return TRUE;
}
void list_free(linked_list **head) {
    linked_list *next;

    if (!head) {
        return;
    }

    while (*head) {
        next = (*head)->next;
        free(*head);
        *head = next;
    }
    *head = NULL;
}