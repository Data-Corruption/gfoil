#version 330 core

out vec4 f_color;

in vec3 v_texture_coords;
in vec3 v_color;

uniform sampler2D u_texture;

void main() {
    float mask = texture(u_texture, vec2(v_texture_coords.x, v_texture_coords.y)).x;
    f_color = vec4(mask * v_color, mask);
}