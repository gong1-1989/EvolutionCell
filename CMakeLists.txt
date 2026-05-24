cmake_minimum_required(VERSION 3.16)

project(EvolutionCell VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui)
include_directories(${PROJECT_SOURCE_DIR}/src)
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(EvolutionCell
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        Res.qrc
        src/Utils/collisionutil.h src/Utils/collisionutil.cpp
        src/Utils/MathAToB.h
        src/Utils/gamecanvas.h src/Utils/gamecanvas.cpp
        src/Core/gameglobal.h src/Core/gameglobal.cpp
        src/Utils/randomutil.h src/Utils/randomutil.cpp
        src/Cell/playercell.h src/Cell/playercell.cpp
        src/Cell/monstercell.h src/Cell/monstercell.cpp
        src/Core/gamemanager.h src/Core/gamemanager.cpp
        src/Core/gamerender.h src/Core/gamerender.cpp
        src/Cell/gene.h src/Cell/gene.cpp
        src/Utils/configreader.h src/Utils/configreader.cpp
        src/Utils/savemanager.h src/Utils/savemanager.cpp
        src/cell/symbiosiscell.h src/cell/symbiosiscell.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET EvolutionCell APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(EvolutionCell SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(EvolutionCell
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(EvolutionCell PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.EvolutionCell)
endif()
set_target_properties(EvolutionCell PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS EvolutionCell
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(EvolutionCell)
endif()
