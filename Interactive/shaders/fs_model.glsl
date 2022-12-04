#version 410 core

out vec4 FragColor;

in VS_OUT {
    vec3 frag_pos;
    vec2 tex_coords;
    vec3 tangent_light_pos;
    vec3 tangent_view_pos;
    vec3 tangent_frag_pos;
} fs_in;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normals;
uniform sampler2D texture_metallic_roughness;

const vec3 sun = -normalize(vec3(.2, -.8, -.5));
const vec3 sun_color = vec3(1.0, .5, .3);
const float ambient_strength = .15;

void main()
{
    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(texture_normals, fs_in.tex_coords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // get diffuse color
    vec4 color = texture(texture_diffuse, fs_in.tex_coords).rgba;
    // ambient
    vec3 ambient = 0.1 * color.rgb;
    // diffuse
    vec3 light_dir = normalize(fs_in.tangent_light_pos - fs_in.tangent_frag_pos);
    float diff = max(dot(light_dir, normal), 0.0);
    vec3 diffuse = diff * color.rgb;
    // specular
    vec3 view_dir = normalize(fs_in.tangent_view_pos - fs_in.tangent_frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    vec3 halfway_dir = normalize(light_dir + view_dir);  
    float spec = pow(max(dot(normal, halfway_dir), 0.0), 32.0 * (texture(texture_metallic_roughness, fs_in.tex_coords).r));

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, color.a);
}

// void main(void){
//   //color = vec4(1.0,1.0,1.0,1.0);
//   //color = vec4(fs_in.normals, 1.0);
//   vec3 ambient = ambient_strength * sun_color;
//   float theta = max(dot(sun, fs_in.normals), 0);

//   vec4 tex_color = texture(texture_diffuse, fs_in.uv).rgba;

//   vec3 color = tex_color.rgb * theta;
//   // vec3 color = ambient + tex_color.rgb * theta;
//   FragColor = vec4(color, tex_color.a);
//   // color = vec4((fs_in.normals+1.0)/2.0, 1.0);
// }
