cmake_minimum_required(VERSION 3.9)
project(qt_cv_cap)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC  ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Multimedia)
find_package(Qt5MultimediaWidgets)

find_package(OpenCV REQUIRED)

set(SOURCE_FILES
        main.cpp cv_camera.cpp cv_camera.h)

add_executable(qt_cv_cap ${SOURCE_FILES})

target_link_libraries(qt_cv_cap
        ${OpenCV_LIBRARIES}
        Qt5::Core
        Qt5::Widgets
        Qt5::Multimedia
        Qt5::MultimediaWidgets)