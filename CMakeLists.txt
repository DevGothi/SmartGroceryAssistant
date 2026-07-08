cmake_minimum_required(VERSION 3.16)
project(SmartGroceryAssistant LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt needs these ON so it auto-processes GUI classes, .ui files, and resources
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Qt 6 (whole group is using Qt6)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Header files location
include_directories(include)

#  The Qt GUI application (the real project) 
add_executable(SmartGrocery
    src/main.cpp
    #src/GroceryManager.cpp
    #src/ItemsTabWidget.cpp
    src/HelpDialog.cpp
    # >>> teammates add their .cpp files here, e.g.:
    # src/MainWindow.cpp
    # src/Budget.cpp
    # src/DataManager.cpp
)

target_link_libraries(SmartGrocery PRIVATE Qt6::Widgets)

#  Optional: your console test (no Qt needed) 
add_executable(test_run
    src/GroceryManager.cpp
    tests/test_main.cpp
)