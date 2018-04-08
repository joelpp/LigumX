#version 430 core
in float v_Height;
in vec2 v_TexCoords;
in vec3 v_Normal;
in float v_maxHeight;

#define PROVIDER_Material

// Include ProvidersMarker

layout(binding = 3) uniform sampler2D g_HeightfieldTexture;
layout(binding = 1) uniform sampler2D g_SplatMapTexture;


layout(binding = 4) uniform sampler2D g_RockTexture;
layout(binding = 5) uniform sampler2D g_SandTexture;
layout(binding = 6) uniform sampler2D g_WoodTexture;
layout(binding = 7) uniform sampler2D g_AlbedoTexture;



vec3 ComputeNormal(float heightMid, vec2 texCoord, float resolution)
{
	vec2 du = vec2(resolution, 0);
	vec2 dv = vec2(0, resolution);

	float heightLeft = textureLod(g_HeightfieldTexture, texCoord - du, 0.f).r;

	float heightRight =	textureLod(g_HeightfieldTexture, texCoord + du, 0.f).r;

	float heightBottom = textureLod(g_HeightfieldTexture, texCoord - dv , 0.f).r;
	float heightTop = textureLod(g_HeightfieldTexture, texCoord + dv , 0.f).r;

	float dhdu = (2 * resolution) * (heightRight - heightLeft); 
	float dhdv = (2 * resolution) * (heightTop - heightBottom); 

	//vec3 normal = normalize(vec3(0,0,1) + vec3(1,0,0) * dhdu + vec3(0,1,0) * dhdv);

	vec3 va = normalize(vec3(2, 0, 1.f / 25 * (heightRight - heightLeft)));
    vec3 vb = normalize(vec3(0, 2, 1.f / 25 * (heightTop - heightBottom)));
    vec3 normal = cross(va,vb);

	return normalize(normal);
}
vec3 blend(vec3 texture1, float a1, vec3 texture2, float a2)
{
    float depth = 0.2;
    float ma = max(a1, a2) - depth;

    float b1 = max(a1 - ma, 0);
    float b2 = max(a2 - ma, 0);

    return (texture1.rgb * b1 + texture2.rgb * b2) / (b1 + b2);
}


out vec4 o_PSOutput;
void main() 
{

	vec3 f_Normal = v_Normal;

	float resolution = 1.f / 64;
	f_Normal = ComputeNormal(v_Height, v_TexCoords, resolution);

	vec2 splatTexCoords = vec2(1 - v_TexCoords.x, v_TexCoords.y);
	vec4 splatMap = textureLod(g_SplatMapTexture, v_TexCoords, 0.0f);
	//vec3 lightDirection = normalize(vec3(0.5f, 0.5f, 1.0f));
	vec3 lightDirection = normalize(vec3(0.f, 0.f, 1.0f));
    float lightPower = dot(f_Normal, lightDirection);

	vec3 ambient = vec3(0.1f);
	vec3 diffuse = vec3(1.0f, 1.0f, 1.0f);

	if ( any(greaterThan( splatMap.rgb, vec3(0,0,0) )) )
	{
		//diffuse = splatMap.rgb;
	} 

	float uvScale = 10.f;
	vec2 diffuseTexCoords = v_TexCoords * uvScale;


	//diffuse = splatMap.x *
	//		+ splatMap.y * texture(g_RockTexture, diffuseTexCoords).rgb
	//		+ splatMap.z * texture(g_SandTexture, diffuseTexCoords).rgb
	//		+ splatMap.w * texture(g_WoodTexture, diffuseTexCoords).rgb;
	
	vec3 grass =  texture(g_Material.m_DiffuseTexture, diffuseTexCoords).rgb;
	vec3 Rock =  texture(g_RockTexture, diffuseTexCoords).rgb;
	//vec3 Sand =  texture(g_SandTexture, diffuseTexCoords).rgb;
	//vec3 Wood =  texture(g_WoodTexture, diffuseTexCoords).rgb;
	//vec3 neutral = vec3(0.2, 0.1, 0.3);
	//diffuse = grass * splatMap.x;
	//diffuse += Rock * splatMap.y;
	//diffuse += Sand * splatMap.z;
	//diffuse += Wood * splatMap.w;

	//vec3 lerp0 = mix(neutral, grass, splatMap.x);
	//lerp0 = mix(lerp0, Rock, splatMap.y);
	//lerp0 = mix(lerp0, Sand, splatMap.z);
	diffuse = mix(grass, Rock, 1.f - splatMap.z);

	//diffuse = mix(diffuse, grass, splatMap.x);
	//diffuse = mix(diffuse, Rock, splatMap.y);
	//diffuse = mix(diffuse, Sand, splatMap.z);
	//diffuse = mix(diffuse, Wood, splatMap.w);

	//float a = splatMap.x + splatMap.y * (1 - splatMap.x);
	//vec3 c0 = (grass * splatMap.x + Rock * splatMap.y * (1 - splatMap.x)) / a;

	//if (a == 0)
	//{
	//	c0 = vec3(0,0,0);
	//}

	//float olda = a;
	//a = olda + splatMap.z * (1 - olda);
	//c0 = (c0 * olda + Sand * splatMap.z * (1 - olda)) / a;

	//if (a == 0)
	//{
	//	c0 = vec3(0,0,0);
	//}
	//olda = a;
	//a = olda + splatMap.w * (1 - olda);
	//c0 = (c0 * olda + Wood * splatMap.w * (1 - olda)) / a;

	//if (a == 0)
	//{
	//	c0 = vec3(0,0,0);
	//}

	//diffuse *= texture(g_Material.m_DiffuseTexture, diffuseTexCoords).rgb;
	//diffuse = vec3(0.5f * (1.f + v_Height / 300.f), 0, 0);
	//diffuse = c0;

	vec3 finalColor = diffuse * lightPower;
	//vec3 finalColor = diffuse /** lightPower*/;
	//finalColor *= vec3(v_TexCoords, 0);
    o_PSOutput = vec4(finalColor, 1.0);

	



    //o_PSOutput = vec4(0.5f * (normal + vec3(1,1,1)), 1.0);
   
}