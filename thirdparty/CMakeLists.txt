include(FetchContent)

FetchContent_Declare(
	OpenCV_contrib
	GIT_REPOSITORY
	https://github.com/opencv/opencv_contrib.git
	GIT_PROGRESS
    ON
	GIT_TAG
	master
	UPDATE_COMMAND
	""
)
FetchContent_GetProperties(OpenCV_contrib)
if (NOT OpenCV_contrib_POPULATED)
    FetchContent_Populate(OpenCV_contrib)
endif ()

add_subdirectory(opencv)
add_subdirectory(FLTK)