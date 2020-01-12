/** @file Trie.h
 *  Interface for the 'Trie' data structure.
 *
 * @author Cezary Chodun
 */

#ifndef Trie_h
#define Trie_h

/// @private
typedef struct Trie Trie;

/**
    @brief
        Creates a new trie.
    @return
        A pointer to the trie.
 */
Trie *newTrie(unsigned alphabet);

/**
    @brief
        Destroys the trie.
 */
void destroyTrie(Trie *trie);

/**
    @brief
        Adds the element(*data) to the trie.
 <b>NOTE: </b> the data cannot be a NULL pointer.
 */
//assert (data != NULL)
void *addTrie(Trie *node, const char *s, int size, void *data);

/**
    @brief
        Returns the data if the element was found or NULL otherwise
 */
void *getTrie(Trie *node, const char *s, int size);

#endif /* Trie_h */
