/** @file City.h
 *  Interface for the 'City' class which contain information about a City.
 *
 * @author Cezary Chodun
 */

#ifndef City_h
#define City_h

#include "vector.h"

/// @private
typedef struct Road Road;

/// @private
typedef struct City City;

/**
    @brief
        Creates a new City.
    @return
        A pointer to the City.
 */
City *newCity(int ID, const char *name);

/**
 @brief
     Frees the memory allocated by the City pointer.
     <b>NOTE: </b> after calling this function the "city"
     pointer will become invalid.
 */
void destroyCity(City *city);

/** Adds a Road to the City. */
void addCityRoad(City *city, Road *road);

/** Removes the x-th road from the City roads. */
void removeRoadCity(City *city, int x);

/**
    @brief
        Returns roads connected to the city.
    @return
        A pointer to the vector
        that conatains the City
        roads(see @ref Road)
 */
vector *getRoadsCity(City *city);

/**
    @brief
        Returns the City name.
    @return
        The City name.
 */
const char *getCityName(City *city);

/**
    @brief
        Return the City identification number.
    @return
        The City ID.
 */
int getCityID(City *city);

#endif /* City_h */
