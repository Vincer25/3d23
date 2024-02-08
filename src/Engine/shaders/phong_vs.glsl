#version 430

layout(location=0) in vec4 a_vertex_position;
layout(location=1) in vec2 a_vertex_texcoords;
layout(location=5) in vec3 a_vertex_normal;

layout(std140, binding = 1) uniform Transformations {
    mat4 PVM;
};

layout(std140, binding=2) uniform Matrices {
    mat4 VM;
    mat3 N;
};

out vec4 vertex_position;
out vec3 vertex_normal;
out vec2 vertex_texcoords;

out vec3 vertex_coords_in_vs;
out vec3 vertex_normals_in_vs;

void main() {
    vertex_normal = a_vertex_normal;
    vertex_position = a_vertex_position;
    vertex_texcoords = a_vertex_texcoords;
    
    vec4 vertex_coords_in_vs4 = VM * a_vertex_position;
    vertex_coords_in_vs = vertex_coords_in_vs4.xyz;

    vertex_normals_in_vs = normalize(N * a_vertex_normal);
    gl_Position =  PVM * a_vertex_position;
}