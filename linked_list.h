#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__
/*
 * Struct representing linked list with two values. Used for an equivalency
 * list and for list of colors.
 */
typedef struct thelinked_list {
    int value;
    int value2;
    struct thelinked_list *next;
} linked_list;
/* ____________________________________________________________________________

    int list_sort(linked_list **head)

    Sorts linked list by value1 with a bubble sort.
    ____________________________________________________________________________
*/
void list_sort(linked_list **head);
/* ____________________________________________________________________________

    void swap(linked_list *first, linked_list *second)

    Swaps two values of a linked list. Used in a buble sort for linked list.
    ____________________________________________________________________________
*/
void swap(linked_list *first, linked_list *second);
/* ____________________________________________________________________________

    void list_add(linked_list **head, int value, int value2)

    Add new node to the list as a new head.
    ____________________________________________________________________________
*/
void list_add(linked_list **head, int value, int value2);
/* ____________________________________________________________________________

    int list_contains(linked_list *head, int value)

    Checks where value is already present in linked list. Used for adding
    colors to color list.
    Returns 1 (TRUE), if list already contains the value or 0 (FALSE) if the
    value is not in the list.
    ____________________________________________________________________________
*/
int list_contains(linked_list *head, int value);
/* ____________________________________________________________________________

    void list_free(linked_list **head)

    Frees all memory allocated for the list.
    ____________________________________________________________________________
*/
void list_free(linked_list **head);
/* ____________________________________________________________________________

    int update_equivalencies(linked_list **equivalency_list, int value,
                            int value2)

    Checks equivalency list and rewrites equivalencies when needed. Runs
    recursively for all equivalencies and merges them together that way.
    Returns 0 (FALSE) if no further equivalencies need to be updated.
    Return 1 (TRUE) if equivalency already present or value and value2 are
    equal.
    ____________________________________________________________________________
*/
int update_equivalencies(linked_list **equivalency_list, int value, int value2);

#endif
