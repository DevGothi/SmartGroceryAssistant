cmake_minimum_required(VERSION 3.16)
project(SmartGroceryAssistant LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt automatic processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Header files
include_directories(include)

# Main Qt application
add_executable(SmartGrocery
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    src/HelpDialog.cpp
    src/Budget.cpp
    src/DataManager.cpp
    src/ItemSearchDelete.cpp
)

target_link_libraries(SmartGrocery PRIVATE Qt6::Widgets)

# Optional console test
# Uncomment when GroceryManager.cpp exists
#
# add_executable(test_run
#     src/GroceryManager.cpp
#     tests/test_main.cpp
# )