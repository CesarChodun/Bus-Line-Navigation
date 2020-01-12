/** @file Text.h
 *  Interface for the 'Text' class.
 *
 * @author Cezary Chodun
 */

#ifndef Text_h
#define Text_h

#include <stdbool.h>

#include "vector.h"

/// @private
typedef struct Text Text;

/**
    @brief
        Creates a new Text.
    @return
        A pointer to the Text.
 */
Text *newText(char *s);

/**
    @brief
        Frees the Text and its data.
 */
void destroyText(Text *s);

/**
    @brief
        Returns the x-th character in the Text.
    @return
        x-th character
 */
char getTextChar(Text *s, int x);

/**
    @brief
        Returns substring of the Text('s').
        The substring contains all characters
        with indices in the range <start, end).
 <b>Note:</b> '<','>' - inclusive; '('')' - exclusive
    @return
        The substring.
 */
Text *getSubstring(Text *s, int start, int end);

/**
    @brief
        Splits the Text into subTexts,
        so that none of them contains the
        'c' character.
        Eg:
 split("abcadba#", 'a') => {"", "bc", "db", "#"}
    @return
        SubTexts
 */
vector *splitText(Text *s, char c);

/**
    @brief
        Checks whether two Text are equal.
 */
bool equals(Text *a, Text *b);

/**
 @brief
 Checks whether two Text are equal.
 */
bool equalsC(Text *a, const char *b);

/**
    @brief
        Checks wheter the 's' Text starts
        with 't' Text.
    @return
        True if 's' starts with 't' and
        false otherwise
 */
bool startsWith(Text *s, Text *t);

/**
    @brief
        Checks wheter the 's' Text starts
        with 't' Text.
    @return
        True if 's' starts with 't' and
        false otherwise
 */
bool startsWithC(Text *s, const char *t);

/**
 @brief
 Converts Text to integer value.
 
 @param[in] s    - text to be converted to int
 @param[out] val - value from the Text
 
 @return
 @p true if conversion was successful, and
 @p false otherwise.
 */
bool toIntVal(Text *s, int *val);

/**
    @brief
        Converts Text to unsigned integer value.
 
    @param[in] s    - text to be converted to unsigned int
    @param[out] val - value from the Text
 
    @return
        @p true if conversion was successful, and
        @p false otherwise.
 */
bool toUIntVal(Text *s, unsigned *val);

/**
    @brief
        Returns size of the Text.
    @return
        Size of the Text.
*/
int textSize(Text *s);

/**
    @brief
        Returns the value of the last character
        in the Text.
    @return
        Value of the last character
 */
char backText(Text *s);

/**
    @brief
        Removes the last character from the Text.
    @return
        's' if the operation was succesfull and
        NULL otherwise
 */
void *popBackText(Text *s);

/**
    @brief
        Clears the Text data.
 */
void clearText(Text *s);

/**
    @brief
        Appends a character to the Text.
    @return
        's' if the operation was successful
        and NULL otherwise
 */
void *appendChar(Text *s, char c);
/**
    @brief
        Appends the 'c' string to the end of 's'.
    @return
        's' if the operation was successful
        and NULL otherwise
 */
void *appendString(Text *s, const char *c);

/**
    @brief
        Converts the 'val' to a Text and
        appends it to the end of 's'.
    @return
        's' if the operation was successful
        and NULL otherwise
 */
void *appendUInt(Text *s, unsigned val);

/**
    @brief
        Converts the 'val' to a Text and
        appends it to the end of 's'.
    @return
        's' if the operation was successful
        and NULL otherwise
 */
void *appendInt(Text *s, int val);

/**
    @brief
        Creates a C style string from 's'
     @return
        A C style string that contains the
        characters stored in 's'(Text)
        if the operation was successful
        and NULL otherwise
 */
char *to_cString(Text *s);

/**
    @brief
        Returns the C style string size.
    @return
        The C style string size
 */
int cStringSize(const char *s);

#endif /* Text_h */
