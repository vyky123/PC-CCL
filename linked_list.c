#include "ccl.h"

int list_contains(linked_list *head, int value) {
    linked_list *it;

    if (!head) {
        return FALSE;
    }

    it = head;
    while(it) {
        if (it->value == value) {
            return TRUE;
        }
        it = it->next;
    }
    return FALSE;
}

int list_contains_equivalency(linked_list **head, int value, int value2) {
    linked_list *it, *it2;
    if (!head || !*head) {
        return FALSE;
    }

    it = *head;
 //   printf("T1: %d %d\n", it->value, it->value2);
    while(it) {
        if(it->value == value) {
            if (it->value2 == value2) {
                return TRUE;
            }
            else if (it->value2 != value2) {
                it->value2 = value2;

                it2 = *head;
//                printf("T1: %d %d; T2: %d %d (%d %d)\n", it->value, it->value2, it2->value, it2->value2, (*head)->value, (*head)->value2);
                while (it2) {
                    if (it2->value2 == it->value2) {
                        it2->value2 = it->value2;
                        return TRUE;
                    }
                    it2 = it2->next;
                }
                return TRUE;
            }
        }
        it = it->next;
    }
    return FALSE;
}

void list_add_equivalency(linked_list **head, int value, int value2) {
    if (value > value2) {
        list_add_equivalency2(head, value, value2);
    }
    else {
        list_add_equivalency2(head, value2, value);
    }
}

int list_add_equivalency2(linked_list **head, int value, int value2) {
    linked_list *temp;

    if (!head) {
        return FALSE;
    }
    if (list_contains_equivalency(head, value, value2)) {
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