cmake_minimum_required(VERSION 3.5)

project(IT_Connect VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Sql Widgets OpenGL)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Sql Widgets OpenGL)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(IT_Connect
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        InitDataBase.h
        User.h User.cpp
        Project.h Project.cpp
        WgtWorker.h WgtWorker.cpp WgtWorker.ui
        WgtProject.h WgtProject.cpp WgtProject.ui
        DlgSelSkills.h DlgSelSkills.cpp DlgSelSkills.ui
        ItemSkill.h ItemSkill.cpp
        DlgSelProject.h
        DlgSelProject.cpp
        DlgSelProject.ui


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET IT_Connect APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(IT_Connect SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(IT_Connect
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(IT_Connect PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.IT_Connect)
endif()
set_target_properties(IT_Connect PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Resources:
set(resource_files
    "icons/icons.qrc"
    "themes/themes.qrc"
)

qt_add_resources(IT_Connect "IT_Connect"
    PREFIX
        "/"
    FILES
        ${resource_files}
)

include(GNUInstallDirs)
install(TARGETS IT_Connect
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(IT_Connect)
endif()
