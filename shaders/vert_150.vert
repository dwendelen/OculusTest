#version 150
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 nml;

out vec3 normalO;

layout(std140) uniform Matrices {
    mat4 pvm;
    mat4 rot;
} uni;

void main() {
    gl_Position = uni.pvm * vec4(vertex, 1);
    normalO = vec3(uni.rot * vec4(nml, 1));
}
