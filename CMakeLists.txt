cmake_minimum_required (VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("OculusTest.bin")

#add_subdirectory(external)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
#find_package(SDL2 REQUIRED)
set(SDL2_INCLUDE_DIR /usr/local/include/SDL2)
set(SDL2_LIBRARIES /usr/local/lib/libSDL2.so)
include_directories (
    ${PROJECT_SOURCE_DIR}/OculusTest
    ${PROJECT_SOURCE_DIR}/OculusTest/OVR
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
)
message("inc")
message(${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR})
message("libs")
message(${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${SDL2_LIBRARIES})

add_executable(OculusTestN
    OculusTest/normalMain.cpp
    OculusTest/Scene.cpp
    OculusTest/Engine.cpp
    OculusTest/LegoBrick.cpp
    OculusTest/NormalCamera.cpp
    OculusTest/NormalDisplay.cpp
    OculusTest/NormalRenderingTarget.cpp)

target_link_libraries(OculusTestN
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${SDL2_LIBRARIES}
)

