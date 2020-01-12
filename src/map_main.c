/** @file map_main.c
 *  Class containing the 'main' function.
 *
 * @author Cezary Chodun
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "MapParser.h"
#include "Text.h"
#include "map.h"

/// @private
static bool readLine(Text *s, int buffSize) {
    assert(buffSize > 1);
    
    char *buff = (char *) malloc(buffSize * sizeof(char));
    if (buff == NULL)
        return false;

    int c;
    int current = 0;
    while (true) {
        c = getc(stdin);
        buff[current++] = c;

        if (c == EOF || c == '\n')
            break;

        if (current == buffSize - 1) {
            buff[current] = '\0';
            if (appendString(s, buff) == NULL){
                free(buff);
                return false;
            }
            current = 0;
        }
    }

    buff[current] = '\0';
    if (current != 0)
        if (appendString(s, buff) == NULL){
            free(buff);
            return false;
        }

    free(buff);
    return true;
}

/**
 * @brief
 *  After invoking the function the program will start waiting for input.
 */
int main() {
    int bufSize = 64;

    Map *map = newMap();
    if (map == NULL)
        return 0;

    int lineNum = 0;
    Text *s = newText("");
    while (s != NULL) {
        bool err = false;
        lineNum++;

        if (!readLine(s, bufSize)) {
            destroyText(s);
            s = NULL;
            break;
        }

        if (err);
        else if (textSize(s) == 1 && (backText(s) == '\n' || backText(s) == EOF));//Empty line
        else if (getTextChar(s, 0) == '#');//Comment
        else if (backText(s) == EOF)
            err = true; //Wrong format
        else {
            popBackText(s);//Removes the end of a line character
            vector *args = splitText(s, ';');
            if (args == NULL) {   //Failed to allocate momory for substring
                destroyText(s);
                s = NULL;
                break;
            }

            Text *cmd = getVec(args, 0);

            unsigned tmp;
            if (equalsC(cmd, "addRoad"))
                err |= !addRoadFoo(map, args);
            else if (equalsC(cmd, "repairRoad"))
                err |= !repairRoadFoo(map, args);
            else if (equalsC(cmd, "getRouteDescription"))
                err |= !getRouteDescriptionFoo(map, args);
            else if (equalsC(cmd, "newRoute"))
                err |= !newRouteFoo(map, args);
            else if (equalsC(cmd, "extendRoute"))
                err |= !extendRouteFoo(map, args);
            else if (equalsC(cmd, "removeRoad"))
                err |= !removeRoadFoo(map, args);
            else if (equalsC(cmd, "removeRoute"))
                err |= !removeRouteFoo(map, args);
            else if (toUIntVal(cmd, &tmp))   //Exact route
                err |= !exactRouteFoo(map, args);
            else
                err = true; //Wrong command

            for (int i = 0; i < vecSize(args); i++)
                destroyText(getVec(args, i));
            destroyVec(args);
        }

        if (err)
            fprintf(stderr, "%s %d\n", "ERROR", lineNum);
        if (backText(s) == EOF)
            break;
        clearText(s);
    }

    if (s != NULL)
        destroyText(s);
    deleteMap(map);

    return 0;
}
