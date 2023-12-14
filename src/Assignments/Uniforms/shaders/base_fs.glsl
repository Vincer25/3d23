#version 410

layout(location=0) out vec4 vFragColor;

in vec3 in_color;

layout(std140) uniform Modifier {
    float strength;
    vec3  color;
};

void main() {
    vFragColor.a = 1.0f;
    vFragColor.rgb = strength*color*in_color.rgb;
}
