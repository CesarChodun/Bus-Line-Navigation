/** @file MapParser.h
 *  Interface for the 'MapParser' class which helps parse data to the Map.
 *
 * @author Cezary Chodun
 */

#ifndef MapParser_h
#define MapParser_h

#include <stdio.h>
#include <stdbool.h>

#include "vector.h"
#include "map.h"

/**
 * @brief
 *  Parses data(args) to the addRoad function.
 * @param[in, out] map  - the map;
 * @param[in] args      - a vector of arguments(Text).
 * @return @p true if the operation was successful, and
 *  @p false otherwise.
 */
bool addRoadFoo(Map *map, vector *args);

/**
 * @brief
 *  Parses data(args) to the repairRoad function.
 * @param[in, out] map  - the map;
 * @param[in] args      - a vector of arguments(Text).
 * @return @p true if the operation was successful, and
 *  @p false otherwise.
 */
bool repairRoadFoo(Map *map, vector *args);

/**
 * @brief
 *  Parses data(args) to the removeRoad function.
 * @param[in, out] map  - the map;
 * @param[in] args      - a vector of arguments(Text).
 * @return @p true if the operation was successful, and
 *  @p false otherwise.
 */
bool removeRoadFoo(Map *map, vector *args);

/**
 * @brief
 *  Parses data(args) to the newRoute function.
 * @param[in, out] map  - the map;
 * @param[in] args      - a vector of arguments(Text).
 * @return @p true if the operation was successful, and
 *  @p false otherwise.
 */
bool newRouteFoo(Map *map, vector *args);

/**
 * @brief
 *  Parses data(args) to the extendRoute function.
 * @param[in, out] map  - the map;
 * @param[in] args      - a vector of arguments(Text).
 * @return @p true if the operation was successful, and
 *  @p false otherwise.
 */
bool extendRouteFoo(Map *map, vector *args);

/**
 * @brief
 *  Parses data(args) to the exactRoute function.
 * @param[in, out] map  - the map;
 * @param[in] args      - a vector of arguments(Text).
 * @return @p true if the operation was successful, and
 *  @p false otherwise.
 */
bool exactRouteFoo(Map *map, vector *args);

/**
 * @brief
 *  Parses data(args) to the removeRoute function.
 * @param[in, out] map  - the map;
 * @param[in] args      - a vector of arguments(Text).
 * @return @p true if the operation was successful, and
 *  @p false otherwise.
 */
bool removeRouteFoo(Map *map, vector *args);

/**
 * @brief
 *  Parses data(args) to the getRouteDescription function.
 * @param[in, out] map  - the map;
 * @param[in] args      - a vector of arguments(Text).
 * @return @p true if the operation was successful, and
 *  @p false otherwise.
 */
bool getRouteDescriptionFoo(Map *map, vector *args);


#endif /* MapParser_h */
