/**
 * @file deque.h
 * @brief A simple doubly-linked list implementation.
 * @author Cameron Unterberger
 */

#if !defined(_DEQUE_H_)
#define _DEQUE_H_

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Node {
    struct Node *next;
    struct Node *prev;
    void *data;
} node_t;

typedef struct Deque {
    struct Node *head;
    struct Node *tail;
    ssize_t n_items;
} deque_t;

/**
 * @brief A function pointer with signature matching free()
 */
typedef void (*free_func_t)(void *);

/**
 * @brief Return a pointer to a new deque object.
 *
 * This pointer must be freed by the caller. Before freeing the deque,
 * be sure to pop all items off of the deque first. As a convenience, you can
 * accomplish that with dq_destroy()
 */
deque_t *dq_create(void);

/**
 * @brief Empties the deque of nodes, optionally calling free_func on the node
 * data.
 *
 * Free_func must match the function signature of free(). If free_func
 * is NULL, dq_destroy will not attmpt to free the data; it will only
 * discard it.
 */
void dq_destroy(deque_t *dq, free_func_t free_func);

/**
 * @brief Return true if there are no items (nodes) in the deque
 */
bool dq_is_empty(deque_t *dq);

/**
 * @brief Return the number of items in the deque.
 *
 * @returns Number of items in the deque, @c -1 on error.
 */
ssize_t dq_len(deque_t *dq);

/* Pushes an item of data onto the head of the deque. Returns a pointer to the
 * node that was pushed onto the deque (i.e. the new head), or NULL on error. */
node_t *dq_push(deque_t *dq, void *data);

/* Pops a node off the head of the deque, and returns the contained data, or
 * NULL if empty. */
void *dq_pop(deque_t *dq);

/* Appends an item of data onto the tail of the deque. Returns a pointer to the
 * node that was appended onto the deque (i.e. the new tail), or NULL on error.
 */
node_t *dq_append(deque_t *dq, void *data);

/* Dequeues a node off the tail of the deque, and returns the contained data, or
 * NULL if empty. */
void *dq_dequeue(deque_t *dq);

/* Joins A and B. Returns pointer to A on success, NULL on failure, with
 * errno set to EINVAL (i.e. A or B is NULL). Items from B are appended to the
 * end of A (in order). B will be empty after calling. Result after execution: A
 * -> A + B; B -> <empty>. NOTE: this is faster than manually popping and
 * appending. */
deque_t *dq_join(deque_t *a, deque_t *b);

/* Returns a new deque object with the same data as orig in each element. This
 * is a shallow copy (i.e. it does not make deep copies of the data) */
deque_t *dq_copy(deque_t *orig);

/* Returns a new list containing the elemints of orig, sorted in ascending order
 */
deque_t *dq_sorted(deque_t *orig);

/* Returns a new list that is the result of merging two sorted lists, left and
 * right, in ascending sorted order. The resulting list will be sorted as long
 * as both left and right are sorted in ascending order */
deque_t *dq_merge(deque_t *left, deque_t *right);

#endif /* _DEQUE_H_ */
