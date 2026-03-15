cmake_minimum_required(VERSION 3.21)
project(QtDataManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt's automatic moc / rcc / uic integration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Test Qml)

# --- Define Source Groups ---
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
qt_add_executable(${PROJECT_NAME} 
    ${SOURCES} 
    ${HEADERS} 
    views/mainwindow.ui
)

# FIX: You must define the QML module for the executable to avoid "DQT_QML_MODEL" errors
qt_add_qml_module(${PROJECT_NAME}
    URI QtDataManager
    VERSION 1.0
    QML_FILES main.qml
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Sql Qt6::Qml)

# --- Tests Configuration ---
enable_testing()

# 1. TreeNode Test
# We include the .cpp and .h to ensure all symbols are present
add_executable(test_treenode 
    test_treenode.cpp 
    models/treenode.cpp 
    models/treenode.h
)
target_include_directories(test_treenode PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_treenode PRIVATE Qt6::Test Qt6::Widgets Qt6::Core)
add_test(NAME TreeNodeTest COMMAND test_treenode)

# 2. ValueDelegate Test
add_executable(test_valuedelegate
    tests/test_valuedelegate.cpp
    controllers/valuedelegate.cpp
    controllers/valuedelegate.h
)
target_include_directories(test_valuedelegate PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_valuedelegate PRIVATE Qt6::Test Qt6::Widgets)
add_test(NAME ValueDelegateTest COMMAND test_valuedelegate)

# 3. DbManager Test
add_executable(test_dbmanager
    tests/test_dbmanager.cpp
    dbmanager.cpp
    dbmanager.h
)
target_include_directories(test_dbmanager PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_dbmanager PRIVATE Qt6::Test Qt6::Sql)
add_test(NAME DbManagerTest COMMAND test_dbmanager)

