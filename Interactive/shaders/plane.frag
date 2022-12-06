#version 410 core

out vec4 FragColor;

in VS_OUT {
    vec3 frag_pos;
    vec2 tex_coords;
    float bump;
} fs_in;

uniform float time;
uniform float step_value;
uniform vec3 camera_pos;
uniform float plane_scale;

const float epsilon = 0.001;

//  Classic Perlin 2D Noise
//  by Stefan Gustavson
//
vec2 fade(vec2 t) {return t*t*t*(t*(t*6.0-15.0)+10.0);}
vec4 permute(vec4 x){return mod(((x*34.0)+1.0)*x, 289.0);}

float cnoise(vec2 P){
    vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
    vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
    Pi = mod(Pi, 289.0); // To avoid truncation effects in permutation
    vec4 ix = Pi.xzxz;
    vec4 iy = Pi.yyww;
    vec4 fx = Pf.xzxz;
    vec4 fy = Pf.yyww;
    vec4 i = permute(permute(ix) + iy);
    vec4 gx = 2.0 * fract(i * 0.0243902439) - 1.0; // 1/41 = 0.024...
    vec4 gy = abs(gx) - 0.5;
    vec4 tx = floor(gx + 0.5);
    gx = gx - tx;
    vec2 g00 = vec2(gx.x,gy.x);
    vec2 g10 = vec2(gx.y,gy.y);
    vec2 g01 = vec2(gx.z,gy.z);
    vec2 g11 = vec2(gx.w,gy.w);
    vec4 norm = 1.79284291400159 - 0.85373472095314 *
        vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11));
    g00 *= norm.x;
    g01 *= norm.y;
    g10 *= norm.z;
    g11 *= norm.w;
    float n00 = dot(g00, vec2(fx.x, fy.x));
    float n10 = dot(g10, vec2(fx.y, fy.y));
    float n01 = dot(g01, vec2(fx.z, fy.z));
    float n11 = dot(g11, vec2(fx.w, fy.w));
    vec2 fade_xy = fade(Pf.xy);
    vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
    float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
    return 2.3 * n_xy;
}

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main()
{
    //normals
    vec2 offset = vec2(sin(time) * .2, cos(time) * .6) / 2.0;
    vec2 p1 = fs_in.tex_coords * (10.0 + plane_scale) + offset;
    vec2 p2 = fs_in.tex_coords * (10.0 + plane_scale) + offset + vec2(epsilon, 0);
    vec2 p3 = fs_in.tex_coords * (10.0 + plane_scale) + offset + vec2(0, epsilon);

    float noise_value1 = cnoise(p1);
    float noise_value2 = cnoise(p2);
    float noise_value3 = cnoise(p3);

    vec3 pp1 = (vec3(p1.x, noise_value1, p1.y));
    vec3 pp2 = (vec3(p2.x, noise_value2, p2.y));
    vec3 pp3 = (vec3(p3.x, noise_value3, p3.y));

    vec3 normal = normalize(cross(pp2 - pp1, pp3 - pp1));

    // layers
    // vec2 layer1_offset = vec2(sin(time) * .15, cos(time) * .6) / 3.0;
    vec3 layer1 = vec3(fs_in.bump);//vec3(cnoise(fs_in.tex_coords * (10.0 + plane_scale) - layer1_offset));
    vec2 layer2_offset = vec2(sin(time) * .4, cos(time) * .8) + 10.0;
    vec3 layer2 = vec3(cnoise(fs_in.tex_coords * (10.0 + plane_scale) + layer2_offset));

    // color
    vec3 color = vec3(0.0);
    // color = step(step_value, fs_in.noise / 2.0 + .5);

    layer1 = step(step_value, layer1 / 2.0 + .5);
    layer2 = step(step_value, layer2 / 2.0 + .5);

    color = mix(vec3(0.1216, 0.3608, 0.6314), vec3(0.1137, 0.4824, 0.7882), layer1);
    float theta = max(0.0, dot(normal, normalize(camera_pos - fs_in.frag_pos)));
    vec3 alpha = clamp(layer2 - .0, 0.0, 1.0);
    color = mix(color, vec3(0.3961, 0.7216, 0.9882), alpha + (alpha * theta));

    FragColor = vec4(color, 1.0);
    // FragColor = vec4(vec3(theta), 1.0);
    // FragColor = vec4(vec3(fs_in.frag_pos.y + 1.0), 1.0);
    // FragColor = vec4(normal, 1.0);
}
