// Shadow Map

#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_Transform;

void main()
{
    gl_Position = u_LightSpaceMatrix * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core

layout(location = 0) out float depth;

void main()
{
    depth = gl_FragCoord.z;
}