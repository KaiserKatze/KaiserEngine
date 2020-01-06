#version 330 core

uniform sampler2D texture_diffuse;

in vec4 pass_color;
in vec2 pass_texture_coord;

out vec4 out_Color;

void main(void)
{
    out_Color = pass_color;
    out_Color = texture2D(texture_diffuse, pass_texture_coord);
}
