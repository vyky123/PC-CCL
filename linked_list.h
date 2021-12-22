#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

typedef struct thelinked_list {
    int value;
    int value2;
    struct thelinked_list *next;
} linked_list;

int list_add(linked_list **head, int value, int value2);
int list_contains(linked_list *head, int value);
int list_contains_equivalency(linked_list **head, int value, int value2);
void list_add_equivalency(linked_list **head, int value, int value2);
int list_add_equivalency2(linked_list **head, int value, int value2); /* TODO prejmenovat */
void list_free(linked_list **head);
#endif
