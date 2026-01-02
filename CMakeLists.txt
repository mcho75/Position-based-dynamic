cmake_minimum_required(VERSION 3.16)

project(Position-based-dynamic VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS OpenGLWidgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        simulation/Context.cpp
        simulation/Context.h
        Vec2.h
        widgets/Viewport.cpp
        widgets/Viewport.h
        simulation/Entity.h
        simulation/attributes/Attribute.h
        simulation/attributes/Rigidbody.cpp
        simulation/attributes/Collider.cpp
        simulation/attributes/Sprite.cpp
        simulation/attributes/Collider.h
        simulation/attributes/Rigidbody.h
        simulation/attributes/Sprite.h
        simulation/Entity.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Position-based-dynamic
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Position-based-dynamic APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Position-based-dynamic SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Position-based-dynamic
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Position-based-dynamic PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Position-based-dynamic PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

target_include_directories(Position-based-dynamic
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Position-based-dynamic)
endif()
set_target_properties(Position-based-dynamic PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Position-based-dynamic
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Position-based-dynamic)
endif()
