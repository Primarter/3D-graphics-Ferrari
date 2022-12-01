#version 410 core

out vec4 FragColor;

in VS_OUT
{
    vec3 vertex;
    vec3 normals;
    vec2 uv;
} fs_in;

uniform sampler2D tex;
uniform mat4 model_matrix;

const vec3 sun = -normalize(vec3(.2, -.8, -.5));
const vec3 sun_color = vec3(1.0, .5, .3);
const float ambient_strength = .15;

void main(void){
  //color = vec4(1.0,1.0,1.0,1.0);
  //color = vec4(fs_in.normals, 1.0);
  vec3 ambient = ambient_strength * sun_color;
  float theta = max(dot(sun, fs_in.normals), 0);

  vec3 color = ambient + texture(tex, fs_in.uv).rgb * theta;
  FragColor = vec4(color, 1.0);
  // color = vec4((fs_in.normals+1.0)/2.0, 1.0);
}
