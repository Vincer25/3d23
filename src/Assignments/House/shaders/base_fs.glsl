#version 410

layout(location=0) out vec4 vFragColor;
layout(location=1) in vec4 my_color;

void main() {
    vFragColor = my_color;
    //vFragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
