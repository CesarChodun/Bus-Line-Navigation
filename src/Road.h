/** @file Road.h
 *  Interface for the 'Road' class which contain information about a Road.
 *
 * @author Cezary Chodun
 */

#ifndef Road_h
#define Road_h

#include "vector.h"

/// @private
typedef struct Road Road;
/// @private
typedef struct Route Route;
/// @private
typedef struct City City;

/**
    @brief
        Creates a new Road.
    @return
        A pointer to the Road.
 */
Road *newRoad(City *a, City *b, int year, int length);

/**
    @brief
        Frees the data allocated in road.
 */
void destroyRoad(Road *road);

/**
    @brief
        Returns some city from the road.
    @return
        Some City connected by the road.
 */
City *getAnyCityFromRoad(Road *road);

/**
    @brief
        Returns a pointer to the City that
        is common for botth roads.
    @return
        A pointer to the common City.
 */
City *commonCityRoad(Road *road1, Road *road2);

/**
    @brief
        Returns the City that has the road "road1"
        and is not directly connected to any other
        City by "road2".
    @return
        A pointer to the City.
 */
City *firstDifferentCityRoad(Road *road1, Road *road2);

/**
    @brief
        Adds a route to the road.
 */
void addRouteRoad(Road *road, Route *route);

/**
    @brief
        Removes route from the road.
 */
void removeRouteRoad(Road *road, Route *route);

/**
    @brief
        Clears the routes
 */
void clearRoutesRoad(Road *road);

/**
    @brief
        Returns the routes that uses the road.
    @return
        Pointer to the routes vector.
 */
vector *getRoutesRoad(Road *road);

/**
    @brief
        Returns the City on the other side of the road.
    @return
        Pointer to the City.
 */
City *getConnectedCity(Road *road, City *from);

/**
    @brief
        Returns the road build/repair year.
    @return
        Build/repair year.
 */
int getRoadYear(Road *road);

/**
    @brief
        Sets the road repair year.
 */
void setRoadYear(Road *road, int year);

/**
    @brief
        Returns the length of the road.
    @return
        The length of the road.
 */
int getRoadLength(Road *road);

#endif /* Road_h */
