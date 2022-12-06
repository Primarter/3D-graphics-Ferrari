#version 410 core

out vec4 FragColor;

in VS_OUT {
    vec3 frag_pos;
    vec2 tex_coords;
} fs_in;

uniform sampler2D texture_albedo;

void main()
{
    vec4 color = texture(texture_albedo, fs_in.tex_coords).rgba;
    if (color.a < .2)
        discard;

    FragColor = color;
}
