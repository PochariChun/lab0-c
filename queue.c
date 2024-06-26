#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include "queue.h"
#define MAX_LEN 100000
/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head) {
        return NULL;
    }
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    element_t *entry, *safe;
    list_for_each_entry_safe (entry, safe, head, list) {
        q_release_element(entry);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }

    element_t *new = malloc(sizeof(element_t));
    if (!new) {
        return false; /* q_insert_head failed */
    }

    /* Allocate memory for the string value and copy the string */
    new->value = malloc(strlen(s) + 1);
    if (!new->value) {
        free(new);    /* Release memory for new element */
        return false; /* q_insert_head failed */
    }
    strncpy(new->value, s, strlen(s) + 1); /* Copy string s to new->value */

    list_add(&new->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }

    element_t *new = malloc(sizeof(element_t));
    if (!new) {
        return false; /* q_insert_head failed */
    }

    /* Allocate memory for the string value and copy the string */
    new->value = malloc(strlen(s) + 1);
    if (!new->value) {
        free(new);    /* Release memory for new element */
        return false; /* q_insert_head failed */
    }
    strncpy(new->value, s, strlen(s) + 1); /* Copy string s to new->value */

    list_add_tail(&new->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *first_entry = list_first_entry(head, element_t, list);

    list_del_init(head->next);

    if (sp) {
        strncpy(sp, first_entry->value, bufsize);
        sp[bufsize - 1] = '\0';
    }

    return first_entry;
}
/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *last_entry = list_last_entry(head, element_t, list);

    list_del_init(head->prev);

    if (sp) {
        strncpy(sp, last_entry->value, bufsize);
        sp[bufsize - 1] = '\0';
    }

    return last_entry;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    struct list_head *fast = head->next;
    struct list_head *slow = head->next;
    while (fast != head && fast->next != head) {
        fast = fast->next->next;
        slow = slow->next;
    }

    struct list_head *mid = slow;
    slow->prev->next = slow->next->next;
    free(mid);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *safe, *current;
    list_for_each_safe (current, safe, head) {
        current->next = current->prev;
        current->prev = safe;
        if (safe == (head)) {
            safe->prev = safe->next;
            safe->next = current;
        }
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_is_singular(head))
        return;


    element_t *begin[MAX_LEN], *end[MAX_LEN], *L, *R;
    element_t pivot;
    int i = 0;

    begin[0] = list_first_entry(head, element_t, list);
    end[0] = list_last_entry(head, element_t, list);

    while (i >= 0) {
        L = begin[i];
        R = end[i];

        if (L != R && &begin[i]->list != head) {
            pivot = *begin[i];
            if (i == MAX_LEN - 1) {
                assert(-1);
                return;
            }

            while (L != R) {
                if (descend) {
                    while (R->value <= pivot.value && L != R)
                        R = list_entry(R->list.prev, element_t, list);
                } else {
                    while (R->value >= pivot.value && L != R)
                        R = list_entry(R->list.prev, element_t, list);
                }
                if (L != R) {
                    L->value = R->value;
                    L = list_entry(L->list.next, element_t, list);
                }

                if (descend) {
                    while (L->value >= pivot.value && L != R)
                        L = list_entry(L->list.next, element_t, list);
                } else {
                    while (L->value <= pivot.value && L != R)
                        L = list_entry(L->list.next, element_t, list);
                }

                if (L != R) {
                    R->value = L->value;
                    R = list_entry(R->list.prev, element_t, list);
                }
            }

            L->value = pivot.value;

            begin[i + 1] = list_entry(L->list.next, element_t, list);
            end[i + 1] = end[i];
            end[i++] = L;

        } else {
            i--;
        }
    }
}


/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
