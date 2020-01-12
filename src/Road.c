/** @file Road.c
 *  Class containing information about a Road.
 *
 * @author Cezary Chodun
 */

#include "Road.h"

#include <stdlib.h>

/**
    Data structure that contains
    information about the road.
 */
typedef struct Road{
    /// Citi connected by the road.
    City *a;
    /// Citi connected by the road.
    City *b;
    
    /// Routes that go through the road.
    vector *routes;

    /// The road build year.
    int year;
    /// The road length.
    int length;
}Road;

Road *newRoad(City *a, City *b, int year, int length) {
    Road *out = (struct Road*) malloc(sizeof(Road));
    if (out == NULL)
        return NULL;

    out->a = a;
    out->b = b;
    out->year = year;
    out->length = length;
    out->routes = newVec(0);
    if (out->routes == NULL) {
        free(out);
        return NULL;
    }

    return out;
}

void destroyRoad(Road *road) {
    if (road == NULL)
        return;

    destroyVec(road->routes);
    free(road);
}

City *getAnyCityFromRoad(Road *road) {
    return road->a;
}

City *commonCityRoad(Road *road1, Road *road2) {
    if (road1 == NULL || road2 == NULL)
        return NULL;

    if (road1->a == road2->a || road1->a == road2->b)
        return road1->a;
    if (road1->b == road2->a || road1->b == road2->b)
        return road1->b;
    return NULL;
}

City *firstDifferentCityRoad(Road *road1, Road *road2) {
    City *common = commonCityRoad(road1, road2);
    if (common == NULL)
        return NULL;

    City *out = getConnectedCity(road1, common);
    if (out == road2->a || out == road2->b)
        return NULL;
    return out;
}

void addRouteRoad(Road *road, Route *route) {
    pushBackVec(road->routes, route);
}

void removeRouteRoad(Road *road, Route *route){
    for (int i = 0; i < vecSize(road->routes); i++)
        if (getVec(road->routes, i) == route)
            removeVec(road->routes, i);
}

vector *getRoutesRoad(Road *road) {
    return road->routes;
}

void clearRoutesRoad(Road *road) {
    resetVec(road->routes);
}

City *getConnectedCity(Road *road, City *from) {
    if (road == NULL)
        return NULL;

    if (from != road->a && from != road->b)
        return NULL;

    if (road->a == from)
        return road->b;
    return road->a;
}

int getRoadYear(Road *road) {
    return road->year;
}

void setRoadYear(Road *road, int year) {
    road->year = year;
}

int getRoadLength(Road *road) {
    return road->length;
}
