set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCE
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/HeadPose.cc
	${CMAKE_CURRENT_SOURCE_DIR}/CRForestEstimator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CRTree.cpp
	PARENT_SCOPE
)

#modify according to your opencv installation
SET( OPENCV_INCLUDES /usr/local/include/opencv ) 
SET( OPENCV_LIB /usr/local/lib ) 


SET(MODULES_INCLUDS
    ${MODULES_INCLUDS}
    ${OPENCV_INCLUDES}
    PARENT_SCOPE
)
SET(MODULES_LIBS_DIR
    ${MODULES_LIBS_DIR}
    ${OPENCV_LIB}
    PARENT_SCOPE
)

SET(MODULES_LIBS
    ${MODULES_LIBS}
    opencv_core 
    opencv_highgui 
    opencv_imgproc
    PARENT_SCOPE
)
