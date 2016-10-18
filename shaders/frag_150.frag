#version 150
#extension GL_ARB_explicit_attrib_location : enable

in vec3 normalO;

layout(location=0) out vec4 color;

layout(std140) uniform ColorUniform {
    vec4 baseColor;
} uni;

void main() {
    color.xyz = (0.3 + (1.0+normalO.x)*0.25) * uni.baseColor.xyz;
    color.w = uni.baseColor.w;
}
