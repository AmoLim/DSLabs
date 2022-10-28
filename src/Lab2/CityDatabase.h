//
// Created by Amo on 2022/10/28.
//

#ifndef LABS_CITYDATABASE_H
#define LABS_CITYDATABASE_H

#include <utility>
#include <cmath>

#include "BSTree.h"

/** Basic structs for City Database */
struct COOR {
    double x;
    double y;

    explicit COOR(double x1 = 0, double y1 = 0)
    : x(x1), y(y1) {
        // empty
    }

    bool operator ==(const COOR & rhs) const;

    bool operator !=(const COOR & rhs) const;

    bool operator >(const COOR & rhs) const;

    bool operator <(const COOR & rhs) const;
};

struct City {
    std::string name;
    COOR position;

    City(std::string n, double x, double y)
    : name(std::move(n)), position(x, y){
    }

    City(std::string n, const COOR & p)
    : name(std::move(n)), position(p) {
        // empty
    }

    bool operator >(const City & rhs) const;

    bool operator <(const City & rhs) const;

    bool operator ==(const City & rhs) const;

    bool operator !=(const City & rhs) const;

    friend std::ostream & operator <<(std::ostream & out, const City & rhs);

    /** Get the distance of two city.*/
    static double getDistance(const City & lhs, const City & rhs);
};

static const City NULL_CITY = City("", 0 , 0);

/** CityNameTree.
 * A binary tree that managing City by City's name.
 * Inherited from BinarySearchTree<City>. */
class CityNameTree : public BinarySearchTree<City> {
public:
    /**
     * Remove a city from the tree based on its name
     * @param name city's name
     */
    void remove(std::string name);

    /**
     * Remove a city from the tree based on its position
     * @param position the coordinate of the city
     */
    void remove(const COOR & position);

    const City & find(std::string name);

    const City & find(const COOR & position);

private:
    // Internal help functions.

    /** Remove node with COOR position */
    void removeByCOOR(const COOR & position, BinaryNode * t);

    /** Remove node with Name name*/
    void removeByName(std::string name);

    /** Traverse the subtree to find the city with given coordinate */
    BinaryNode * findByCOOR(const COOR & position, BinaryNode *t);

    /** Find node with specified name in subtree.
     * @param target is the City with the specified name and unknown position.
     * @return return a node containing the whole information of the City with specified name. */
    BinaryNode * findByName(const City & target, BinaryNode * t);
};


#endif //LABS_CITYDATABASE_H
