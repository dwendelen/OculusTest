#version 420
#extension GL_ARB_separate_shader_objects : enable

in vec3 normalO;

layout(location=0) out vec4 color;

layout(std140, /*set=0,*/ binding=1) uniform ColorUniform {
    vec4 baseColor;
} uni;
void main() {
    color.xyz = (0.3 + (1.0+normalO.x)*0.25) * uni.baseColor.xyz;
    color.w = uni.baseColor.w;
}
