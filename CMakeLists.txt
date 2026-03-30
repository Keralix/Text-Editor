cmake_minimum_required(VERSION 3.16)
project(TextEd)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(TextEd
    main.cpp
    Editor.cpp
    Editor.h
    Text.h
    Text.cpp
)

target_link_libraries(TextEd PRIVATE Qt6::Widgets)