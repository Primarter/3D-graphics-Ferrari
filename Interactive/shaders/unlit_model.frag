#version 410 core

out vec4 FragColor;

in VS_OUT {
    vec3 frag_pos;
    vec2 tex_coords;
} fs_in;

uniform sampler2D texture_albedo;
uniform vec3 color_factor = vec3(1.0);

void main()
{
    vec4 color = texture(texture_albedo, fs_in.tex_coords).rgba;
    if (color.a < .2)
        discard;

    FragColor = vec4(color.rgb * color_factor, color.a);
}
