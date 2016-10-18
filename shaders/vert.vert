#version 420
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 nml;

out vec3 normalO;

#ifdef VULKAN
    layout(std140, set=0, binding=0) uniform Matrices {
        mat4 pvm;
        mat4 rot;
    } uni;
#else
    layout(std140, binding = 0) uniform Matrices {
        mat4 pvm;
        mat4 rot;
    } uni;
#endif


void main() {
    gl_Position = uni.pvm * vec4(vertex, 1);
    normalO = vec3(uni.rot * vec4(nml, 1));
}
