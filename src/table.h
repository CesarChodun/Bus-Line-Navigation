/** @file table.h
 *  Interface for the 'Table' data structure.
 *
 * @author Cezary Chodun
 */

#ifndef table_h
#define table_h

/// @private
typedef struct table table;

/**
    @brief
        Creates a new table.
    @return
        A pointer to the table.
 */
table *newTab(int capacity);

/**
    @brief
        Frees the table.
 <b>NOTE: </b> the pointer 'tab' becomes invalid.
 */
void destroyTab(table *tab);


/**
    @brief
        Changes size of the table.
    <ul>
        <li>If the capacity is smaller then the
        current size: Then elements, with
        indices bigger or equal to the capacity,
        become unaccessible.</li>
 
        <li>If the capacity is higher then current
        size of the table: Then the value,
        of elements with indices greater or
        equal to the old size, is undefined.</li>
    </ul>
 */
void *resizeTab(table *tab, int capacity);

/**
     @brief
        Returns the table size.
     @return
        Table size.
 */
int tabSize(table *tab);

/**
    @brief
        Returns the value of the x-th element.
    @return
        The x-th element.
 */
void *getTab(table *tab, int x);

/**
    @brief
        Sets the value of the x-th element.
 */
void setTab(table *tab, int pos, void *val);

#endif /* table_h */
