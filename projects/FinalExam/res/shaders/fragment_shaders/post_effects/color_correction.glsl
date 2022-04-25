#version 430

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec3 outColor;


uniform layout(binding = 0) sampler2D s_Image;
uniform layout(binding = 1) sampler3D s_Lut;
uniform layout(binding = 2) sampler3D s_Lut2;

uniform float u_Strength;
uniform float u_Choice;

void main() {
    vec3 color = texture(s_Image, inUV).rgb;
    float test = u_Choice;
    if(test == 0)
    {
        outColor = mix(color, texture(s_Lut, color).rgb, clamp(u_Strength, 0, 1));
    }
    else if(test == 1)
    {
        outColor = mix(color, texture(s_Lut2, color).rgb, clamp(u_Strength, 0, 1));
    }
    
    
    
    
    
    
}


