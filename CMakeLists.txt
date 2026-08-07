cmake_minimum_required(VERSION 3.16)

project(
    SmartGroceryAssistant
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------------------------------
# BACKEND: works on every group member's computer
# --------------------------------------------------

add_library(
    smart_grocery_core
    STATIC

    src/ShoppingItem.cpp
    src/GroceryItem.cpp
    src/ItemValidation.cpp
    src/ItemCrud.cpp
    src/ItemSearchDelete.cpp

    src/Budget.cpp
    src/DataManager.cpp
        src/GroceryManager.cpp
        src/ShoppingList.cpp
        src/UserProfile.cpp
        src/PurchaseRecord.cpp
)

target_include_directories(
    smart_grocery_core
    PUBLIC
    include
)

# --------------------------------------------------
# BACKEND TESTS: Dev can run these without Qt
# --------------------------------------------------

add_executable(
    dev_tests
    tests/test_main.cpp
        include/GroceryManager.h
        include/ShoppingList.h
        include/UserProfile.h
        include/PurchaseRecord.h
)

target_link_libraries(
    dev_tests
    PRIVATE
    smart_grocery_core
)

# Optional console demonstration
add_executable(
    interactive_demo
    tests/interactive_main.cpp
)

target_link_libraries(
    interactive_demo
    PRIVATE
    smart_grocery_core
)

# --------------------------------------------------
# QT GUI: built only when Qt is installed
# --------------------------------------------------

find_package(Qt6 QUIET COMPONENTS Widgets)

if(Qt6_FOUND)

    message(STATUS "Qt 6 found: building the GUI application.")

    add_executable(
        SmartGroceryAssistant

        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

        include/HelpDialog.h
        src/HelpDialog.cpp
    )

    target_include_directories(
        SmartGroceryAssistant
        PRIVATE
        include
    )

    target_link_libraries(
        SmartGroceryAssistant
        PRIVATE
        smart_grocery_core
        Qt6::Widgets
    )

    set_target_properties(
        SmartGroceryAssistant
        PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
    )

else()

    message(
        STATUS
        "Qt 6 was not found. GUI target skipped. "
        "Backend and tests can still be built."
    )

endif()

# --------------------------------------------------
# COMPILER WARNINGS
# --------------------------------------------------

if(MSVC)

    target_compile_options(
        smart_grocery_core
        PRIVATE
        /W4
    )

else()

    target_compile_options(
        smart_grocery_core
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )

endif()