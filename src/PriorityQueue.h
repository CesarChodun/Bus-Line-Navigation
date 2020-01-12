/** @file PriorityQueue.h
 *  Interface for the 'PriorityQueue' data structure.
 *
 * @author Cezary Chodun
 */

#ifndef PriorityQueue_h
#define PriorityQueue_h

/**
 @brief
     Implementation of
     <a href="https://en.wikipedia.org/wiki/Heap_(data_structure)">Heap</a>
     data structure.
 */
typedef struct PriorityQueue PriorityQueue;

/**
    @brief
        Creates new priority queue.
 */
PriorityQueue *newPriorityQueue(void (*comparator)(void *a, void *b, int *ret));

/**
    @brief
        Destroys the priority queue.
 <b>NOTE: </b> queue pointer becomes invalid.
 */
void destroyPriorityQueue(PriorityQueue *queue);

/**
    @brief
        Adds an element to the queue.
 */
void *addPQueue(PriorityQueue *queue, void *val);

/**
    @brief
        Returns the smallest element in the queue.
    @return
        Smallest element from the queue.
 */
void *getPQueue(PriorityQueue *queue);

/**
    @brief
        Deletes the smallest element from the queue.
    @return
        Smallest element from the queue.
 */
void *popPQueue(PriorityQueue *queue);

/**
    @brief
        Checks whether the queue contains element 'val'.
    @return
        A pointer to the element if it is present in the queue,
        and NULL otherwise.
 */
void *containsPQueue(PriorityQueue *queue, void *val);

#endif /* PriorityQueue_h */
