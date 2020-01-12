/** @file vector.h
 *  Interface for the 'Vector' data structure.
 *
 * @author Cezary Chodun
 */

#ifndef vector_h
#define vector_h

/// @private
typedef struct vector vector;

/**
    @brief
        Creates a new vector.
 */
vector *newVec(int capacity);

/**
    @brief
        Frees the vector.
 */
void destroyVec(vector *vec);

/**
    @brief
        Creates new vector and copies
        the data from a 'vec' to it.
    @return
        Pointer to the created vector.
 */
vector *copyVec(vector *vec);

/**
    @brief
        Reserves additional memory for
        the vector.
 */
void *reserveVec(vector *vec, int capacity);

/**
    @brief
        Resizes the vector.
    <b>NOTE: </b> the data outside of the
    capacity range is discarded.
    And the value of elements outside
    of the old size range is undefined.
 */
void *resizeVec(vector *vec, int capacity);

/**
    @brief
        Inserts the 'val' element at
        the end of a vector(index = size).
 */
void *pushBackVec(vector *vec, void *val);

/**
    @brief
        Inserts the elements from vector
        'insert' to the 'origin' vector.
        At point 'x'.
 */
void insertVec(vector *origin, vector *insert, int x);

/**
    @brief
        Returns the vector size.
    @return
        The size of the vector.
 */
int vecSize(vector *vec);

/**
    @brief
        Returns the x-th element in the vector.
 */
void *getVec(vector *vec, int x);

/**
    @brief
        Sets the value of the x-th element.
 */
void setVec(vector *vec, int x, void *val);

/**
    @brief
        Returns the value of the last element.
    @return
        The value of the last element.
 */
void *backVec(vector *vec);

/**
    @brief
        Deletes the last element.
    @return
        The value of the last element.
 */
void *popBackVec(vector *vec);

/**
    @brief
        Discardes all data contained in
        the vector, but preserves its
        capacity.
 */
void resetVec(vector *vec);

/**
    @brief
        Reverses the vector.
 */
void reverseVec(vector *vec);

/**
    @brief
        Removes the x-th element in the vector.
    <b>NOTE: </b> The order in which the elements
        are stored in the vector may be changed.
 */
void *removeVec(vector *vec, int x);

#endif /* vector_h */
