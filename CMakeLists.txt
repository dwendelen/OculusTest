cmake_minimum_required (VERSION 3.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "./")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -std=c++11)

project("OculusTest.bin")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

include_directories (
    ${PROJECT_SOURCE_DIR}/OculusTest
    ${PROJECT_SOURCE_DIR}/OculusTest/OVR
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
)

add_executable(OculusTestN
    OculusTest/normalMain.cpp
    OculusTest/Scene.cpp
    OculusTest/Engine.cpp
    OculusTest/LegoBrick.cpp
    OculusTest/NormalCamera.cpp
    OculusTest/NormalDisplay.cpp
    OculusTest/MemoryManager.cpp
    OculusTest/InputManager.cpp
    OculusTest/NormalRenderingTarget.cpp)

target_link_libraries(OculusTestN
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${SDL2_LIBRARY}
)

#add_executable(testEx test.cpp)
