#version 410 core
out vec4 FragColor;

in vec2 tex_coords;

uniform sampler2D screenTexture;
uniform vec2 resolution;
uniform int effect = 1;
uniform float time;

#define PS1_EFFECT 1
const float steps = 8;

#define COLOR_ABERRATION 2
const float epsilon = 0.02;

#define GRAYSCALE 3

// inspired by https://www.shadertoy.com/view/MslGR8
#define DITHERING_1 4
//note: from https://www.shadertoy.com/view/4djSRW
// This set suits the coords of of 0-1.0 ranges..
#define MOD3 vec3(443.8975,397.2973, 491.1871)

float hash12(vec2 p)
{
    vec3 p3  = fract(vec3(p.xyx) * MOD3);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.x + p3.y) * p3.z);
}
vec3 hash32(vec2 p)
{
    vec3 p3 = fract(vec3(p.xyx) * MOD3);
    p3 += dot(p3, p3.yxz+19.19);
    return fract(vec3((p3.x + p3.y)*p3.z, (p3.x+p3.z)*p3.y, (p3.y+p3.z)*p3.x));
}

float dithering(float brightness, float seed)
{
    const float c0 = 32.0;
    vec3 outcol;
    #if defined( ANIMATED )
    float its = mix( 0.0, 1.0 / c0, brightness + 0.25*sin(0.5*time) );
    #else
    float its = mix( 0.0, 1.0 / c0, brightness );
    #endif

    //note: triangluarly distributed noise, 1.5LSB
    vec2 seed2 = vec2(brightness, seed);
    #if defined( ANIMATED )
    seed2 += fract(time);
    #endif

    #ifdef CHROMATIC
    vec3 rnd = hash32( seed2 ) + hash32(seed2 + 0.59374) - 0.5;
    #else
    vec3 rnd = vec3(hash12( seed2 ) + hash12(seed2 + 0.59374) - 0.5 );
    #endif

    outcol = vec3(its) + rnd/255.0;
    outcol.rgb = floor( outcol.rgb * 255.0 ) / 255.0;
    outcol.rgb *= c0;
    return outcol.r;
}

float easeOutCubic(float x)
{
    return 1.0 - pow(1.0 - x, 3.0);
}
float easeInCubic(float x)
{
    return x * x * x;
}

void main()
{
    vec3 base_color = texture(screenTexture, tex_coords).rgb;
    vec2 uv = gl_FragCoord.xy / resolution;
    vec2 centered_uv = (uv * 2.0) - 1.0;

    if (effect == PS1_EFFECT)
    {
        float intensity = float(int((1.0 - pow(1.0 - base_color.g, 3.0)) * steps)) / steps;
        base_color *= intensity;
    }
    else if (effect == COLOR_ABERRATION)
    {
        float offset = epsilon * pow(length(centered_uv), 3.0);
        base_color.r = texture(screenTexture, tex_coords - vec2(.0, offset)).r;
        base_color.b = texture(screenTexture, tex_coords + vec2(.0, offset)).b;
        base_color.r += texture(screenTexture, tex_coords - vec2(offset, .0)).r;
        base_color.b += texture(screenTexture, tex_coords + vec2(offset, .0)).b;
        base_color.rb *= .5;
    }
    else if (effect == GRAYSCALE)
        base_color = base_color.ggg;
    else if (effect == DITHERING_1)
    {
        base_color = base_color.ggg;
        float dither_factor = dithering(easeInCubic(length(centered_uv / 1.4)), cos(uv.y));
        base_color = base_color * (1.0 - dither_factor);
    }
    else
    {
    }

    // base_color = vec3(centered_uv, 0.0);
    FragColor = vec4(base_color, 1.0);
}