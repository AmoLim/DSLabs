//
// Created by Amo on 2022/10/28.
//

#include "CityDatabase.h"

/** COOR Struct. */
bool COOR::operator ==(const COOR &rhs) const  {
    return (x == rhs.x) && (y == rhs.y);
}

bool COOR::operator !=(const COOR &rhs) const  {
    return !(*this == rhs);
}

bool COOR::operator >(const COOR &rhs) const  {
    return x == rhs.x? y > rhs.y : x > rhs.x;
}

bool COOR::operator <(const COOR &rhs) const {
    return x == rhs.x? y < rhs.y : x < rhs.x;
}

/** City Struct. */
double City::getDistance(const City &lhs, const City &rhs) {
    double dx = lhs.position.x - rhs.position.x;
    double dy = lhs.position.y - rhs.position.y;

    return std::sqrt( dx * dx + dy * dy);
}

bool City::operator >(const City &rhs) const {
    return this->name > rhs.name;
}

bool City::operator <(const City &rhs) const {
    return this->name < rhs.name;
}

bool City::operator ==(const City &rhs) const {
    return this->name == rhs.name;
}

bool City::operator !=(const City &rhs) const {
    return !(*this == rhs);
}

std::ostream & operator <<(std::ostream & out, const City & rhs) {
    if (rhs == NULL_CITY)
        out << "NULL CITY\n";
    else
        out << "City Name: " << rhs.name << "\nPosition( " << rhs.position.x << " , " << rhs.position.y <<" )\n";
    return out;
}

/** CityNameTree. */

// Internal help method.
void CityNameTree::removeByCOOR(const COOR &position, BinarySearchTree<City>::BinaryNode *t) {
    BinaryNode * target_node = findByCOOR(position, t);
    if (target_node == nullptr)
        return;

    City target_city(target_node->data);
    BinarySearchTree<City>::remove(target_city, target_node);
}


void CityNameTree::removeByName(std::string name) {
    City city(name, 0, 0);
    BinarySearchTree<City>::remove(city);
}

BinarySearchTree<City>::BinaryNode *
CityNameTree::findByCOOR(const COOR &position, BinarySearchTree<City>::BinaryNode *t) {
    if (t == nullptr)
        return nullptr;
    if (t->data.position == position)
        return t;
    BinaryNode * find_left = findByCOOR(position, t->left);
    BinaryNode * find_right = findByCOOR(position, t->right);

    return (find_left == nullptr)? find_right: find_left;
}

BinarySearchTree<City>::BinaryNode *
CityNameTree::findByName(const City & target, BinarySearchTree<City>::BinaryNode *t) {
    if (t == nullptr)
        return nullptr; // not found

    if (target > t->data)
        return findByName(target, t->right);
    if (target < t->data)
        return findByName(target, t->left);

    return t; // found
}

void CityNameTree::remove(const COOR &position) {
    removeByCOOR(position, root_);
}


void CityNameTree::remove(std::string name) {
    removeByName(name);
}

const City & CityNameTree::find(std::string name) {
    // construct a city with the given name.
    City target_city(name, 0, 0);

    BinaryNode * target = findByName(target_city, root_);
    if (target == nullptr)
        return NULL_CITY;
    return target->data;
}

const City & CityNameTree::find(const COOR &position) {
    BinaryNode * target = findByCOOR(position, root_);

    if (target == nullptr)
        return NULL_CITY;
    return target->data;
}



