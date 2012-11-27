set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(MinimalModule)
add_subdirectory(MyModule)
add_subdirectory(ArmPoint)
add_subdirectory(EyeDir)
add_subdirectory(MyClosest)
add_subdirectory(PrioMod)
add_subdirectory(HeadPose)
add_subdirectory(TickCounter)
add_subdirectory(PrioModHead)

add_subdirectory(ColPick)

add_subdirectory(Nikaros)
add_subdirectory(OpenNi)
add_subdirectory(MarkerMemory)

set(SOURCE ${SOURCE} PARENT_SCOPE)
set(MODULES_INCLUDS ${MODULES_INCLUDS} PARENT_SCOPE)
set(MODULES_LIBS ${MODULES_LIBS} PARENT_SCOPE)
