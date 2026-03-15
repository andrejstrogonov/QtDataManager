cmake_minimum_required(VERSION 3.21)
project(QtDataManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Автоматическая генерация мета-кода Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Test Qml)

# Список исходных файлов
set(SOURCES
    main.cpp
    dbmanager.cpp
    models/treemodel.cpp
    models/treenode.cpp
    models/spectablemodel.cpp
    views/mainwindow.cpp
    controllers/valuedelegate.cpp
)

# Список заголовочных файлов
set(HEADERS
    dbmanager.h
    models/treemodel.h
    models/treenode.h
    models/spectablemodel.h
    views/mainwindow.h
    controllers/valuedelegate.h
)

# --- Основное приложение ---
# Используем имя QtDataManagerApp, чтобы избежать конфликта с папкой проекта
qt_add_executable(QtDataManagerApp 
    ${SOURCES} 
    ${HEADERS} 
    views/mainwindow.ui
)

# Конфигурация QML модуля (привязана к QtDataManagerApp)
qt_add_qml_module(QtDataManagerApp
    URI QtDataManager
    VERSION 1.0
    QML_FILES main.qml
)

target_include_directories(QtDataManagerApp PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(QtDataManagerApp PRIVATE Qt6::Widgets Qt6::Sql Qt6::Qml)

# --- Тесты ------------------------------------------------------------
enable_testing()

# 1. Тест TreeNode (добавлен treenode.cpp для линковки)
add_executable(test_treenode 
    tests/test_treenode.cpp 
    models/treenode.cpp 
    models/treenode.h
)
target_include_directories(test_treenode PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_treenode PRIVATE Qt6::Test Qt6::Widgets Qt6::Core)
add_test(NAME TreeNodeTest COMMAND test_treenode)

# 2. Тест ValueDelegate
add_executable(test_valuedelegate
    tests/test_valuedelegate.cpp
    controllers/valuedelegate.cpp
    controllers/valuedelegate.h
)
target_include_directories(test_valuedelegate PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_valuedelegate PRIVATE Qt6::Test Qt6::Widgets)
add_test(NAME ValueDelegateTest COMMAND test_valuedelegate)

# 3. Тест DbManager
add_executable(test_dbmanager
    tests/test_dbmanager.cpp
    dbmanager.cpp
    dbmanager.h
)
target_include_directories(test_dbmanager PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_dbmanager PRIVATE Qt6::Test Qt6::Sql)
add_test(NAME DbManagerTest COMMAND test_dbmanager)
