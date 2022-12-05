#version 410 core

out vec4 FragColor;

in VS_OUT {
    vec3 frag_pos;
    vec2 tex_coords;
} fs_in;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normals;
uniform sampler2D texture_metallic_roughness;

void main()
{
    vec4 color = texture(texture_diffuse, fs_in.tex_coords).rgba;

    FragColor = color;
}
