# CMakeList.txt: CMake-Projekt für "Spritesheet-Studio". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)

# Fügen Sie der ausführbaren Datei dieses Projekts eine Quelle hinzu.
set( APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/icon.rc" )

# OpenCV hinzufügen
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable ( Spritesheet-Studio "main.cpp" "main.h" ${APP_ICON_RESOURCE_WINDOWS} )
target_link_libraries( Spritesheet-Studio ${OpenCV_LIBS} )

# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.
