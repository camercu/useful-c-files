/**
 * @file deque.c
 * @brief A simple doubly-linked list implementation.
 * @author Cameron Unterberger
 */

#include "deque.h"

#include <assert.h>
#include <errno.h>

/* Return a pointer to a new deque object. This pointer must be freed by the
 * caller. Before freeing the deque, be sure to pop all items off of the
 * deque
 * first. As a convenience, you can accomplish that with dq_destroy() */
deque_t *dq_create(void) {
    return calloc(1, sizeof(deque_t));
}

/* Empties the deque of nodes, optionally calling free_func on the node
 * data. Free_func must match the function signature of free(). If free_func
 * is NULL, dq_destroy will not attmpt to free the data; it will only
 * discard it. */
void dq_destroy(deque_t *dq, freefunc_t free_func) {
    void *data;
    if (!dq) {
        return;
    }
    while (!dq_is_empty(dq)) {
        data = dq_pop(dq);
        if (free_func) {
            free_func(data);
        }
    }
    free(dq);
}

/* return true if there are no items (nodes) in the deque */
bool dq_is_empty(deque_t *dq) {
    bool res;
    if (!dq) {
        errno = EINVAL;
        return true;
    }
    res = !dq->head;
    assert(res == !dq->n_items);
    return !dq->head;
}

/* Pushes an item of data onto the head of the deque. Returns a pointer to the
 * node that was pushed onto the deque (i.e. the new head), or NULL on error. */
node_t *dq_push(deque_t *dq, void *data) {
    node_t *new;
    if (!dq) {
        errno = EINVAL;
        return NULL;
    }

    new = calloc(1, sizeof(*new));
    if (!new) {
        errno = ENOMEM;
        return NULL;
    }

    new->data = data;
    new->next = dq->head;
    if (dq->head) {
        dq->head->prev = new;
    }
    dq->head = new;
    if (!dq->tail) {
        dq->tail = new;
    }
    dq->n_items++;
    return new;
}

/* Pops a node off the head of the deque, and returns the contained data, or
 * NULL if empty. */
void *dq_pop(deque_t *dq) {
    void *ret = NULL;
    node_t *tmp = NULL;
    if (!dq) {
        errno = EINVAL;
        return NULL;
    }

    if (!dq_is_empty(dq)) {
        tmp = dq->head;
        ret = tmp->data;
        dq->head = dq->head->next;
        free(tmp);
        if (dq->head) {
            dq->head->prev = NULL;
        } else {
            dq->tail = NULL;
        }
        dq->n_items--;
    }
    return ret;
}

/* Appends an item of data onto the tail of the deque. Returns a pointer to the
 * node that was appended onto the deque (i.e. the new tail), or NULL on error.
 */
node_t *dq_append(deque_t *dq, void *data) {
    node_t *new;
    if (!dq) {
        errno = EINVAL;
        return NULL;
    }

    new = calloc(1, sizeof(*new));
    if (!new) {
        errno = ENOMEM;
        return NULL;
    }

    new->data = data;
    new->prev = dq->tail;
    if (dq->tail)
        dq->tail->next = new;
    dq->tail = new;
    if (!dq->head) {
        dq->head = new;
    }
    dq->n_items++;
    return new;
}

/* Dequeues a node off the tail of the deque, and returns the contained data, or
 * NULL if empty. */
void *dq_dequeue(deque_t *dq) {
    void *ret = NULL;
    node_t *tmp = NULL;
    if (!dq) {
        errno = EINVAL;
        return NULL;
    }

    if (!dq_is_empty(dq)) {
        tmp = dq->tail;
        ret = tmp->data;
        dq->tail = dq->tail->prev;
        free(tmp);
        if (dq->tail) {
            dq->tail->next = NULL;
        } else {
            dq->head = NULL;
        }
        dq->n_items--;
    }
    return ret;
}


/* Return the number of items in the deque. Returns (size_t)-1 on error */
ssize_t dq_len(deque_t *dq) {
    if (!dq)
        return (size_t)-1;
    return dq->n_items;
}


/* Joins A and B. Returns pointer to A on success, NULL on failure, with
 * errno set to EINVAL (i.e. A or B is NULL). Items from B are appended to the
 * end of A (in order). B will be empty after calling. Result after execution: A
 * -> A + B; B -> <empty>. NOTE: this is faster than manually popping and
 * appending. */
deque_t *dq_join(deque_t *a, deque_t *b) {
    if (!a || !b) {
        errno = EINVAL;
        return NULL;
    }
    if (dq_is_empty(a)) {
        a->head = b->head;
        a->tail = b->tail;
    } else {
        a->tail->next = b->head;
        if (b->head) {
            b->head->prev = a->tail;
        }
        a->tail = b->tail;
    }
    a->n_items += b->n_items;
    b->head = NULL;
    b->tail = NULL;
    b->n_items = 0;
    return a;
}

/* Returns a new deque object with the same data as orig in each element. This
 * is a shallow copy (i.e. it does not make deep copies of the data) */
deque_t *dq_copy(deque_t *orig) {
    deque_t *new;
    node_t *p;
    ssize_t i;
    if (!orig) {
        errno = EINVAL;
        return NULL;
    }
    new = dq_create();
    if (!new) {
        errno = ENOMEM;
        return NULL;
    }
    p = orig->head;
    for (i = 0; i < orig->n_items; i++) {
        if (!dq_append(new, p->data)) {
            errno = ENOMEM;
            goto error;
        }
        p = p->next;
    }
    return new;
error:
    dq_destroy(new, NULL);
    return NULL;
}

/* Returns a new list that is the result of merging two sorted lists, left and
 * right, in ascending sorted order. The resulting list will be sorted as long
 * as both left and right are sorted in ascending order */
deque_t *dq_merge(deque_t *left, deque_t *right) {
    deque_t *result;
    if (!left || !right) {
        errno = EINVAL;
        return NULL;
    }

    result = dq_create();
    if (!result) {
        errno = ENOMEM;
        return NULL;
    }

    while (!dq_is_empty(left) && !dq_is_empty(right)) {
        if (left->head->data <= right->head->data) {
            dq_append(result, dq_pop(left));
        } else {
            dq_append(result, dq_pop(right));
        }
    }

    /* consume any remaining items, if any */
    if (!dq_is_empty(left)) {
        dq_join(result, left);
    }
    if (!dq_is_empty(right)) {
        dq_join(result, right);
    }
    return result;
}

/* Returns a new list containing the elemints of orig, sorted in ascending order
 */
deque_t *dq_sorted(deque_t *orig) {
    deque_t *left, *right, *dq, *result;
    size_t split, i = 0;

    if (!orig)
        return NULL;
    if (orig->n_items < 2)
        return orig; /* base case: sorted by definition */
    left = dq_create();
    right = dq_create();
    dq = dq_copy(orig);
    if (!left || !right || !dq) {
        dq_destroy(left, NULL);
        dq_destroy(right, NULL);
        dq_destroy(dq, NULL);
        errno = ENOMEM;
        return NULL;
    }

    split = dq->n_items / 2;
    while (!dq_is_empty(dq)) {
        if (i < split)
            dq_push(left, dq_pop(dq));
        else
            dq_push(right, dq_pop(dq));
        i++;
    }
    left = dq_sorted(left);
    right = dq_sorted(right);
    result = dq_merge(left, right);
    dq_destroy(dq, NULL);    /* empty, so don't need free_func */
    dq_destroy(left, NULL);  /* empty, so don't need free_func */
    dq_destroy(right, NULL); /* empty, so don't need free_func */
    return result;
}
