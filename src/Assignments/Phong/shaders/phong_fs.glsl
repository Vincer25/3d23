#version 430

#define MAX_POINT_LIGHTS 24

layout(location=0) out vec4 vFragColor;

layout(std140, binding = 0) uniform Modifier {
    vec4 Kd;
    bool use_map_Kd;
} modifier;

layout(std140, binding=3) uniform Light {
    PointLight light[MAX_POINT_LIGHTS];
} p_light;

struct PointLight {
    vec3 position_in_view_space;
    vec3 color;
    vec3 atn;
};

uniform sampler2D map_Kd;
uniform vec3 ambient_light;

in vec2 vertex_texcoords;
in vec3 vertex_coords_in_vs;
in vec3 vertex_normals_in_vs;

void main() {
    vec4 Kd = modifier.Kd;

    if (modifier.use_map_Kd) {
        Kd *= texture(map_Kd, vertex_texcoords);
    }

    vFragColor.a = Kd.a;
    vFragColor.rgb = Kd.rgb;
}