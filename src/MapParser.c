/** @file MapParser.c
 *  Class which helps parse data to the Map.
 *
 * @author Cezary Chodun
 */

#include "MapParser.h"

#include <stdlib.h>

#include "Text.h"
#include "map.h"

// Probowalem inaczej ale define ciagle pokazywal sie w dokumentacji
/// @cond
#define FREE(a) if (a != NULL) free(a);
/// @endcond

//NOTE: args[0] = the name of the command(or route id)
//      args[1...] = the arguments
bool addRoadFoo(Map *map, vector *args) {
    if (vecSize(args) != 5)
        return false;   //Wrong amount of parameters
    
    bool err = false;
    
    char *city1 = to_cString(getVec(args, 1));
    char *city2 = to_cString(getVec(args, 2));
    unsigned length;
    err |= !toUIntVal(getVec(args, 3), &length);
    int builtYear;
    err |= !toIntVal(getVec(args, 4), &builtYear);
    
    if (err || city1 == NULL || city2 == NULL)
        err = true;
    else
        err |= !addRoad(map, city1, city2, length, builtYear);
    
    FREE(city1);
    FREE(city2);
    
    return !err;
}

bool repairRoadFoo(Map *map, vector *args) {
    if (vecSize(args) != 4)
        return false;   //Wrong amount of parameters
    
    bool err = false;
    
    char *city1 = to_cString(getVec(args, 1));
    char *city2 = to_cString(getVec(args, 2));
    int repairYear;
    err |= !toIntVal(getVec(args, 3), &repairYear);
    
    if (err || city1 == NULL || city2 == NULL)
        err = true;
    else
        err |= !repairRoad(map, city1, city2, repairYear);
    
    FREE(city1);
    FREE(city2);
    
    return !err;
}

bool removeRoadFoo(Map *map, vector *args) {
    if (vecSize(args) != 3)
        return false; // Wrong number of arguments
    
    char *city1 = to_cString(getVec(args, 1));
    char *city2 = to_cString(getVec(args, 2));
    
    bool err = false;
    
    if (city1 == NULL || city2 == NULL)
        err |= true;
    else
        err |= !removeRoad(map, city1, city2);
    
    FREE(city1);
    FREE(city2);
    
    return !err;
}

bool newRouteFoo(Map *map, vector *args) {
    if (vecSize(args) != 4)
        return false; // Wrong number of arguments
    
    int err = false;
    
    unsigned num;
    err |= !toUIntVal(getVec(args, 1), &num);
    char *city1 = to_cString(getVec(args, 2));
    char *city2 = to_cString(getVec(args, 3));
    
    if (err || city1 == NULL || city2 == NULL)
        err |= true;
    else
        err |= !newRoute(map, num, city1, city2);
    
    FREE(city1);
    FREE(city2);
    
    return !err;
}

bool extendRouteFoo(Map *map, vector *args) {
    if (vecSize(args) != 3)
        return false; // Wrong number of arguments
    
    int err = false;
    
    unsigned num;
    err |= !toUIntVal(getVec(args, 1), &num);
    char *city = to_cString(getVec(args, 2));
    
    if (err || city == NULL)
        err |= true;
    else
        err |= !extendRoute(map, num, city);
    
    FREE(city);
    
    return !err;
}

bool exactRouteFoo(Map *map, vector *args) {
    if (vecSize(args) < 5 || vecSize(args)%3 != 2)
        return false;   //Wrong amount of parameters
    
    bool err = false;
    
    unsigned routeNumber;
    err |= !toUIntVal(getVec(args, 0), &routeNumber);
    
    vector *cityNames = newVec(vecSize(args)/3 + 1);
    vector *roadLengths = newVec(vecSize(args)/3);
    vector *roadBuiltYears = newVec(vecSize(args)/3);
    
    if (cityNames == NULL || roadLengths == NULL || roadBuiltYears == NULL)
        err = true;
    
    for (int i = 1; i < vecSize(args); i++) {
        if (err)
            break;
        
        if (i%3 == 1)    //City name
            pushBackVec(cityNames, to_cString(getVec(args, i)));
        else if (i%3 == 2) { //Road length
            unsigned *length = (unsigned*) malloc(sizeof(unsigned));
            if (length == NULL) {
                err = true;
                break;
            }
            err |= !toUIntVal(getVec(args, i), length);
            
            pushBackVec(roadLengths, length);
        }
        else {  //i%3 == 0 => road built year
            int *year = (int*) malloc(sizeof(int));
            if (year == NULL) {
                err = true;
                break;
            }
            err |= !toIntVal(getVec(args, i), year);
            
            pushBackVec(roadBuiltYears, year);
        }
    }
    
    if (!err)
        err |= !exactRoute(map, routeNumber, cityNames, roadLengths, roadBuiltYears);
    
    for (int i = 0; i < vecSize(cityNames); i++)
        FREE(getVec(cityNames, i));
    for (int i = 0; i < vecSize(roadLengths); i++)
        FREE(getVec(roadLengths, i));
    for (int i = 0; i < vecSize(roadBuiltYears); i++)
        FREE(getVec(roadBuiltYears, i));
    
    destroyVec(cityNames);
    destroyVec(roadLengths);
    destroyVec(roadBuiltYears);
    
    return !err;
}

bool removeRouteFoo(Map *map, vector *args) {
    if (vecSize(args) != 2)
        return false; // Wrong number of parameters
    
    bool err = false;
    
    unsigned num;
    err |= !toUIntVal(getVec(args, 1), &num);
    
    if (!err)
        err |= !removeRoute(map, num);
    
    return !err;
}

bool getRouteDescriptionFoo(Map *map, vector *args) {
    if (vecSize(args) != 2)
        return false;   //Wrong amount of parameters
    
    bool err = false;
    
    unsigned routeID;
    err |= !toUIntVal(getVec(args, 1), &routeID);
    
    const char *out = NULL;
    if (!err)
        out = getRouteDescription(map, routeID);
    
    if (err || out == NULL)
        err = true;
    else
        fprintf(stdout, "%s\n", out);
    
    FREE((void*) out);
    
    return !err;
}
