
#include "Lab2/CityDatabase.h"
#include <iostream>
#include <chrono>

using namespace std;
int main() {
    CityNameTree cityNameTree;

    cityNameTree.insert(City("Shanghai", 0, 0));

    cityNameTree.insert(City("Beijing", 0, 3));

    cityNameTree.insert(City("Tianjing", 0, 1));

    cityNameTree.remove(COOR(0, 0));

    cityNameTree.print(cout);
    City city1 = cityNameTree.find("Beijing");

    cityNameTree.printCityWithinDistance(1, 1, 2, cout);

    return 0;
}
