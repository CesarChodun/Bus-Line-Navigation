/** @file Trie.c
 *  Trie data structure.
 *
 * @author Cezary Chodun
 */

#include "Trie.h"

#include <stdlib.h>

#include "table.h"

/// @private
struct TNode;
/// @private
typedef struct TNode{
    void *data;
    table *nodes;
}TNode;

/**
    <a href="https://it.wikipedia.org/wiki/Trie">Trie</a> data structure.
 */
typedef struct Trie{
    /// Size of the alphabet.
    int alphabet;
    /// Main node of the trie.
    TNode *nodes;
}Trie;

/// @private
static TNode *newTNode(void *data, int alphabet) {
    TNode *out = (struct TNode*) malloc(sizeof(TNode));
    if (out == NULL)
        return NULL;

    out->data = data;
    out->nodes = newTab(alphabet);
    if (out->nodes == NULL) {
        free(out);
        return NULL;
    }

    for (int i = 0; i < alphabet; i++)
        setTab(out->nodes, i, NULL);

    return out;
}

Trie *newTrie(unsigned alphabet) {
    Trie *t = (struct Trie*) malloc(sizeof(Trie));
    if (t == NULL)
        return NULL;

    t->nodes = newTNode(NULL, alphabet);
    t->alphabet = alphabet;
    if (t->nodes == NULL) {
        free(t);
        return NULL;
    }

    return t;
}

/// @private
static int translateChar(char c, int alphabet) {
    int out = c;
    if (c < 0)
        out = c + alphabet / 2;

    return out;
}

/// @private
static void destroyTNode(TNode *node, int alphabet) {
    if (node == NULL)
        return;

    for (int i = 0; i < alphabet; i++)
        destroyTNode(getTab(node->nodes, i), alphabet);

    destroyTab(node->nodes);
    free(node);
}

void destroyTrie(Trie *trie) {
    if (trie == NULL)
        return;

    destroyTNode(trie->nodes, trie->alphabet);
    free(trie);
}

/// @private
static void *addTNode(TNode *node, int alphabet, const char *s, int size, void *data) {
    TNode *next = node;
    for (int i = 0; i < size; i++) {
        if (next == NULL)
            return NULL;
        
        if (getTab(next->nodes, translateChar(s[i], alphabet)) == NULL)
            setTab(next->nodes, translateChar(s[i], alphabet), newTNode(NULL, alphabet));

        next = getTab(next->nodes, translateChar(s[i], alphabet));
    }

    next->data = data;
    return data;
}

void *addTrie(Trie *trie, const char *s, int size, void *data) {
    if (trie == NULL)
        return NULL;
    return addTNode(trie->nodes, trie->alphabet, s, size, data);
}

/// @private
static void *getTNode(TNode *node, int alphabet, const char *s, int size) {
    TNode *next = node;
    for (int i = 0; i < size; i++) {
        if (next == NULL)
            return NULL;

        int pos = translateChar(s[i], alphabet);
        next = getTab(next->nodes, pos);
    }

    if (next != NULL)
        return next->data;
    return NULL;
}

void *getTrie(Trie *trie, const char *s, int size) {
    if (trie == NULL)
        return NULL;
    return getTNode(trie->nodes, trie->alphabet, s, size);
}
