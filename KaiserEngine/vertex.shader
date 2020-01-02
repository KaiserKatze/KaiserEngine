#version 330 core

uniform mat4 matrix_projection;
uniform mat4 matrix_view;
uniform mat4 matrix_model;

in vec4 in_position;
in vec4 in_color;

out vec4 pass_color;

void main(void)
{
    gl_Position = in_position;
    gl_Position = matrix_projection * matrix_view * matrix_model * in_position;

    pass_Color = in_color;
}
