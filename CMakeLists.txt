cmake_minimum_required(VERSION 3.23)
project(Labs)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
add_executable(Lab1 main.cpp
                    src/BigInt.cpp
                   # src/SLListTest.cpp
        )
