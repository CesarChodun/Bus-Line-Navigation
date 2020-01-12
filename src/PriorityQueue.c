/** @file PriorityQueue.c
 *  Class implementing priority queue on the cheap.
 *
 * @author Cezary Chodun
 */

#include "PriorityQueue.h"

#include <stdlib.h>
#include <assert.h>

/// @private
struct Node;

/// @private
typedef struct Node{
    void *val;
    int size;
    
    struct Node *left;
    struct Node *right;
}Node;

/// Heap data structure.
typedef struct PriorityQueue{
    /// Comparator for the data structure.
    void *comparator;
    /// The main node of the heap.
    Node *tree;
}PriorityQueue;

PriorityQueue *newPriorityQueue(void (*comparator)(void *a, void *b, int *ret)) {
    PriorityQueue *out = (struct PriorityQueue*) malloc(sizeof(PriorityQueue));
    if (out == NULL)
        return NULL;
    
    out->comparator = comparator;
    out->tree = NULL;
    
    return out;
}

/// @private
static Node *newNode(void *val) {
    Node *node = (struct Node*) malloc(sizeof(Node));
    if (node == NULL)
        return NULL;
    
    node->size = 1;
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    
    return node;
}

/// @private
static void freeNode(Node *node) {
    if (node == NULL)
        return;
    
    freeNode(node->left);
    freeNode(node->right);
    
    free(node);
}

void destroyPriorityQueue(PriorityQueue *queue) {
    if (queue == NULL)
        return;
    
    freeNode(queue->tree);
    free(queue);
}


/// @private
static void *containsNode(Node *node, void *val, void (*comparator)(void *a, void *b, int *ret)){
    if (node == NULL)
        return NULL;
    Node *next = node;
    
    while (next->val != val){
        int cmp;
        comparator(next->val, val, &cmp);
        
        if (cmp == 0)
            break;
        else if (cmp == 1)
            next = next->left;
        else
            next = next->right;
        
        if (next == NULL)
            return NULL;
    }
    
    return next->val;
}

/// @private
static void *addNode(Node *node, void *val, void (*comparator)(void *a, void *b, int *ret)) {
    void *next = val;
    int cmp = 0;
    comparator(node->val, val, &cmp);
    if (cmp == 1) {
        next = node->val;
        node->val = val;
    }
    
    if (node->left == NULL) {
        node->left = newNode(next);
        return node->left;
    }
    if (node->right == NULL) {
        node->right = newNode(next);
        return node->right;
    }
    
    if (node->left->size > node->right->size)
        return addNode(node->right, next, comparator);
    return addNode(node->left, next, comparator);
}

/// @private
static void fixChildren(Node *node) {
    if (node->left == NULL) {
        if (node->right == NULL)
            return;
        if (node->right->val == NULL) {
            freeNode(node->right);
            node->right = NULL;
        }
    }
    else {
        if (node->left->val == NULL) {
            freeNode(node->left);
            node->left = NULL;
        }
    }
}

/// @private
static void *popValueNode(Node *node, void (*comparator)(void *a, void *b, int *ret)) {
    assert(node != NULL);
    Node *next = node->left;

    if (node->left == NULL)
        next = node->right;
    else if (node->right == NULL);
    else if (node->left->size < node->right->size)
        next = node->right;

    void *out = node->val;
    if (next == NULL)
        node->val = NULL;
    else {
        node->val = popValueNode(next, comparator);
        if (next->val == NULL)
            fixChildren(node);
    }
    
    return out;
}

void *addPQueue(PriorityQueue *queue, void *val) {
    if (queue->tree == NULL) {
        queue->tree = newNode(val);
        if (queue->tree == NULL)
            return NULL;
        return val;
    }
    
    return addNode(queue->tree, val, queue->comparator);
}

void *getPQueue(PriorityQueue *queue) {
    if (queue->tree == NULL)
        return NULL;
    
    return queue->tree->val;
}

void *popPQueue(PriorityQueue *queue) {
    if (queue->tree == NULL)
        return NULL;
    
    void *out = popValueNode(queue->tree, queue->comparator);
    if (queue->tree->val == NULL) {
        freeNode(queue->tree);
        queue->tree = NULL;
    }
    
    return out;
}

void *containsPQueue(PriorityQueue *queue, void *val) {
    if (queue->tree == NULL)
        return NULL;
    
    return containsNode(queue->tree, val, queue->comparator);
}
