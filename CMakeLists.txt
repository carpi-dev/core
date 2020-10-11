cmake_minimum_required(VERSION 3.1.0)
project(carpi_qt)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_STANDARD 11)

option(DEBUG "Compile for debugging environment" OFF)

if(DEBUG)
    add_compile_definitions(CARPI_DEBUG)
endif()

unset(DEBUG CACHE)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Gui Widgets REQUIRED)

add_executable(carpi
        main.cpp

        libs/ModuleLoader/ModuleLoader.h
        libs/ModuleLoader/ModuleLoader.cpp

        libs/Utils/Utils.h
        libs/Utils/Utils.cpp

        libs/Manager/Manager.h
        libs/Manager/Manager.cpp

        libs/UI/MainWindow/MainWindow.h
        libs/UI/MainWindow/MainWindow.cpp

        libs/UI/VTabBar/VTabBar.h
        libs/UI/VTabBar/VTabBar.cpp

        libs/UI/VTabWidget/VTabWidget.h
        libs/UI/VTabWidget/VTabWidget.cpp)

target_link_libraries(carpi
        Qt5::Gui
        Qt5::Widgets)

install(TARGETS carpi
        RUNTIME DESTINATION bin)

install(DIRECTORY DESTINATION /var/lib/carpi/)

install(FILES carpi.desktop
        DESTINATION /root/.config/autostart/)
