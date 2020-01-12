/** @file table.c
 *  Class containing the 'main' function.
 *
 * @author Cezary Chodun
 */

#include "table.h"

#include <stdlib.h>
#include <assert.h>

/// Array implementation.
typedef struct table{
    /// Data contained in the array.
    void **data;
    /// Array size.
    int size;
} table;

table *newTab(int capacity) {
    table *out = (struct table*) malloc(sizeof(table));
    if (out == NULL)
        return NULL;
    
    out->size = capacity;
    out->data = (void**)malloc(capacity * sizeof(void*));
    if (out->data == NULL) {
        free(out);
        return NULL;
    }
    
    return out;
}

void destroyTab(table *tab) {
    if (tab == NULL)
        return;
    if (tab->data != NULL)
        free(tab->data);
    free(tab);
}

void *resizeTab(table *tab, int capacity) {
    void **tmp = realloc(tab->data, capacity * sizeof(void*));
    if (tmp == NULL)
        return NULL;//Failed to allocate memory
    
    tab->data = tmp;
    tab->size = capacity;
    return tab;
}

void *getTab(table *tab, int x) {
    assert(tab != NULL);
    assert(tab->data != NULL);
    assert(x >= 0 && x < tab->size);
    return tab->data[x];
}

void setTab(table *tab, int x, void *val) {
    assert(x < tab->size);
    tab->data[x] = val;
}

int tabSize(table *tab) {
    return tab->size;
}
