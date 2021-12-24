#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

typedef struct thelinked_list {
    int value;
    int value2;
    struct thelinked_list *next;
} linked_list;

void list_add_at_start(linked_list **start_ref, int data);
int list_add(linked_list **head, int value, int value2);
int list_contains(linked_list *head, int value);
void list_free(linked_list **head);
int add_equivalency(linked_list **list, int value, int value2);
int update_equivalencies(linked_list *list, int value, int value2);

#endif
