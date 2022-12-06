#version 410 core

out vec4 FragColor;

in VS_OUT {
    vec3 frag_pos;
    vec2 tex_coords;
    vec3 noise;
    vec3 layer1;
    vec3 layer2;
    vec3 normal;
} fs_in;

uniform float step_value;
uniform vec3 camera_pos;

void main()
{
    vec3 color = vec3(0.0);
    color = step(step_value, fs_in.noise / 2.0 + .5);

    vec3 layer1 = step(step_value, fs_in.layer1 / 2.0 + .5);
    vec3 layer2 = step(step_value, fs_in.layer2 / 2.0 + .5);

    color = mix(vec3(0.1216, 0.3608, 0.6314), vec3(0.1137, 0.4824, 0.7882), layer1);
    float theta = max(0.35, dot(fs_in.normal, normalize(camera_pos - fs_in.frag_pos)));
    vec3 alpha = clamp(layer2 - .0, 0.0, 1.0);
    color = mix(color, vec3(0.3961, 0.7216, 0.9882), alpha * theta);

    FragColor = vec4(color, 1.0);
    // FragColor = vec4(fs_in.normal, 1.0);
}
