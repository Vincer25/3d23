#version 410

layout(location=0) in vec4 vertex_position;
layout(location=1) in vec3 vertex_color;

out vec3 in_color;

layout(std140) uniform Transformations {
    mat4 PVM;
};

void main() {
    in_color = vertex_color;
    gl_Position =  PVM * vertex_position;
}
