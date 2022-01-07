# CMakeList.txt: CMake-Projekt for "Spritesheet-Studio". Add sources and libraries here.

cmake_minimum_required (VERSION 3.8)

# Add the App Icon
set( APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/icon.rc" )

# Add OpenCV
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Add wxWidgets
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

# Add executable linked with all the external libraries
add_executable ( Spritesheet-Studio "main.cpp" "main.h" ${APP_ICON_RESOURCE_WINDOWS} )
target_link_libraries( Spritesheet-Studio ${OpenCV_LIBS} ${wxWidgets_LIBRARIES} )


# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.
