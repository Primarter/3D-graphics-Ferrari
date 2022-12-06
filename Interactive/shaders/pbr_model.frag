#version 410 core

out vec4 FragColor;

in VS_OUT {
    vec3 frag_pos;
    vec2 tex_coords;
    vec3 normal;
} fs_in;

// material parameters
// uniform sampler2D albedoMap;
// uniform sampler2D normalMap;
// uniform sampler2D metallicMap;
// uniform sampler2D roughnessMap;
// uniform sampler2D aoMap;

uniform sampler2D texture_albedo;
uniform bool has_albedo;
uniform sampler2D texture_normals;
uniform bool has_normals;
uniform sampler2D texture_metallic_roughness;
uniform bool has_metallic_roughness;
uniform sampler2D texture_ambient_occlusion;
uniform bool has_ambient_occlusion;

// lights
const int NB_LIGHTS = 1;
uniform vec3 light_pos[1] = {vec3(1.0, 1.0, 1.0)};
// uniform vec3 light_pos[1] = {-vec3(.2, -.8, -.5)};
uniform vec3 light_colors[1];// = {vec3(1.0, 1.0, 1.0)};
uniform float light_powers[1] = {3.0};

uniform vec3 camera_pos;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal
// mapping the usual way for performance anways; I do plan make a note of this
// technique somewhere later in the normal mapping tutorial.
vec3 get_normal_from_map()
{
    vec3 tangent_normal = texture(texture_normals, fs_in.tex_coords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fs_in.frag_pos);
    vec3 Q2  = dFdy(fs_in.frag_pos);
    vec2 st1 = dFdx(fs_in.tex_coords);
    vec2 st2 = dFdy(fs_in.tex_coords);

    vec3 N   = normalize(fs_in.normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangent_normal);
}
// ----------------------------------------------------------------------------
float distribution_GGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float geometry_schlick_GGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float geometry_smith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometry_schlick_GGX(NdotV, roughness);
    float ggx1 = geometry_schlick_GGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnel_schlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
void main()
{
    vec4 base_color         = has_albedo ? texture(texture_albedo, fs_in.tex_coords).rgba : vec4(1.0, 0.0, 1.0, 1.0);
    vec3 albedo             = pow(base_color.rgb, vec3(2.2));
    vec2 metallic_roughness = has_metallic_roughness ? texture(texture_metallic_roughness, fs_in.tex_coords).gb : vec2(.5);
    float roughness         = metallic_roughness.x;//texture(roughnessMap, fs_in.tex_coords).r;
    float metallic          = metallic_roughness.y;//texture(metallicMap, fs_in.tex_coords).r;
    float ao                = has_ambient_occlusion ? texture(texture_ambient_occlusion, fs_in.tex_coords).r : 1.0;

    vec3 N = get_normal_from_map();
    vec3 V = normalize(camera_pos - fs_in.frag_pos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < NB_LIGHTS; ++i)
    {
        // calculate per-light radiance
        vec3 L = normalize(light_pos[i] - fs_in.frag_pos);
        vec3 H = normalize(V + L);
        float distance = length(light_pos[i] - fs_in.frag_pos);
        float attenuation = (1.0 / (distance * distance)) * light_powers[i];
        vec3 radiance = light_colors[i] * attenuation;

        // Cook-Torrance BRDF
        float NDF = distribution_GGX(N, H, roughness);
        float G   = geometry_smith(N, V, L, roughness);
        vec3 F    = fresnel_schlick(max(dot(H, V), 0.0), F0);

        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;

        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }

    // ambient lighting (note that the next IBL tutorial will replace
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, base_color.a);
}