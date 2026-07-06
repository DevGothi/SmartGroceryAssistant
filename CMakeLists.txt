cmake_minimum_required(VERSION 3.16)
project(SmartGroceryAssistant LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Where the header files live
include_directories(include)

# Build the search/delete test program (no Qt needed)
add_executable(test_run
    src/ItemSearchDelete.cpp
    tests/test_main.cpp
)

add_executable(demo
    src/ItemSearchDelete.cpp
    tests/interactive_main.cpp
)