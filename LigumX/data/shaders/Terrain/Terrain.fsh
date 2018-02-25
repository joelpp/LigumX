#version 430 core
in float v_Height;
in vec2 v_TexCoords;
in vec3 v_Normal;
in float v_maxHeight;

#define PROVIDER_Material

// Include ProvidersMarker

layout(binding = 3) uniform sampler2D g_HeightfieldTexture;
uniform sampler2D g_SplatMapTexture;


//vec3 ComputeNormal(float heightMid, vec2 texCoord, float resolution)
//{
//	vec2 du = vec2(resolution, 0);
//	vec2 dv = vec2(0, resolution);

//	float heightLeft = textureLod(g_HeightfieldTexture, texCoord - du, 0.f).r;

//	float heightRight =	textureLod(g_HeightfieldTexture, texCoord + du, 0.f).r;

//	float heightBottom = textureLod(g_HeightfieldTexture, texCoord - dv , 0.f).r;
//	float heightTop = textureLod(g_HeightfieldTexture, texCoord + dv , 0.f).r;

//	float dhdu = (2 * resolution) * (heightRight - heightLeft); 
//	float dhdv = (2 * resolution) * (heightTop - heightBottom); 

//	//vec3 normal = normalize(vec3(0,0,1) + vec3(1,0,0) * dhdu + vec3(0,1,0) * dhdv);

//	vec3 va = normalize(vec3(2, 0, heightRight - heightLeft));
//    vec3 vb = normalize(vec3(0, 2, heightTop - heightBottom));
//    vec3 normal = cross(va,vb);

//	return normalize(normal);
//}


out vec4 o_PSOutput;
void main() 
{

	vec3 f_Normal = v_Normal;

	vec4 splatMap = textureLod(g_SplatMapTexture, v_TexCoords, 0.0f);
	//vec3 lightDirection = normalize(vec3(0.5f, 0.5f, 1.0f));
	vec3 lightDirection = normalize(vec3(0.f, 0.f, 1.0f));
    float lightPower = dot(f_Normal, lightDirection);

	vec3 ambient = vec3(0.1f);
	vec3 diffuse = vec3(1.0f, 1.0f, 1.0f);

	if ( any(greaterThan( splatMap.rgb, vec3(0,0,0) )) )
	{
		diffuse = splatMap.rgb;
	} 

	float uvScale = 10.f;
	vec2 diffuseTexCoords = v_TexCoords * uvScale;
	diffuse = texture(g_Material.m_DiffuseTexture, diffuseTexCoords).rgb;

	vec3 finalColor = diffuse * lightPower;
	//finalColor *= vec3(v_TexCoords, 0);
    o_PSOutput = vec4(finalColor, 1.0);

	

	//float resolution = 1.f / 64;
	//normal = ComputeNormal(v_Height, v_TexCoords, resolution);

    //o_PSOutput = vec4(0.5f * (normal + vec3(1,1,1)), 1.0);
   
}