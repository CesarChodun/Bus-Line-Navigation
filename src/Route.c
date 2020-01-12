/** @file map_main.c
 *  Class containing information about a route.
 *
 * @author Cezary Chodun
 */

#include "Route.h"

#include <stdlib.h>
#include <assert.h>

#include "Road.h"

/**
    Data structure that contains
    information about the route.
 */
typedef struct Route{
    /// The route number (0 < x < 1000).
    unsigned number;
    /// The route roads.
    vector *roads;
    
    ///The first city in the route.
    City *start;
    ///The last city in the route.
    City *end;
}Route;

Route *createRoute(unsigned number) {
    Route *out = (struct Route*) malloc(sizeof(Route));
    if (out == NULL)
        return NULL;

    out->number = number;
    out->roads = newVec(0);
    if (out->roads == NULL) {
        free(out);
        return NULL;
    }

    return out;
}

void destroyRoute(Route *route) {
    if (route == NULL)
        return;

    destroyVec(route->roads);
    free(route);
}

void removeRoadRoute(Route *route, Road *road) {
    vector *roads = getRouteRoads(route);
    int lid = -1;

    for (int i = 0; i < vecSize(roads); i++) {
        if (lid != -1)
            setVec(roads, lid++, getVec(roads, i));
        if (getVec(roads, i) == road)
            lid = i;
    }
    
    if (lid != -1)
        popBackVec(roads);
}

City *firstCityInRoute(Route *route, City *a, City *b) {
    City *last = getRouteStart(route);
    vector *roads = getRouteRoads(route);

    for (int i = 0; i < vecSize(roads); i++) {
        if (last == a)
            return a;
        if (last == b)
            return b;

        last = getConnectedCity(getVec(roads, i), last);
    }

    return NULL;
}

vector *getNewCitiesRoute(Route *route, bool withStart, bool withEnd) {
    if (route == NULL || route->roads == NULL)
        return NULL;
    vector *out = newVec(vecSize(route->roads) + 1);
    if (out == NULL)
        return NULL;

    City *last = route->start;
    for (int i = 0; i < vecSize(route->roads); i++) {
        Road *road = getVec(route->roads, i);

        if (last == route->start && !withStart);
        else if (last == route->end && !withEnd);
        else {
            pushBackVec(out, last);
        }

        last = getConnectedCity(road, last);
    }

    if (withEnd)
        pushBackVec(out, last);

    return out;
}

int getCityIndexInRoute(Route *route, City *city) {
    City *last = getRouteStart(route);
    vector *roads = getRouteRoads(route);

    for (int i = 0; i < vecSize(roads); i++) {
        if (last == city)
            return i;

        last = getConnectedCity(getVec(roads, i), last);
    }

    if (last == city)
        return vecSize(roads);
    return -1;
}

void insertRoadsRoute(Route *route, vector *roads, int insertionPoint) {
    if (vecSize(roads) == 0)
        return;

    vector *tmp = copyVec(roads);
    int insertSize = vecSize(tmp);

    if (vecSize(route->roads) == 0);
    else if (insertionPoint == 0) {
       if (commonCityRoad(getVec(tmp, insertSize - 1),
                      getVec(route->roads, insertionPoint)) == NULL)
           reverseVec(tmp);
    }
    else if (commonCityRoad(getVec(tmp, 0),
                      getVec(route->roads, insertionPoint - 1)) == NULL)
        reverseVec(tmp);

    for (int i = 0; i < vecSize(roads); i++)
        addRouteRoad(getVec(roads, i), route);

    insertVec(route->roads, tmp, insertionPoint);
    destroyVec(tmp);

    int size = vecSize(route->roads);
    assert(size > 1);
    route->start = getConnectedCity(getVec(route->roads, 0),
                        commonCityRoad(getVec(route->roads, 0),
                                       getVec(route->roads, 1)));
    route->end = getConnectedCity(getVec(route->roads, size - 1),
                        commonCityRoad(getVec(route->roads, size - 2),
                                       getVec(route->roads, size - 1)));
}

void copyRoadsRoute(Route *route, vector *roads) {
    destroyVec(route->roads);

    route->roads = copyVec(roads);
    if (getConnectedCity(getVec(route->roads, 0),
                        route->start) == NULL)
        reverseVec(route->roads);

    for (int i = 0; i < vecSize(roads); i++)
        addRouteRoad(getVec(roads, i), route);
}

vector *getRouteRoads(Route *route) {
    return route->roads;
}

void setRouteStart(Route *route, City *start) {
    route->start = start;
}

City *getRouteStart(Route *route) {
    return route->start;
}

void setRouteEnd(Route *route, City *end) {
    route->end = end;
}

City *getRouteEnd(Route *route) {
    return route->end;
}
