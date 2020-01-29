#version 330 core

uniform mat4 matrix_projection;
uniform mat4 matrix_view;
uniform mat4 matrix_model;

in vec4 in_position;
in vec4 in_color;
in vec2 in_texture_coord;

out vec4 pass_color;
out vec2 pass_texture_coord;

void main(void)
{
    gl_Position = in_position;
    gl_Position = matrix_projection * matrix_view * matrix_model * in_position;

    pass_color = in_color;
    pass_texture_coord = in_texture_coord;
}
