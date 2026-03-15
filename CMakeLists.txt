cmake_minimum_required(VERSION 3.21)
project(QtDataManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt's automatic moc / rcc / uic integration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Export compile commands for better IDE integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Test Qml)

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
    views/mainwindow.h
    controllers/valuedelegate.h
)

qt_add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    views/mainwindow.ui
)

# Ensure headers are found by relative includes like "models/treenode.h" or "controllers/valuedelegate.h"
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

qt_add_qml_module(${PROJECT_NAME} 
    URI QtDataManager
    VERSION 1.0
    QML_FILES main.qml
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Sql Qt6::Qml)

# Ensure the executable name does not collide with generated QML module folders
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME QtDataManagerApp
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# --- tests ------------------------------------------------------------
enable_testing()

add_executable(test_treenode
    tests/test_treenode.cpp
    models/treenode.cpp
)

target_include_directories(test_treenode PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(test_treenode PRIVATE Qt6::Test Qt6::Widgets)
add_test(NAME TreeNodeTest COMMAND test_treenode)

add_executable(test_valuedelegate
    tests/test_valuedelegate.cpp
    controllers/valuedelegate.cpp
)

target_include_directories(test_valuedelegate PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(test_valuedelegate PRIVATE Qt6::Test Qt6::Widgets)
add_test(NAME ValueDelegateTest COMMAND test_valuedelegate)

add_executable(test_dbmanager
    tests/test_dbmanager.cpp
    dbmanager.cpp
)

target_include_directories(test_dbmanager PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(test_dbmanager PRIVATE Qt6::Test Qt6::Sql)
add_test(NAME DbManagerTest COMMAND test_dbmanager)
