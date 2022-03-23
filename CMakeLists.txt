cmake_minimum_required(VERSION 3.5)

project(OpenCalibGui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

find_package(OpenCV REQUIRED)
#set(OpenCV_LIBS
#    F:/library/opencv/build/x64/vc15/lib/opencv_world342.lib)

#set(OpenCV_INCLUDE_DIRS
#    F:/library/opencv/build/include
#    )
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include)

set(PROJECT_SOURCES
        main.cpp
        ${PROJECT_SOURCE_DIR}/include/mainwindow.h
        ${PROJECT_SOURCE_DIR}/include/thumbnail.h
        ${PROJECT_SOURCE_DIR}/include/calibration.h

        ${PROJECT_SOURCE_DIR}/src/mainwindow.cpp
        ${PROJECT_SOURCE_DIR}/src/thumbnail.cpp
        ${PROJECT_SOURCE_DIR}/src/calibration.cpp

        ${PROJECT_SOURCE_DIR}/src/mainwindow.ui
        ${PROJECT_SOURCE_DIR}/src/thumbnail.ui
)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets  ${OpenCV_LIBS})



