set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCE
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/Nikaros.cc
	PARENT_SCOPE
)


#modify according to your opencv installation
SET( OPENCV_INCLUDES /usr/local/include/opencv ) 
SET( OPENCV_LIB /usr/local/lib ) 

#modify according to your OpenNI installation
SET( OPENNI_INCLUDES /usr/include/ni ) 
SET( OPENNI_LIB /usr/bin ) 

SET(MODULES_INCLUDS
    ${MODULES_INCLUDS}
    ${OPENCV_INCLUDES}
    ${OPENNI_INCLUDES}
    PARENT_SCOPE
)
SET(MODULES_LIBS_DIR
    ${MODULES_LIBS_DIR}
    ${OPENCV_LIB}
    ${OPENNI_LIB}
    PARENT_SCOPE
)

SET(MODULES_LIBS
    ${MODULES_LIBS}
    OpenNI
    opencv_core 
    opencv_highgui 
    opencv_imgproc
    PARENT_SCOPE
)
