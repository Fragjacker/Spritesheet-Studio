cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(Spritesheet-Studio_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/icon.rc" )

# hack for buggy CMake's FindwxWidgets script
if (DEFINED ENV_WX_CONFIG)
    set (ENV{WX_CONFIG} ${ENV_WX_CONFIG})
endif()

set(wxWidgets_USE_STATIC 1)
set(OpenCV_STATIC 1)

find_package(wxWidgets REQUIRED)
find_package(OpenCV REQUIRED)

set(SRCS main.cpp)

include(${wxWidgets_USE_FILE})
#include(${OpenCV_LIBRARIES})

if (APPLE)
    # create bundle on apple compiles
    add_executable(Spritesheet-Studio MACOSX_BUNDLE ${SRCS} )
    # Set a custom plist file for the app bundle - needed for Mac OS Retina display
    set_target_properties(Spritesheet-Studio PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
else()
    # the WIN32 is needed for Windows in order for it to look for WinMain
    # instead of the main function. This is ignored on other systems,
    # so it works on all platforms
    add_executable(Spritesheet-Studio WIN32 ${SRCS} ${APP_ICON_RESOURCE_WINDOWS})
endif()

target_link_libraries(Spritesheet-Studio PRIVATE ${OpenCV_LIBRARIES} ${wxWidgets_LIBRARIES})