// Basic Texture Shader

#type vertex
#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Bitangent;
layout(location = 4) in vec2 a_TexCoord;

layout(location = 5) in int a_EntityID;


struct Material
{
	vec3 color;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normalMap;
	sampler2D displacementMap;
	float shininess;
};

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform vec3 u_ViewPos;
uniform float u_HeightScale;
uniform Material u_Material;


out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPos;
out mat3 TBN;
out vec3 v_ViewPos;

out flat int v_EntityID;

void main()
{
	vec3 T = normalize(vec3(u_Transform * vec4(a_Tangent, 0.0)));
	vec3 B = normalize(vec3(u_Transform * vec4(a_Bitangent, 0.0)));
	vec3 N = normalize(vec3(u_Transform * vec4(a_Normal, 0.0)));
	TBN = transpose(mat3(T, B, N));
	if (a_TexCoord.x == a_TexCoord.y && a_TexCoord.x == 0)
		TBN = mat3(1.0);

	float height = texture(u_Material.displacementMap, a_TexCoord).r;
	height = u_HeightScale * height;

	v_Normal = TBN * mat3(transpose(inverse(u_Transform))) * a_Normal;
	v_FragPos = TBN * vec3(u_Transform * vec4(a_Position + a_Normal * height, 1.0));
	v_ViewPos = TBN * u_ViewPos;
	v_TexCoord = a_TexCoord;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;//µÚ¶þ¸öFrameBuffer

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;
in mat3 TBN;
in vec3 v_ViewPos;

in flat int v_EntityID;


void main()
{
	color = vec4(1.0,1.0,1.0,1.0);
	color2 = v_EntityID; // placeholder for our entity ID
}