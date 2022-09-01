#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_texture_coords;
layout (location = 2) in vec3 a_color;

out vec3 v_texture_coords;
out vec3 v_color;

uniform mat4 u_view;
uniform mat4 u_transform;
uniform mat4 u_projection;

void main() {
    v_texture_coords = a_texture_coords;
    v_color = a_color;
    gl_Position = u_projection * u_view * u_transform * vec4(a_position, 1.0);
}