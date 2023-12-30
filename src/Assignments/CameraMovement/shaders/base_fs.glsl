#version 410

layout(location=0) out vec4 vFragColor;

in vec3 in_color;

void main() {
    vFragColor.a = 1.0f;
    vFragColor.rgb = in_color;
}
