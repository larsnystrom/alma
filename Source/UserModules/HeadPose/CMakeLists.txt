set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCE
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/HeadPose.cc
	${CMAKE_CURRENT_SOURCE_DIR}/CRForestEstimator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CRTree.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/gl_camera.cpp
	PARENT_SCOPE
)

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)
#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)


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
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
    OpenNI
    opencv_core 
    opencv_highgui 
    opencv_imgproc
    PARENT_SCOPE
)
