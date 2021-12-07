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

uniform mat4 u_LightSpaceMatrix;
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
out vec4 FragPosLightSpace;

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


    FragPosLightSpace = u_LightSpaceMatrix * u_Transform * vec4(a_Position + a_Normal * height, 1.0);
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position + a_Normal * height, 1.0);
}

#type fragment
#version 460 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;//第二个FrameBuffer

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;
in mat3 TBN;
in vec3 v_ViewPos;
in vec4 FragPosLightSpace;

in flat int v_EntityID;
struct Material
{
    vec3 color;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normalMap;
    sampler2D displacementMap;
    float shininess;
};

struct Light
{
    vec3 color;
    vec3 position;
    float intensity;
};

struct DirLight
{
    vec3 color;
    vec3 direction;
    float intensity;
};

struct PointLight
{
    vec3 color;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    float intensity;
};

struct SpotLight
{
    vec3 color;
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutOff;
    float intensity;
};

#define MAX_LIGHT_NUM 10

uniform Material u_Material;
uniform Light u_Light[MAX_LIGHT_NUM];
uniform DirLight u_DirLight[MAX_LIGHT_NUM];
uniform PointLight u_PointLight[MAX_LIGHT_NUM];
uniform SpotLight u_SpotLight[MAX_LIGHT_NUM];

uniform sampler2D ShadowMap;

uniform int DirectLightNum;
uniform int PointLightNum;
uniform int SpotLightNum;
uniform int u_UseNormalMap;

uniform vec3 u_LightPos;

// TODO
float Ambient = 0.3;
vec3 normal = vec3(0.0, 0.0, 0.0);


//  Calculate directional light
vec3 CalculateDirLight(DirLight light, float shadow)
{
    //  environment light
    vec3 ambient = Ambient * light.color * vec3(texture(u_Material.diffuse, v_TexCoord));

    //  diffuse light
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(texture(u_Material.diffuse, v_TexCoord)) * light.color * diff;

    //  specular light
    vec3 viewDir = normalize(v_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = vec3(texture(u_Material.specular, v_TexCoord)) * spec * light.color;

    return (ambient + (1.0 - shadow) * (diffuse + specular)) * light.intensity;
}

//  Calculate point light
vec3 CalculatePointLight(PointLight light, float shadow)
{
    //  environment light
    vec3 ambient = Ambient * light.color * vec3(texture(u_Material.diffuse, v_TexCoord));

    //  diffuse light
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(TBN * light.position - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(texture(u_Material.diffuse, v_TexCoord)) * light.color * diff;

    //  specular light
    vec3 viewDir = normalize(v_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = vec3(texture(u_Material.specular, v_TexCoord)) * spec * light.color;

    //  attenuation factor
    float dist = length(TBN * light.position - v_FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    return (ambient + diffuse + specular) * attenuation * light.intensity;
}

//  Calculate Spot light
vec3 CalculateSpotLight(SpotLight light, float shadow)
{
    //  environment light
    vec3 ambient = Ambient * light.color * vec3(texture(u_Material.diffuse, v_TexCoord));

    //  diffuse light
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(TBN * light.position - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    float theta = dot(lightDir, normalize(light.direction));
    float epsilon = light.cutoff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 diffuse = vec3(texture(u_Material.diffuse, v_TexCoord)) * light.color * diff;
    //  specular light
    vec3 viewDir = normalize(v_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = vec3(texture(u_Material.specular, v_TexCoord)) * spec * light.color;
    return (ambient + diffuse + specular) * light.intensity * intensity;

}


//float ShadowCalculation(vec4 fragPosLightSpace)
//{
//    // perform perspective divide
//    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//    // transform to [0,1] range
//    projCoords = projCoords * 0.5 + 0.5;
//    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
//    float closestDepth = texture(ShadowMap, projCoords.xy).r;
//    // get depth of current fragment from light's perspective
//    float currentDepth = projCoords.z;
//    // calculate bias (based on depth map resolution and slope)
//    vec3 normal = normalize(v_Normal);
//    vec3 lightDir = normalize(u_LightPos - v_FragPos);
//    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
//    // check whether current frag pos is in shadow
//    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
//    // PCF
//    float shadow = 0.0;
//    vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
//    for (int x = -1; x <= 1; ++x)
//    {
//        for (int y = -1; y <= 1; ++y)
//        {
//            float pcfDepth = texture(ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
//            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
//        }
//    }
//    shadow /= 1.0;
//
//    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
//    //if (projCoords.z > 1.0)
//    //    shadow = 0.0;
//
//    return shadow;
//}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    float closestDepth = texture(ShadowMap, projCoords.xy).g;
    // 取得当前片段在光源视角下的深度
    float currentDepth = projCoords.z;
    // 检查当前片段是否在阴影中
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    if (u_UseNormalMap == 1)
        normal = normalize(texture(u_Material.normalMap, v_TexCoord).rgb * 2.0 - 1.0);
    else
        normal = v_Normal;

    vec3 result = vec3(0.0);

    float shadow = ShadowCalculation(FragPosLightSpace);

    for (int i = 0; i < DirectLightNum; i++)
    {
        result += CalculateDirLight(u_DirLight[i], shadow);
    }

    for (int i = 0; i < PointLightNum; i++)
    {
        result += CalculatePointLight(u_PointLight[i], shadow);
    }

    //for (int i = 0; i < SpotLightNum; i++)
    //{
    //    result += CalculateSpotLight(u_SpotLight[i], shadow);
    //}

    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float d = texture(ShadowMap, projCoords.xy).r;
    color = vec4(d, d, d, 1);;
    if(shadow>0)
        color = vec4(d, 0, 0, 1);
	//color = vec4(result * u_Material.color,1.0);
	color2 = v_EntityID; // placeholder for our entity ID
}