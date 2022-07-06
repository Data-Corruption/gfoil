#version 330 core
layout (location = 0) in vec2 a_position;
layout (location = 1) in vec3 a_texture_coords;
layout (location = 1) in vec3 a_color;

out vec3 v_texture_coords;
out vec4 v_color;

uniform vec2 u_offset;

void main() {
    v_texture_coords = a_texture_coords;
    v_color = vec4(a_color, 1.0);
    gl_Position = vec4(a_position.x - u_offset.x, a_position.y - u_offset.y, 0.0, 1.0);
}