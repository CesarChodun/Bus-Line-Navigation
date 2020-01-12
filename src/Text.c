/** @file map_main.c
 *  Naive implementation of Java String.
 *
 * @author Cezary Chodun
 */

#include "Text.h"

#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <math.h>

/// @private
static const int SMALL_SIZE = 10;
/// @private
static const int EMPTY_SIZE = 2;
/// @private
const char EOS = '\0';

///Data structure for data operations.
typedef struct Text{
    /// Array of characters.
    char *data;
    /// Size of the used part of the array.
    int size;
    /// Array size.
    int capacity;
}Text;

Text *newText(char *s) {
    Text *out = (Text*) malloc(sizeof(Text));
    if (out == NULL)
        return NULL;

    int csize = 1 + cStringSize(s);
    out->data = (char*) malloc(csize * sizeof(char));
    if (out->data == NULL) {
        free(out);
        return NULL;
    }

    out->size = 0;
    out->capacity = 1;
    
    if (appendString(out, s) == NULL) {
        destroyText(out);
        return NULL;
    }

    return out;
}

void destroyText(Text *s) {
    if (s == NULL)
      return;

    free(s->data);
    free(s);
}

int cStringSize(const char *c) {
    int size = 0;
    while (*c != '\0') {
        size++;
        c++;
    }

    return size;
}

/// @private
static void *fixSize(Text *s, int extra) {
    if (s->size + extra > s->capacity);  //Not enough space
    else if (s->capacity > SMALL_SIZE &&
            s->size + extra < s->capacity / 2 - 2);    //Space is beeing wasted
    else
        return s;//no changes required

    int size = s->size + extra;
    
    size *= 3;
    size /= 2;
    size += EMPTY_SIZE;
    
    char *tmp = (char *) realloc(s->data, size * sizeof(char));
    if (tmp == NULL)
        return NULL;
    
    s->data = tmp;
    s->capacity = size;
    return s;
}

char getTextChar(Text *s, int x) {
    assert(x >= 0 && x < s->size);
    return s->data[x];
}

int textSize(Text *s) {
    return s->size;
}

char backText(Text *s) {
    assert(s->size > 0);
    return s->data[s->size - 1];
}

Text *getSubstring(Text *s, int start, int end) {
    assert(start <= end);
    
    Text *out = newText("");
    for (int i = start; i < end; i++)
        if (appendChar(out, s->data[i]) == NULL) {
            destroyText(out);
            return NULL;
        }
    return out;
}

vector *splitText(Text *s, char c) {
    vector *out = newVec(2);
    
    int last = 0;
    for (int i = 0; i < s->size; i++)
        if (s->data[i] == c) {
            Text *sub = getSubstring(s, last, i);
            if (sub == NULL) {    //Failed to create subText
                for (int j = 0; j < vecSize(out); j++)
                    destroyText(getVec(out, j));
                destroyVec(out);
                return NULL;
            }
            
            pushBackVec(out, sub);
            last = i + 1;
        }
    pushBackVec(out, getSubstring(s, last, textSize(s)));
    
    return out;
}

/// @private
static bool comparePart(const char *a, const char *b, int length) {
    for (int i = 0; i < length; i++)
        if (a[i] != b[i])
            return false;
    return true;
}

bool equals(Text *a, Text *b) {
    if (a->size != b->size)
        return false;
    
    return comparePart(a->data, b->data, a->size);
}

bool equalsC(Text *a, const char *b) {
    int bsize = cStringSize(b);
    if (a->size != bsize)
        return false;
    
    return comparePart(a->data, b, a->size);
}

bool startsWith(Text *s, Text *t) {
    if (s->size < t->size)
        return false;
    return comparePart(s->data, t->data, t->size);
}

bool startsWithC(Text *s, const char *t) {
    int tsize = cStringSize(t);
    if (s->size < tsize)
        return false;
    return comparePart(s->data, t, tsize);
}

void *popBackText(Text *s) {
    assert(s->size > 0);
    if (fixSize(s, -1) == NULL)
        return NULL;
    s->size--;
    return s;
}

void clearText(Text *s) {
    fixSize(s, -s->size);
    s->size = 0;
}

void *appendChar(Text *s, char c) {
    if (fixSize(s, 1) == NULL)
        return NULL;
    s->data[s->size++] = c;
    return s;
}

void *appendString(Text *s, const char *c) {
    if (s == NULL)
        return NULL;

    int ssize = cStringSize(c);
    if (ssize == 0)
        return s;
    if (fixSize(s, ssize) == NULL)
        return NULL;

    for (int i = 0; i < ssize; i++)
        s->data[s->size + i] = c[i];
    s->size += ssize;

    return s;
}

void *appendUInt(Text *s, unsigned val) {
    if (s == NULL)
        return NULL;

    int tab[20], size = 0;
    for (size = 0; val != 0; size++) {
        tab[size] = val%10;
        val /= 10;
    }

    if (fixSize(s, size) == NULL)
        return NULL;

    for (int i = 0; i < size; i++)
        s->data[s->size + i] = '0' + tab[size - i - 1];
    s->size += size;

    return s;
}

void *appendInt(Text *s, int val) {
    if (s == NULL)
        return NULL;

    if (val < 0) {
        if (appendString(s, "-") == NULL)
            return NULL;
        val = -val;
    }

    return appendUInt(s, (unsigned) val);
}

static bool readUIntVal(Text *s, unsigned *val, int start) {
    if (textSize(s) == 0)
        return false;
    
    long out = 0;
    
    for (int i = start; i < textSize(s) && out <= UINT_MAX; i++) {
        char c = getTextChar(s, i);
        
        if (c < '0' || c > '9')
            return false;
        out *= 10;
        out += c - '0';
    }
    
    if (out > UINT_MAX)
        return false;
    
    val[0] = (int)out;
    return true;
}

bool toIntVal(Text *s, int *val) {
    if (textSize(s) == 0)
        return false;
    
    bool negative = (getTextChar(s, 0) == '-');
    unsigned tmp = 0;
    
    if (!readUIntVal(s, &tmp, negative? 1 : 0))
        return false;
    
    long out = tmp;
    if (negative)
        out *= -1;
    
    if (out <= INT_MAX && out >= INT_MIN)
        val[0] = (int)out;
    else
        return false;
    
    return true;
}

bool toUIntVal(Text *s, unsigned *val) {
    return readUIntVal(s, val, 0);
}

char *to_cString(Text *s) {
    char *out = (char *) malloc((s->size + 1) * sizeof(char));
    if (out == NULL)
        return NULL;

    for (int i = 0; i < s->size; i++)
        out[i] = s->data[i];
    out[s->size] = EOS;

    return out;
}
