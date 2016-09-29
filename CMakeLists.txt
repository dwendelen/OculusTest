cmake_minimum_required (VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("OculusTest.bin")

include_directories (
    ${PROJECT_SOURCE_DIR}/OculusTest
)

add_executable(OculusTest
    OculusTest/normalMain.cpp
    OculusTest/Scene.cpp
    OculusTest/Engine.cpp
    OculusTest/LegoBrick.cpp
    OculusTest/NormalCamera.cpp
    OculusTest/NormalDisplay.cpp
    OculusTest/NormalRenderingTarget.cpp)
