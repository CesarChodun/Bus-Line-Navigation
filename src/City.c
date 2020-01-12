/** @file City.c
 *  Class containing information about a City.
 *
 * @author Cezary Chodun
 */

#include "City.h"

#include <stdlib.h>
#include <string.h>

#include "Text.h"

/**
    A data structure containing information
    about the given city.
*/
typedef struct City{
    /** City identification number. */
    int id;
    /** Roads(see @ref Road) that are connected
        to the City.
     */
    vector *roads;
    /** City name. */
    const char *name;
}City;

City *newCity(int id, const char *cityName) {
    City *out = (struct City*) malloc(sizeof(City));
    if (out == NULL)
        return NULL;

    int size = cStringSize(cityName);
    char *name = (char*) malloc((size + 1) * sizeof(char));
    if (name == NULL) {
        destroyCity(out);
        return NULL;
    }
    memcpy(name, cityName, size * sizeof(char));
    name[size] = '\0';

    out->roads = newVec(0);
    if (out->roads == NULL) {
        free(out);
        return NULL;
    }

    out->id = id;
    out->name = name;

    return out;
}

void destroyCity(City *city) {
    if (city == NULL)
        return;

    destroyVec(city->roads);
    if (city->name != NULL)
        free((void *) city->name);

    free(city);
}

void addCityRoad(City *city, Road *road) {
    pushBackVec(city->roads, road);
}

void removeRoadCity(City *city, int x) {
    setVec(city->roads, x, backVec(city->roads));
    popBackVec(city->roads);
}

vector *getRoadsCity(City *city) {
    return city->roads;
}

const char *getCityName(City *city) {
    return city->name;
}

int getCityID(City *city) {
    if (city == NULL)
        return -1;
    
    return city->id;
}
