#version 410 core
layout (location = 0) in vec3 in_vertex;
layout (location = 1) in vec3 in_normals;
// layout (location = 2) in vec3 in_tangents;
layout (location = 2) in vec2 in_uv;


out VS_OUT {
    vec3 frag_pos;
    vec2 tex_coords;
    vec3 normal;
} vs_out;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

void main()
{
    vec4 pos = model_matrix * vec4(in_vertex, 1.0);

    vs_out.tex_coords = in_uv;
    vs_out.frag_pos = pos.xyz;
    vs_out.normal = mat3(model_matrix) * in_normals;

    gl_Position =  proj_matrix * view_matrix * pos;
}