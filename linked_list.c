#include "ccl.h"

void list_sort(linked_list **head) {
    int swapped;
    linked_list *temp = NULL, *temp2 = NULL;

    do {
        swapped = FALSE;
        temp = *head;
        while (temp->next != temp2) {
            /* Swaps "nodes" of linked list by value so that they are in descending order */
            if (temp->value < temp->next->value) {
                swap(temp, temp->next);
                swapped = TRUE;
            }
            temp = temp->next;
        }
        temp2 = temp;
    }
    while (swapped);
}

void swap(linked_list *first, linked_list *second) {
    int temp = first->value;

    first->value = second->value;
    second->value = temp;
    temp = first->value2;
    first->value2 = second->value2;
    second->value2 = temp;
}

int update_equivalencies(linked_list **head, int value, int value2) {
    linked_list *it;
    int temp;
    /* Returns if value and value2 are equal, which would add redundant equivalencies */
    if (value == value2) {
        return TRUE;
    }
    /* Swaps value and value2 to ensure value is always smaller than value2 */
    if (value < value2) {
        temp = value;
        value = value2;
        value2 = temp;
    }

    it = *head;
    /* Goes through the linked list and tries to find equal value to the new value */
    while (it) {
        /* The first value is in the list */
        if (it->value == value) {
            /* Both values are already present in the list */
            if (it->value2 == value2) {
                return TRUE;
            }
            /* The second value of equivalency list is different and might need to be updated */
            else {
                /* Recursively update all equivalencies (which also adds new ones if needed),
                 * if 0 (FALSE) is returned, return */
                if (!update_equivalencies(head, value2, it->value2)) {
                    return TRUE;
                }
                /* Updates the value */
                it->value2 = value2;
                return TRUE;
            }

        }
        it = it->next;
    }
    list_add(head, value, value2);
    return FALSE;
}

int list_contains(linked_list *head, int value) {
    linked_list *it;

    if (!head) {
        return FALSE;
    }

    it = head;
    while (it) {
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

void list_add(linked_list **head, int value, int value2) {
    linked_list *temp;

    if (!head) {
        return;
    }

    temp = (linked_list*) malloc(sizeof(linked_list));
    if (!temp) {
        printf("ERR#3: Memory allocation was unsuccesful!\n");
        exit(ERR_3);
    }

    temp->value = value;
    temp->value2 = value2;
    temp->next = *head;
    *head = temp;

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
    next = NULL;
    *head = NULL;
}