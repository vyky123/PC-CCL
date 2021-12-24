#include "ccl.h"

int add_equivalency(linked_list **table, int value, int value2) {
    if (update_equivalencies(*table, value, value2)) {
        return TRUE;
    }

    linked_list *temp = malloc(sizeof(linked_list));
    if (!temp) {
        return FALSE;
    }

    temp->value = value;
    temp->value2 = value2;
    temp->next = *table;
    *table = temp;

    printf("New equivalency: %d -> %d \n", temp->value, temp->value2);
    return TRUE;
}

int update_equivalencies(linked_list *head, int value, int value2) {
    linked_list *it, *it2;
    if (!head) {
        return FALSE;
    }

    it = head;
    //   printf("T1: %d %d\n", it->value, it->value2);
    while(it) {
        if(it->value == value) {
            if (it->value2 == value2) {
                return TRUE;
            }
            if (it->value2 != value2 && value2 < it->value2) {
                it->value2 = value2;

                it2 = head;
//                printf("T1: %d %d; T2: %d %d (%d %d)\n", it->value, it->value2, it2->value, it2->value2, (*head)->value, (*head)->value2);
                while (it2) {
                    if (it2->value2 == it->value2 && value2 < it2->value2) {
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