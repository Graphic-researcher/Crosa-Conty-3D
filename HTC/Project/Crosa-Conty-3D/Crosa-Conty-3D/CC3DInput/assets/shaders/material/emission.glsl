#type vertex
#version 450 

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform int a_EntityID;

out flat int v_EntityID;

void main()
{
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int color1;

uniform vec3 u_EmissionColor;
uniform float u_Intensity;

in flat int v_EntityID;

void main()
{
	FragColor=vec4(u_EmissionColor*u_Intensity,1.0);
	color1 = v_EntityID;
}