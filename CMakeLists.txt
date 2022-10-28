cmake_minimum_required(VERSION 3.23)
project(Labs)

set(CMAKE_CXX_STANDARD 17)

set(CITYDATABASE src/Lab2/CityDatabase.cpp)

include_directories(src)
add_executable(Lab1 main.cpp
                   # src/Lab1/BigInt.cpp
                   # src/SLListTest.cpp
                    ${CITYDATABASE}
        )
