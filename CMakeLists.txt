cmake_minimum_required(VERSION 3.16)
project(SmartGroceryAssistant LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(test_run
        src/ItemSearchDelete.cpp
        src/Budget.cpp
        src/DataManager.cpp
        tests/test_main.cpp
)

add_executable(demo
        src/ItemSearchDelete.cpp
        src/Budget.cpp
        src/DataManager.cpp
        tests/interactive_main.cpp
)