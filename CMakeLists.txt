cmake_minimum_required(VERSION 3.21)
project(QtDataManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Test Qml)

# --- Sources ---
set(SOURCES
    main.cpp
    dbmanager.cpp
    models/treemodel.cpp
    models/treenode.cpp
    models/spectablemodel.cpp
    views/mainwindow.cpp
    controllers/valuedelegate.cpp
)

set(HEADERS
    dbmanager.h
    models/treemodel.h
    models/treenode.h
    models/spectablemodel.h
    views/mainwindow.h
    controllers/valuedelegate.h
)

# --- Main App ---
qt_add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} views/mainwindow.ui)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Sql Qt6::Qml)

# --- Tests ---
enable_testing()

# FIX: Added models/treenode.cpp so the test has the actual code logic
add_executable(test_treenode 
    test_treenode.cpp 
    models/treenode.cpp 
    models/treenode.h
)
target_include_directories(test_treenode PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_treenode PRIVATE Qt6::Test Qt6::Widgets Qt6::Core)
add_test(NAME TreeNodeTest COMMAND test_treenode)

# FIX: Added missing headers/sources for other tests to prevent similar autogen errors
add_executable(test_valuedelegate
    tests/test_valuedelegate.cpp
    controllers/valuedelegate.cpp
    controllers/valuedelegate.h
)
target_link_libraries(test_valuedelegate PRIVATE Qt6::Test Qt6::Widgets)

add_executable(test_dbmanager
    tests/test_dbmanager.cpp
    dbmanager.cpp
    dbmanager.h
)
target_link_libraries(test_dbmanager PRIVATE Qt6::Test Qt6::Sql)
