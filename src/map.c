/** @file
 *      The implementation of a class containing a map of routes.
 * 
 *  @author
 *      Cezary Chodun
 */

#include "map.h"

#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "PriorityQueue.h"
#include "Route.h"
#include "Road.h"
#include "Trie.h"
#include "City.h"
#include "Text.h"

/**
    A data structure containing a map of routes.
 */
typedef struct Map{
    /** Names of the cities(see @ref City) in the map. */
    Trie *cityNames;

    /** A list of the cities(see @ref City) in the map. */
    vector *cities;

    /** A list of routes(see @ref Route) in the map. */
    vector *routes;

    ///@private
    vector *id_ptrs;
}Map;

Map *newMap() {
    Map *out = (struct Map*) malloc(sizeof(Map));
    if (out == NULL)
        return NULL;

    out->cityNames = newTrie(260);
    out->cities = newVec(10);
    out->routes = newVec(1000);
    out->id_ptrs = newVec(10);

    if (out->cityNames == NULL || out->cities == NULL ||
       out->routes == NULL || out->id_ptrs == NULL) {
        deleteMap(out);
        return NULL;
    }

    for (int i = 0; i < 1000; i++)
        if (pushBackVec(out->routes, NULL) == NULL)
            return NULL;

    return out;
}

///@private
static Road *remRoad(City *c1, City *c2);

void deleteMap(Map *map) {
    if (map == NULL)
        return;

    for (int i = 0; i < vecSize(map->routes); i++)
        destroyRoute(getVec(map->routes, i));

    for (int i = 0; i < vecSize(map->cities); i++) {
        City *c1 = getVec(map->cities, i);

        vector *roads = getRoadsCity(c1);
        while (vecSize(roads) > 0) {
            Road *road = backVec(roads);
            clearRoutesRoad(road);
            remRoad(c1, getConnectedCity(road, c1));
            destroyRoad(road);
        }
    }

    for (int i = 0; i < vecSize(map->cities); i++)
        destroyCity(getVec(map->cities, i));

    for (int i = 0; i < vecSize(map->id_ptrs); i++)
        free(getVec(map->id_ptrs, i));

    if (map->cityNames != NULL)
        destroyTrie(map->cityNames);
    if (map->cities != NULL)
        destroyVec(map->cities);
    if (map->routes != NULL)
        destroyVec(map->routes);
    if (map->id_ptrs)
        destroyVec(map->id_ptrs);

    free(map);
}

///@private
static int getCityNameSize(const char *city) {
    if (city == NULL)
        return -1;

    int size = 0;

    for (int i = 0; city[i] != '\0'; i++) {
        if ((city[i] <= 31 && city[i] >= 0) || city[i] == ';')
            return -1;
        size++;

    }

    return size;
}

/// @private
static Route *getRoute(Map *map, unsigned routeId) {
    if (routeId == 0 || routeId > 999)
        return NULL;

    return getVec(map->routes, routeId);
}

/// @private
static Route *addRoute(Map *map, unsigned routeId) {
    if (routeId == 0 || routeId > 999)
        return NULL;//Wrong parameters

    Route *route = createRoute(routeId);
    if (route == NULL)
        return NULL;//Failed to allocate

    setVec(map->routes, routeId, route);

    return route;
}

/// @private
static int nextID(Map *map){
    return vecSize(map->cities);
}

/// @private
static City *addCity(Map *map, const char *city) {

    int *cityID = (int*) malloc(sizeof(int));
    if (cityID == NULL)
      return NULL;

    cityID[0] = nextID(map);
    pushBackVec(map->id_ptrs, cityID);

    City *out = newCity(*cityID, city);
    if (out == NULL)
        return NULL;

    int size = getCityNameSize(city);
    if (size <= 0) {
        destroyCity(out);
        return NULL;
    }

    if (addTrie(map->cityNames, city, size, cityID) == NULL) {
        destroyCity(out);
        return NULL;
    }

    pushBackVec(map->cities, out);
    return out;
}

/// @private
static City *getCity(Map *map, const char *city) {
    if (city == NULL)
        return NULL;

    int size = getCityNameSize(city);
    if (size <= 0)
        return NULL;

    int *id = getTrie(map->cityNames, city, size);

    if (id == NULL)
        return NULL;//Not found
    return getVec(map->cities, id[0]);
}

/// @private
typedef struct Distance{
    City *city;
    unsigned distance;
    int oldestRoad;
}Distance;

/// @private
static Distance *newDistance() {
    Distance *out = (struct Distance*) malloc(sizeof(Distance));
    if (out == NULL)
        return NULL;

    out->city = NULL;
    return out;
}

/// @private
static void destroyDistance(Distance *d) {
    if (d != NULL)
        free(d);
}

/// @private
static void distanceComparator(void *v1, void *v2, int *ret) {
    ret[0] = 0;

    if (v2 == NULL) {
        ret[0] = -1;
        return;//Every distance is smaller then NULL distance
    }
    else if (v1 == NULL) {
        ret[0] = 1;
        return;
    }

    Distance *d1 = (Distance*)v1;
    Distance *d2 = (Distance*)v2;

    assert(d1->city != NULL && d2->city != NULL);

    if (d1->distance > d2->distance)
        ret[0] = 1;
    else if (d1->distance < d2->distance)
        ret[0] = -1;
    else {
        if (d1->oldestRoad < d2->oldestRoad)
            ret[0] = 1;
        else if (d1->oldestRoad > d2->oldestRoad)
            ret[0] = -1;
    }
}

/// @private
static Distance *composeNewDistance(Distance *last, City *dest, Road *road, bool plus) {
    Distance *out = newDistance();//Calculating the next distance
    if (out == NULL)
        return NULL;

    out->city = dest;
    out->oldestRoad = (last->oldestRoad < getRoadYear(road))?
    last->oldestRoad : getRoadYear(road);
    out->distance = last->distance;
    if (plus)
        out->distance += getRoadLength(road);
    else
        out->distance -= getRoadLength(road);

    if (last->city == NULL)
        out->distance = INT_MAX;
    if (last->distance == INT_MAX)
        out->distance = INT_MAX;

    return out;
}

/// @private
static vector *createDistanceMap(Map *map, City *from, City *to, vector *forbidden, Distance *dst) {
    bool fatalError = false;
    dst->city = NULL;
    PriorityQueue *queue = newPriorityQueue(&distanceComparator);
    if (queue == NULL)
        return NULL;
    
    vector *visited = newVec(nextID(map)); //Distance list
    if (visited == NULL) {
        destroyPriorityQueue(queue);
        return NULL;
    }
    
    int inQueue[nextID(map)];
    
    for (int i = 0; i < nextID(map); i++) {
        Distance *d = newDistance();
        d->city = getVec(map->cities, i);
        d->distance = INT_MAX;
        d->oldestRoad = INT_MIN;
        pushBackVec(visited, d);
        inQueue[i] = 0;
    }
    
    if (forbidden != NULL && fatalError != true) {
        for (int i = 0; i < vecSize(forbidden); i++) {
            Distance *forbiddenDistance = newDistance();
            forbiddenDistance->city = NULL;//This distance cannot be changed
            forbiddenDistance->distance = INT_MAX;
            forbiddenDistance->oldestRoad = INT_MIN;
            
            City *cf = getVec(forbidden, i);
            if (cf != NULL) {
                destroyDistance(getVec(visited, getCityID(cf)));
                setVec(visited, getCityID(cf), forbiddenDistance);
            }
            else
                destroyDistance(forbiddenDistance);
        }
    }
    
    //Create the first element
    Distance *start = newDistance();
    if (start == NULL)
        fatalError = true;
    else {
        start->city = from;
        start->distance = 0;
        start->oldestRoad = INT_MAX;
        
        destroyDistance(getVec(visited, getCityID(from)));
        setVec(visited, getCityID(from), start);
        
        //Add the first element
        if (addPQueue(queue, start) == NULL)
            fatalError = true;  //Failed to add to the queue
    }
    
    //Stores the Distance pointers(to be freed)
    vector *distanceMemoryCrap = newVec(nextID(map) + 10);
    if (distanceMemoryCrap == NULL)
        fatalError = true;  //Failed to allocate memory
    
    if (fatalError) {
        destroyPriorityQueue(queue);
        destroyVec(visited);
        if (distanceMemoryCrap != NULL)
            free(distanceMemoryCrap);
        return NULL;
    }
    
    while (getPQueue(queue) != NULL) {
        Distance *p = popPQueue(queue);
        if (p == NULL)
            break;
        if (p->city == to)
            break;
        
        if (inQueue[getCityID(p->city)] == 0) {
            inQueue[getCityID(p->city)] = 1;
            vector *roads = getRoadsCity(p->city);
            
            for (int i = vecSize(roads) - 1; i >= 0; i--) {
                Road *road = getVec(roads, i);
                City *dest = getConnectedCity(road, p->city);
                int destId = getCityID(dest);
                if (dest == NULL) {
                    fatalError = true;
                    break;
                }
                
                //Calculating the next distance
                Distance *nextDis = composeNewDistance(p, dest, road, true);
                if (nextDis == NULL) {
                    fatalError = true;
                    break;
                }
                
                //Checking if the new distance is smaller
                Distance *fromVisited = getVec(visited, destId);
                if (fromVisited == NULL || (fromVisited->city != NULL)) {
                    int ret = 0;
                    distanceComparator(nextDis, fromVisited, &ret);
                    if (ret == -1) {//The distance is smaller
                        if (addPQueue(queue, nextDis) == NULL) {
                            fatalError = true;
                            free(nextDis);
                            break;
                        }
                        pushBackVec(distanceMemoryCrap, fromVisited);
                        setVec(visited, destId, nextDis);
                    }
                    else
                        destroyDistance(nextDis);
                }
                else
                    destroyDistance(nextDis);
                
            }
            
            if (fatalError)
                break;  //Eroor occured
        }
    }
    
    
    //Cleaning up
    for (int i = vecSize(distanceMemoryCrap) - 1; i >= 0; i--)
        free(getVec(distanceMemoryCrap, i));
    
    destroyVec(distanceMemoryCrap);
    destroyPriorityQueue(queue);
    
    return visited;
}

/// @private
static vector *shortestRoute(Map *map, City *from, City *to, vector *forbidden, Distance *dst) {
    if (from == NULL || to == NULL)
        return NULL;
    bool fatalError = false;
    
    vector *visited = createDistanceMap(map, from, to, forbidden, dst);
    if (visited == NULL)
        return NULL;

    //List of routes from cities 'from' to 'to'.
    vector *out = newVec(10);;
    if (out == NULL)
        fatalError = true;

    if (!fatalError) {
        City *last = to;
        int lastID = getCityID(last);
        Distance *lastDistance = getVec(visited, lastID);

        Distance *best = NULL;
        bool ambi = false;

        while (last != from && lastDistance->distance != INT_MAX) {
            vector *roads = getRoadsCity(last);
            Road *bestRoad = NULL;
            best = NULL;

            for (int i = vecSize(roads) - 1; i >= 0; i--) {
                Road *road = getVec(roads, i);
                City *dest = getConnectedCity(road, last);

                int ret = 0;
                Distance *stored = composeNewDistance(getVec(visited, getCityID(dest)), dest, road, true);
                if (stored == NULL) {
                    fatalError = true;//Cannot reach the city
                    break;
                }
                distanceComparator(best, stored, &ret);
                if (ret == 1 && stored->city != NULL) {
                    free(best);
                    best = stored;
                    bestRoad = road;
                    ambi = false;
                }
                else
                    destroyDistance(stored);

                if (ret == 0)
                    ambi = true;
            }

            if (ambi) {
                destroyDistance(best);
                best = NULL;
            }

            if (best == NULL || lastDistance->distance != best->distance) {
                fatalError = true;
                if (best != NULL)
                    destroyDistance(best);
            }
            if (fatalError)
                break;

            last = best->city;
            lastID = getCityID(last);
            lastDistance = getVec(visited, lastID);
            pushBackVec(out, bestRoad);
            free(best);
        }

        reverseVec(out);
    }


    Distance *tmpDis = getVec(visited, getCityID(to));
    if (tmpDis == NULL || tmpDis->distance == INT_MAX) {//Cannot reach the city
        dst->city = from;
        dst->distance = INT_MAX;
        fatalError = true;
    }
    else {
        if (fatalError == true)
            dst->city = NULL;//Some other error(eg. memory)
        else {
            dst->city = tmpDis->city;//Route found
            dst->distance = tmpDis->distance;
            dst->oldestRoad = tmpDis->oldestRoad;
        }
    }

    for (int i = 0; i < vecSize(visited); i++)
       destroyDistance(getVec(visited, i));
    
    destroyVec(visited);

    if (fatalError) {
        destroyVec(out);
        return NULL;//Problems with memory allocation
    }
    return out;//Everything went well
}


/// @private
static vector *fixRouteVec(Map *map, Route *route, City *c1, City *c2) {
    City *first = firstCityInRoute(route, c1, c2);
    City *second = c1;
    if (second == first)
        second = c2;

    vector *forbidden = getNewCitiesRoute(route, true, true);
    for (int i = 0; i < vecSize(forbidden); i++)
        if (getVec(forbidden, i) == second) {
            removeVec(forbidden, i);
            i--;
        }

    Distance d;
    vector *roads = shortestRoute(map, first, second, forbidden, &d);
    destroyVec(forbidden);

    return roads;
}

/// @private
static int connectedRoadID(vector *roads, City *c) {
    for (int i = 0; i < vecSize(roads); i++)
        if (getConnectedCity(getVec(roads, i), c) != NULL)
            return i;
    return -1;
}

/// @private
static Road *remRoad(City *from, City *to) {
    vector *roads1 = getRoadsCity(from);
    vector *roads2 = getRoadsCity(to);
    if (roads1 == NULL || roads2 == NULL)
        return NULL;

    int x = connectedRoadID(roads1, to);
    int y = connectedRoadID(roads2, from);

    if (x == -1 || y == -1)
        return NULL;

    Road *out = getVec(roads1, x);
    removeVec(roads1, x);
    removeVec(roads2, y);

    return out;
}

/**
 @private
 @brief
 Finds the road that connects cities.
 @return
 The road that connects city 'from' with
 city 'to', or NULL if no such road exists.
 */
static void *getRoadCity(City *from, City *to) {
    vector *roads = getRoadsCity(from);
    for (int i = 0; i < vecSize(roads); i++)
        if (getConnectedCity(getVec(roads, i), from) == to)
            return getVec(roads, i);
    
    return NULL;
}

// Defined in map.h

bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int builtYear) {
    if (builtYear == 0 || map == NULL || length == 0)
        return false;//Wrong parameters

    City *c1, *c2;

    if ((c1 = getCity(map, city1)) == NULL)
        if ((c1 = addCity(map, city1)) == NULL)
            return false;//Failed to allocate memory
    if ((c2 = getCity(map, city2)) == NULL)
        if ((c2 = addCity(map, city2)) == NULL)
            return false;//Failed to allocate memory
    if (c1 == c2)
        return false;

    vector *roads = getRoadsCity(c1);
    for (int i = 0; i < vecSize(roads); i++)
        if (getConnectedCity(getVec(roads, i), c1) == c2)
            return NULL;

    Road *r = newRoad(c1, c2, builtYear, length);
    if (r == NULL)
        return false;//Failed to allocate memory

    addCityRoad(c1, r);
    addCityRoad(c2, r);

    return true;//Everything went well
}

bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear) {
    if (map == NULL || city1 == NULL || city2 == NULL)
        return false;//Wrong parameters

    if (repairYear == 0)
        return false;//Wrong parameters

    City *c1, *c2;

    if ((c1 = getCity(map, city1)) == NULL)
        return false;//City not found
    if ((c2 = getCity(map, city2)) == NULL)
        return false;//City not found

    if (c1 == c2)
        return false;//Same city

    vector *roads = getRoadsCity(c1);
    int roadsSize = vecSize(roads);

    for (int i = 0; i < roadsSize; i++) {
        City *c = getConnectedCity(getVec(roads, i), c1);
        if (c == c2) {
            Road *r = getVec(roads, i);
            if (getRoadYear(r) > repairYear)
                return false;//Wrong repair year

            setRoadYear(r, repairYear);
            return true;//Everything went well
        }
    }

    return false;//Road not found
}

bool newRoute(Map *map, unsigned routeId,
              const char *city1, const char *city2) {
    if (map == NULL || city1 == NULL || city2 == NULL)
        return false;

    if (getRoute(map, routeId) != NULL)
        return false;//Route with the same number already exists

    City *from, *to;
    if ((from = getCity(map, city1)) == NULL)
        return false;//'city1' doesn't exists(or other problem)
    if ((to = getCity(map, city2)) == NULL)
        return false;//'city2' doesn't exists(or other problem)

    if (from == to)
        return false;//Cities are the same

    Route *route = addRoute(map, routeId);
    if (route == NULL)
        return false;//Failed to allocate memory

    Distance routeLength;
    vector *roads = shortestRoute(map, from, to, NULL, &routeLength);
    if (roads == NULL) {
        destroyRoute(route);
        setVec(map->routes, routeId, NULL);
        return false;
    }

    setRouteStart(route, from);
    setRouteEnd(route, to);
    copyRoadsRoute(route, roads);
    destroyVec(roads);

    return true;
}

/// @private
static void cityComparator(void *a, void *b, int *ret){
    City *c1 = (City *) a;
    City *c2 = (City *) b;
    
    if (getCityID(c1) > getCityID(c2))
        ret[0] = 1;
    else if (getCityID(c1) < getCityID(c2))
        ret[0] = -1;
    else
        ret[0] = 0;
}

bool exactRoute(Map *map, unsigned num, vector *cityNames, vector *roadLengths, vector *roadBuiltYears) {
    if (map == NULL || cityNames == NULL || roadLengths == NULL || roadBuiltYears == NULL)
        return false;   //Wrong parameters
    if (num < 1 || num > 999)
        return false;   //Wrong parameters
    if (getVec(map->routes, num) != NULL)
        return false;   //Route with that number already exists
    for (int i = 0; i < vecSize(cityNames); i++)
        if (getCityNameSize((char*) getVec(cityNames, i)) <= 0)
            return false;   //Wrong city name
    
    bool err = false;
    
    PriorityQueue *visited = newPriorityQueue(&cityComparator); //Heap containing visited cities
    if (visited == NULL)
        return false;
    
    Route *route = createRoute(num);
    if (route == NULL)
        return false;
    
    //Check routes
    vector *routeRoads = newVec(vecSize(roadLengths));  //Roads that are a part of the route
    vector *roadsToAdd = newVec(vecSize(roadLengths));  //Roads that have to be added to the map
    if (routeRoads == NULL || roadsToAdd == NULL)
        err |= true;
    
    bool lastCreated = false;
    City *last = getCity(map, (char*) getVec(cityNames, 0));
    if (last == NULL) {
        lastCreated = true;
        last = addCity(map, (char*) getVec(cityNames, 0));
    }
    setRouteStart(route, last);
    
    
    if (last != NULL){
        addPQueue(visited, last);
    
        // Checking if the Cities(roads) exists in the map:
        for (int i = 1; i < vecSize(cityNames); i++) {
            err |= (last == NULL);    //Previously visited city doesn't exist.
            if (err)
                break;
            
            City *c = getCity(map, (char*) getVec(cityNames, i));   //Next City in the route
            Road *r = NULL; //Road connecting previous City('last') and the next City('c').
            
            //  Trying to obtain the road('r') from the map
            if (!lastCreated && c != NULL) {
                r = getRoadCity(last, c);
                
                if (r != NULL) {  // Road exists
                   if (getRoadLength(r) != *(int*) getVec(roadLengths, i - 1) ||  // Length is different
                      getRoadYear(r) > *(int*) getVec(roadBuiltYears, i - 1)) {  // Built year is wrong
                       err |= true;
                       break; //Wrong built year or length
                   }
                }
            }
            
            lastCreated = false;
            if (c == NULL) {    //City with the given name doesn't exist in the map.
                lastCreated = true;
                c = addCity(map, (char*) getVec(cityNames, i)); //New city added to the map.
            }
            if (r == NULL) {    //There is no road between 'last' nad 'c'.
                unsigned length = *(int*) getVec(roadBuiltYears, i - 1);
                int year = *(int*) getVec(roadLengths, i - 1);
                
                if (length == 0 || year == 0) {
                    err = true;
                    break;  //Cannot create road with zero build year, and/or zero length.
                }
                r = newRoad(last, c, length, year); // New road between 'last' and 'c'.
                pushBackVec(roadsToAdd, r);
                err |= (r == NULL);
            }
            
            last = c;
            if (containsPQueue(visited, last) != NULL) { //There is a loop in the route description
                err = true;
                break;
            }
            addPQueue(visited, last);
            pushBackVec(routeRoads, r);
        }
        err |= (last == NULL);
    }
    else
        err = true;
    
    if (!err) {
        // No error occured previously. The route can be created.
        setRouteEnd(route, last);
        copyRoadsRoute(route, routeRoads);
        
        for (int i = 0; i < vecSize(routeRoads); i++) {
            setRoadYear(getVec(routeRoads, i), *(int*) getVec(roadBuiltYears, i));
            addRouteRoad(getVec(routeRoads, i), route);
        }
        for (int i = 0; i < vecSize(roadsToAdd); i++) {
            Road *road = getVec(roadsToAdd, i);
            City *a = getAnyCityFromRoad(road);
            City *b = getConnectedCity(road, a);
            
            addCityRoad(a, road);
            addCityRoad(b, road);
        }
        
        setVec(map->routes, num, route);
    }
    
    
    //Cleanup
    if (roadsToAdd != NULL && err)
        for (int i = 0; i < vecSize(roadsToAdd); i++)
            destroyRoad(getVec(roadsToAdd, i));
    destroyVec(routeRoads);
    destroyVec(roadsToAdd);
    
    if (err)
        destroyRoute(route);
    destroyPriorityQueue(visited);
    
    return !err;
}

bool extendRoute(Map *map, unsigned routeId, const char *city) {
    if (map == NULL || city == NULL)
        return NULL; //Wrong pareameters

    City *c = getCity(map, city);
    if (c == NULL)
        return NULL; //Could not find the city

    Route *route = getRoute(map, routeId);
    if (route == NULL)
        return NULL; //Could not find the route

    vector *roads = getRouteRoads(route);
    for (int i = 0; i < vecSize(roads); i++)
        if (getConnectedCity(getVec(roads, i), c) != NULL)
            return NULL; //The route contains the City

    vector *forbidden = getNewCitiesRoute(route, false, true);

    Distance fromStart, fromEnd;
    fromStart.city = NULL;
    fromStart.distance = INT_MAX;
    fromStart.oldestRoad = INT_MIN;
    fromEnd.city = NULL;
    fromEnd.distance = INT_MAX;
    fromEnd.oldestRoad = INT_MIN;

    vector *roadsFromStart =
        shortestRoute(map, c, getRouteStart(route), forbidden, &fromStart);

    pushBackVec(forbidden, getRouteStart(route));
    vector *roadsFromEnd =
        shortestRoute(map, getRouteEnd(route), c, forbidden, &fromEnd);

    bool fatalError = false;
    if (fromStart.city == NULL || fromEnd.city == NULL)
        fatalError = true;
    else {
        int ret = 0;
        distanceComparator(&fromStart, &fromEnd, &ret);

        if (ret == 0 && fromStart.city != NULL && fromEnd.city != NULL)
            fatalError = true;//Choice is ambiguous
        else if ((ret == 1 || roadsFromStart == NULL) && roadsFromEnd != NULL)
            insertRoadsRoute(route, roadsFromEnd, vecSize(roads));
        else if ((ret == -1 || roadsFromEnd == NULL) && roadsFromStart != NULL)
            insertRoadsRoute(route, roadsFromStart, 0);
        else
            fatalError = true;//Cannot reach the city
    }

    destroyVec(forbidden);
    destroyVec(roadsFromStart);
    destroyVec(roadsFromEnd);

    if (fatalError)
        return false;
    return true;
}

bool removeRoad(Map *map, const char *city1, const char *city2) {
    if (map == NULL || city1 == NULL || city2 == NULL)
        return false;//Wrong parameters

    City *c1, *c2;
    if ((c1 = getCity(map, city1)) == NULL)
        return false;
    if ((c2 = getCity(map, city2)) == NULL)
        return false;
    if (c1 == c2)
        return false;

    Road *road = remRoad(c1, c2);
    if (road == NULL)
        return false;

    bool err = false;
    vector *routes = getRoutesRoad(road);
    vector *inserts = newVec(vecSize(routes));
    vector *insertionPoints = newVec(vecSize(routes));

    if (routes != NULL)
        for (int i = 0; i < vecSize(routes); i++) {
            vector *insert = fixRouteVec(map, getVec(routes, i), c1, c2);
            City *insertionPoint = firstCityInRoute(getVec(routes, i), c1, c2);
            if (insert == NULL || insertionPoint == NULL) {
                err = true;
                break;
            }

            pushBackVec(inserts, insert);
            pushBackVec(insertionPoints, insertionPoint);
        }

    if (!err) {
        for (int i = 0; i < vecSize(routes); i++) {
            Route *route = getVec(routes, i);
            vector *insert = getVec(inserts, i);
            int ip = getCityIndexInRoute(route, getVec(insertionPoints, i));

            removeRoadRoute(route, road);
            insertRoadsRoute(route, insert, ip);

            destroyVec(insert);
        }

        destroyRoad(road);
    }
    else {
        for (int i = 0; i < vecSize(inserts); i++)
            destroyVec((vector *) getVec(inserts, i));

        addCityRoad(c1, road);
        addCityRoad(c2, road);
    }

    destroyVec(inserts);
    destroyVec(insertionPoints);

    return !err;
}

//TODO: Check this function for memory leaks
bool removeRoute(Map *map, unsigned routeId){
    if (map == NULL || routeId == 0 || routeId > 999)
        return false; //Wrong parameters
    
    Route *route = getVec(map->routes, routeId);
    if (route == NULL)
        return false; //Route does not exist
    
    vector *roads = getRouteRoads(route);
    for (int i = 0; i < vecSize(roads); i++)
        removeRouteRoad(getVec(roads, i), route);
    
    destroyRoute(route);
    setVec(map->routes, routeId, NULL);
    
    return true; //Route successfuly deleted
}

char const *getRouteDescription(Map *map, unsigned routeId) {
    if (map == NULL)
        return NULL;//Wrong parameters

    Text *out = newText("");
    if (out == NULL)
        return NULL;//Failed to allocate memory
    
    Route *route = getRoute(map, routeId);
    if (route == NULL) {
        char *str_out = to_cString(out);
        destroyText(out);
        return str_out;
    }
    
    Text *tmp = out;
    tmp = appendUInt(out, routeId);
    tmp = appendString(out, ";");

    City *last = getRouteStart(route);
    vector *roads = getRouteRoads(route);
    for (int i = 0; i < vecSize(roads) && tmp != NULL; i++) {
        tmp = appendString(out, getCityName(last));
        tmp = appendString(out, ";");

        Road *road = getVec(roads, i);

        tmp = appendInt(out, getRoadLength(road));
        tmp = appendString(out, ";");
        tmp = appendInt(out, getRoadYear(road));
        tmp = appendString(out, ";");

        last = getConnectedCity(road, last);
    }
    tmp = appendString(out, getCityName(getRouteEnd(route)));

    if (tmp == NULL) {
        destroyText(out);
        return NULL;//Failed to add data to the string
    }

    char *ready = to_cString(out);
    destroyText(out);
    if (ready == NULL)
        return NULL;//Failed to allocate memory

    return ready;
}
