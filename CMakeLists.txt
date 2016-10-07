cmake_minimum_required (VERSION 3.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS -std=c++11)
if(APPLE)
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
endif(APPLE)

project("OculusTest")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OVR QUIET)

include_directories (
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/OVR
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
)

set(CORE_SRC
    include/Scene.h
    src/Scene.cpp
    include/Engine.h
    src/Engine.cpp
    include/LegoBrick.h
    src/LegoBrick.cpp
    include/MemoryManager.h
    src/MemoryManager.cpp
    include/InputManager.h
    src/InputManager.cpp
    include/ModelInstance.h
    src/ModelInstance.cpp
    include/Camera.h
    include/Display.h
    include/RenderingTarget.h
)
set(CORE_LIBS
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${SDL2_LIBRARY}
)

add_executable(OculusTestN
    ${CORE_SRC}
    src/normalMain.cpp
    include/NormalCamera.h
    src/NormalCamera.cpp
    include/NormalDisplay.h
    src/NormalDisplay.cpp
    include/NormalRenderingTarget.h
    src/NormalRenderingTarget.cpp)

target_link_libraries(OculusTestN
    ${CORE_LIBS}
)

if(OVR_FOUND)
    add_executable(OculusTestO
        ${CORE_SRC}
        OculusTest/oculusMain.cpp
        include/OculusCamera.h
        OculusTest/OculusCamera.cpp
        include/OculusDisplay.h
        OculusTest/OculusDisplay.cpp
        include/OculusRenderingTarget.h
        OculusTest/OculusRenderingTarget.cpp
        include/NormalDisplay.h
        src/NormalDisplay.cpp
        include/NormalRenderingTarget.h
        src/NormalRenderingTarget.cpp
        OculusTest/Oculus.cpp
    )
    target_link_libraries(OculusTestO
        ${CORE_LIBS} ${OVR_LIBRARY}
    )
endif(OVR_FOUND)

add_executable(tst test.cpp)
