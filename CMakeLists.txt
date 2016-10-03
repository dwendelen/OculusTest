cmake_minimum_required (VERSION 3.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "./")
#set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "-stdlib=libc++")
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("OculusTest.bin")

#add_subdirectory(external)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
#find_package(SDL2 REQUIRED)
#if(APPLE)
    find_package(SDL2 REQUIRED)
#else(APPLE) 
#    set(SDL2_INCLUDE_DIR /usr/local/include/SDL2)
#    set(SDL2_LIBRARIES /usr/local/lib/libSDL2.so)
#endif(APPLE)
message("gras" ${SDL2_LIBRARY})
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
