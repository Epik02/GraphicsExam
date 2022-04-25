#version 430

layout(location = 0) in vec2 inUV;
layout(location = 0) out vec4 outColor;

uniform layout(binding = 0) sampler2D s_Albedo;
uniform layout(binding = 1) sampler2D s_NormalsMetallic;
uniform layout(binding = 2) sampler2D s_DiffuseAccumulation;
uniform layout(binding = 3) sampler2D s_SpecularAccumulation;
uniform layout(binding = 4) sampler2D s_Emissive;

//testing
uniform layout(binding = 11) sampler2D testChoice;

#include "../fragments/frame_uniforms.glsl"
#include "../fragments/color_correction.glsl"
#include "../fragments/multiple_point_lights.glsl"

void main() {
    vec3 albedo = texture(s_Albedo, inUV).rgb;
    vec3 diffuse = texture(s_DiffuseAccumulation, inUV).rgb;
    vec3 specular = texture(s_SpecularAccumulation, inUV).rgb;
    vec4 emissive = texture(s_Emissive, inUV);

    vec3 choiceSelection = texture(testChoice, inUV).rgb;

    if(choiceSelection.x > 0) //r value // ambient
    {
        outColor = vec4(albedo * (diffuse + specular + (emissive.rgb * emissive.a)), 1.0); //to be changed
    }
    else if(choiceSelection.y > 0) //g value //diffuse 
    {
        outColor = vec4(albedo * (diffuse + (emissive.rgb * emissive.a)), 1.0); 
    }
    else if(choiceSelection.z > 0) //b value //specular
    {
        outColor = vec4(albedo * (specular + (emissive.rgb * emissive.a)), 1.0);
    }
    else
    {
        outColor = vec4(albedo * (diffuse + specular + (emissive.rgb * emissive.a)), 1.0);
    }
	
}