// Shadow Map

#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_Transform;

out vec4 FragPosLightSpace;

void main()
{
    FragPosLightSpace = u_LightSpaceMatrix* u_Transform* vec4(a_Position, 1.0);
    gl_Position = u_LightSpaceMatrix * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core

in vec4 FragPosLightSpace;
void main()
{
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    // ±ä»»µ½[0,1]µÄ·¶Î§
    projCoords = projCoords * 0.5 + 0.5;
}