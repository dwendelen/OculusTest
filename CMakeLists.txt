cmake_minimum_required (VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -std=c++11)
if(APPLE)
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
endif(APPLE)

project("OculusTest")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OVR QUIET)
find_package(Vulkan QUIET)

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
    ${GLEW_LIBRARY}
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
    message("Found Oculus")
    add_executable(OculusTestO
        ${CORE_SRC}
        src/OculusMain.cpp
        include/OculusCamera.h
        src/OculusCamera.cpp
        include/OculusDisplay.h
        src/OculusDisplay.cpp
        include/OculusRenderingTarget.h
        src/OculusRenderingTarget.cpp
        include/NormalDisplay.h
        src/NormalDisplay.cpp
        include/NormalRenderingTarget.h
        src/NormalRenderingTarget.cpp
        include/Oculus.h
        src/Oculus.cpp
    )
    target_link_libraries(OculusTestO
        ${CORE_LIBS} ${OVR_LIBRARY}
    )
endif(OVR_FOUND)

if(Vulkan_FOUND)
    find_package(X11_XCB REQUIRED)
    message("Found Vulkan")
    include_directories(
        ${Vulkan_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/src/vulkan
    )
    add_executable(OculusV
        ${CORE_SRC}
        src/vulkanMain.cpp
        src/vulkan/VulkanContext.h
        src/vulkan/VulkanContext.cpp
        src/vulkan/VulkanDebug.h
        src/vulkan/VulkanDebug.cpp
        src/vulkan/VulkanDisplay.h
        src/vulkan/VulkanDisplay.cpp
        src/vulkan/VulkanRenderPass.h
        src/vulkan/VulkanRenderPass.cpp
    )
    target_link_libraries(OculusV
        ${CORE_LIBS}
        ${Vulkan_LIBRARIES}
        ${X11_XCB_LIBRARIES}
    )
endif(Vulkan_FOUND)

add_executable(tst test.cpp)
