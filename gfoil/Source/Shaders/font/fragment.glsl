#version 330 core

out vec4 f_color;

in vec3 v_texture_coords;
in vec4 v_color;

uniform sampler2D u_texture;

void main() {
    f_color = texture(u_texture, vec2(v_texture_coords.x, v_texture_coords.y)) * v_color;
}