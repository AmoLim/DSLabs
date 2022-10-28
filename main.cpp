
#include "Lab2/CityDatabase.h"
#include <iostream>
#include <chrono>

using namespace std;
int main() {
    CityNameTree cityNameTree;

    cityNameTree.insert(City("Shanghai", 0, 0));

    cityNameTree.insert(City("Beijing", 0, 3));

    cityNameTree.remove("Shanghai");

    cityNameTree.print(cout);
    City city1 = cityNameTree.find("Beijing");


    return 0;
}
