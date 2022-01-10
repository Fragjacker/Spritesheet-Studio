# check if wxWidgets is already installed in the system - using CMake's built in script FindwxWidgets
find_package(FLTK QUIET)

if (FLTK_FOUND)
    message(STATUS "Found preinstalled FLTK libraries at ${FLTK_LIBRARIES}")
    add_library(FLTK_external INTERFACE)
else()
    message(STATUS "Preinstalled FLTK not found.")
    message(STATUS "Will download and install FLTK in ${STAGED_INSTALL_PREFIX}")
    
    include(ExternalProject)
    ExternalProject_Add(FLTK_external
        GIT_REPOSITORY
        https://github.com/fltk/fltk.git
        GIT_PROGRESS
        TRUE
        GIT_TAG
        release-1.3.8
        UPDATE_COMMAND
        ""
        CMAKE_ARGS
        -G "Visual Studio 17 2022"
        -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
        -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
        -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
        -DOPTION_BUILD_SHARED_LIBS=OFF
        -DBUILD_WITH_STATIC_CRT=OFF
        -DFLTK_BUILD_TEST=OFF
        CMAKE_CACHE_ARGS
        -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
        TEST_AFTER_INSTALL
        0
        DOWNLOAD_NO_PROGRESS
        1
        LOG_CONFIGURE
        1
        LOG_BUILD
        1
        LOG_INSTALL
        1
        )


    set(FLTK_DIR ${STAGED_INSTALL_PREFIX}/Cmake)
    file(TO_NATIVE_PATH "${FLTK_DIR}" FLTK_DIR)
    set(FLTK_DIR ${FLTK_DIR} CACHE INTERNAL "FLTK installation dir")

    set(FLTK_INCLUDE_DIR ${STAGED_INSTALL_PREFIX}/include)
    file(TO_NATIVE_PATH "${FLTK_INCLUDE_DIR}" FLTK_INCLUDE_DIR)
    set(FLTK_INCLUDE_DIR ${FLTK_INCLUDE_DIR} CACHE INTERNAL "FLTK include dir")

    set(FLTK_LIB_DIR ${STAGED_INSTALL_PREFIX}/lib)
    file(TO_NATIVE_PATH "${FLTK_LIB_DIR}" FLTK_LIB_DIR)
    set(FLTK_LIB_DIR ${FLTK_LIB_DIR} CACHE INTERNAL "FLTK include dir")
endif()