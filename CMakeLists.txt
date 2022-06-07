cmake_minimum_required(VERSION 3.5)

project(az_log_analyzer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Network)

set(PROJECT_SOURCES
        img/img.qrc

        main.cpp

        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

        filetree.h
        filetree.cpp
        filetree.ui

        indextree.h
        indextree.cpp
        indextree.ui

        tabview.h
        tabview.cpp
        tabview.ui

        projectview.h
        projectview.cpp
        projectview.ui

        newprojdialog.h
        newprojdialog.cpp
        newprojdialog.ui

        plaintextview.h
        plaintextview.cpp
        plaintextview.ui

        execcmd.h
        execcmd.cpp
        execcmd.ui

        findview.h
        findview.cpp
        findview.ui

        linenumberarea.h

        projectmgr.h
        projectmgr.cpp

        singleton.h

        myplaintextedit.h
        myplaintextedit.cpp
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(az_log_analyzer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET az_log_analyzer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(az_log_analyzer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(az_log_analyzer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(az_log_analyzer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Network)

set_target_properties(az_log_analyzer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(az_log_analyzer)
endif()
