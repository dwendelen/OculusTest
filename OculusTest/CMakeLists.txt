cmake_minimum_required(VERSION 3.6)

project("OculusTest")

add_executable(oculus
    Engine.cpp
    NormalCamera.cpp
    NormalDisplay.cpp
    normalMain.cpp
    NormalRenderingTarget.cpp
    Oculus.cpp
    OculusCamera.cpp
    OculusRenderingTarget.cpp
    Scene.cpp
)