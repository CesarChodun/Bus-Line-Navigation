/** @file vector.c
 *  Expandable array data structure.
 *
 * @author Cezary Chodun
 */

#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "table.h"


/// @private The smallest possible capacity of a vector.
static const int startCapacity = 1;

/// Dynamic array implementation
typedef struct vector{
    /// Data contained in the vector.
    struct table *data;
    /// End of the vector.
    int end;
} vector;


int vecSize(vector *vec);

/// @private
static int binarySize(int capacity) {
    int size = startCapacity;
    while (size < capacity)
        size <<= 1;
    return size;
}

vector *newVec(int capacity) {
    vector *out = (struct vector*) malloc(sizeof(vector));
    if (out == NULL)
        return NULL;
    
    out->end = 0;
    out->data = newTab(binarySize(capacity));
    if (out->data == NULL) {
        free(out);
        return NULL;
    }
    
    return out;
}

vector *copyVec(vector *vec) {
    vector *out = newVec(vecSize(vec));
    if (out == NULL)
        return NULL;
    out->end = vec->end;
    
    for (int i = 0; i < vecSize(vec); i++)
        setVec(out, i, getVec(vec, i));
    
    return out;
}

void destroyVec(vector *vec) {
    if (vec == NULL)
        return;
    
    if (vec->data != NULL)
        destroyTab(vec->data);
    
    free(vec);
}

void *reserveVec(vector *vec, int capacity) {
    if (resizeTab(vec->data, binarySize(capacity)) == NULL)
        return NULL;
    return vec;
}


void *resizeVec(vector *vec, int capacity) {
    if (reserveVec(vec, capacity) == NULL)
        return NULL;
    vec->end = capacity;
    return vec;
}

/// @private
void *balanceVec(vector *vec, int offset) {
    int size = vecSize(vec) + offset;
    if ((size > tabSize(vec->data) && offset >= 0) ||
        ((size + startCapacity)<<1 < tabSize(vec->data) && offset < 0))
        if (reserveVec(vec, size) == NULL)
            return NULL;
    return vec;
}

void *pushBackVec(vector *vec, void *val) {
    if (balanceVec(vec, 1) == NULL)
        return NULL;
    
    setTab(vec->data, vec->end, val);
    vec->end++;
    
    return vec;
}

int vecSize(vector *vec) {
    if (vec == NULL)
        return 0;
    return vec->end;
}

void *getVec(vector *vec, int x) {
    assert(x < vec->end && x >= 0);
    
    return getTab(vec->data, x);
}

void setVec(vector *vec, int x, void *val) {
    assert(x < vec->end);
    
    setTab(vec->data, x, val);
}

void *backVec(vector *vec) {
    assert(vec->end != 0);
    
    return getVec(vec, vec->end - 1);
}

void *popBackVec(vector *vec) {
    assert(vec->end != 0);
    
    void *out = backVec(vec);
    vec->end--;
    balanceVec(vec, 0);
    
    return out;
}

void *removeVec(vector *vec, int x) {
    void *out = getVec(vec, x);
    setVec(vec, x, backVec(vec));
    popBackVec(vec);
    return out;
}

void resetVec(vector *vec) {
    vec->end = 0;
}

/**
 *  Inserts the 'insert' vector between indices x and x + 1
 *  in the 'origin' vector.
 */
void insertVec(vector *origin, vector *insert, int x) {
    assert(x >= 0 || x <= vecSize(origin));
    
    int originSize = vecSize(origin);
    int insertSize = vecSize(insert);
    
    resizeVec(origin, originSize + insertSize);
    
    for (int i = originSize - 1; i >= x; i--)
        setVec(origin, i + insertSize, getVec(origin, i));
    
    for (int i = 0; i < insertSize; i++)
        setVec(origin, x + i, getVec(insert, i));
}

void reverseVec(vector *vec) {
    int size = vecSize(vec);
    
    for (int i = size - 1; i >= size - i - 1; i--) {
        void *tmp = getVec(vec, i);
        setVec(vec, i, getVec(vec, size - i - 1));
        setVec(vec, size - i - 1, tmp);
    }
}
