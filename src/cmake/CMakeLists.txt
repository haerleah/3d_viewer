cmake_minimum_required(VERSION 3.5)

project(3D_Viewer VERSION 2.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
find_package(glm CONFIG REQUIRED)

set(PROJECT_SOURCES
        ../model/s21_obj_loader.cpp
        ../model/s21_obj_loader.h
        ../model/s21_model_facade.cpp
        ../model/s21_model_facade.h
        ../model/s21_model.cpp
        ../model/s21_model.h
        ../model/s21_obj_loader.h
        ../model/s21_transformation_strategy.cpp
        ../model/s21_transformation_strategy.h
        ../controller/s21_controller.cpp
        ../controller/s21_controller.h
        ../view/s21_gif_recorder.h
        ../view/s21_gif_recorder.cpp
        ../view/s21_openGL_widget.h
        ../view/s21_mainwindow.cpp
        ../view/s21_mainwindow.h
        ../view/s21_openGL_widget.cpp
        ../view/s21_error_dialog.h
        ../view/s21_error_dialog.cpp
        ../main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

qt_add_executable(3D_Viewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ../view/shaders.qrc
        ../view/s21_mainwindow.ui
        ../view/s21_error_dialog.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET opengl APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(3D_Viewer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(3D_Viewer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(3D_Viewer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(3D_Viewer PRIVATE Qt6::OpenGLWidgets)
target_link_libraries(3D_Viewer PRIVATE glm::glm)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.3D_Viewer)
endif()
set_target_properties(3D_Viewer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS 3D_Viewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(3D_Viewer)
endif()
