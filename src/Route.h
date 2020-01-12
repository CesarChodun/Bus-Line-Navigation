/** @file Route.h
 *  Interface for the 'Route' class which contain information about a Route.
 *
 * @author Cezary Chodun
 */

#ifndef Route_h
#define Route_h

#include <stdbool.h>

#include "vector.h"
#include "City.h"

/// @private
typedef struct Route Route;

/**
    @brief
        Creates a new Route.
    @return
        A pointer to the Route.
 */
Route *createRoute(unsigned number);

/**
    @brief
        Destroys the Route.
 <b>NOTE: </b> the "route" pointer becomes invalid.
 */
void destroyRoute(Route *route);

/**
    @brief
        Creates a new vector with the cities(see \ref City)
        from the route.
 */
vector *getNewCitiesRoute(Route *route, bool withStart, bool withEnd);

/**
    @brief
        Returns a city(a or b) that is closer to the route start.
        When using only roads in the route.
    @return a, b, or NULL(if could not found either)
 */
City *firstCityInRoute(Route *route, City *a, City *b);

/**
    @brief
        Sets the route roads to the roads supplied in "roads".
 <b>NOTE: </b> Previous roads are discarded.
 */
void copyRoadsRoute(Route *route, vector *roads);

/**
    @brief
        Inserts the "roads" into the route.
 */
void insertRoadsRoute(Route *route, vector *roads, int insertionPoint);

/**
    @brief
        Returns the roads from the route
        (from start to end).
    @return
        A pointer to the vector containing route roads.
 */
vector *getRouteRoads(Route *route);

/**
    @brief
        Returns the City index in route(from start).
    @return
        -1 if could not find the city, otherwise:
        x e <0, num. of roads in route).
 */
int getCityIndexInRoute(Route *route, City *city);

/**
    @brief
        Sets the start of the route.
    <b>NOTE: </b> It <b>SHOULD</b> be ensured that this
        method is called before adding cities to
        the route.
 */
void setRouteStart(Route *route, City *start);

/**
    @brief
        Returns the start of the route.
    @return
        A pointer to the first City in the route.
 */
City *getRouteStart(Route *route);

/**
    @brief
        Sets the end of the route.
    <b>NOTE: </b> It <b>SHOULD</b> be ensured that this
        method is called before adding cities to
        the route.
 */
void setRouteEnd(Route *route, City *end);

/**
    @brief
        Returns the end of the route.
    @return
        A pointer to the last City in the route.
 */
City *getRouteEnd(Route *route);

/**
    @brief
        Removes a road from the route.
 */
void removeRoadRoute(Route *route, Road *road);

#endif /* Route_h */
