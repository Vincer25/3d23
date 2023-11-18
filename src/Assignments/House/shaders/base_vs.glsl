#version 410

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in vec4 color_in;
layout(location=1) out vec4 my_color;

void main() {
    gl_Position = a_vertex_position;
    my_color = color_in;
}
