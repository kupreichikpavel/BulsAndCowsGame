cmake_minimum_required(VERSION 3.21)
project(untitled3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(untitled3
        main.cpp
        window/mainwindow.cpp
        window/mainwindow.h
        game/game.cpp
        game/game.h
        recorddialog/recordsdialog.cpp
        recorddialog/recordsdialog.h
        namedialog/namedialog.cpp
        namedialog/namedialog.h
)

target_link_libraries(untitled3 PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)