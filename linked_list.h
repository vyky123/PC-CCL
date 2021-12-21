#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

typedef struct thelinked_list {
    int value;
    int value2;
    struct thelinked_list *next;
} linked_list;

int list_add(linked_list **list, int value, int value2);
int list_contains(linked_list *head, int value);
#endif