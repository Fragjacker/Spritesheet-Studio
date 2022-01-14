cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(Spritesheet-Studio_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/resources/icon.rc" )

set(OpenCV_STATIC 1)
set(FLTK_STATIC 1)

find_package(FLTK REQUIRED NO_MODULE)
find_package(OpenCV REQUIRED)

set(SRCS 
    main.cpp 
    opencv_image.cpp
    dds_img_reader.cpp
    file_IO.cpp)

include(${FLTK_USE_FILE})

# Additional safety linking for FLTK because it uses no Cmake module.
include_directories(${FLTK_INCLUDE_DIRS}) 
link_directories(${FLTK_LIBRARY_DIRS}) 
add_definitions(${FLTK_DEFINITIONS})

if (APPLE)
    # create bundle on apple compiles
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRCS} )
    # Set a custom plist file for the app bundle - needed for Mac OS Retina display
    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
else()
    # the WIN32 is needed for Windows in order for it to look for WinMain
    # instead of the main function. This is ignored on other systems,
    # so it works on all platforms
    add_executable(${PROJECT_NAME} WIN32 ${SRCS} ${APP_ICON_RESOURCE_WINDOWS})
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS} fltk)
#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR} ${EXTRA_INCLUDES})