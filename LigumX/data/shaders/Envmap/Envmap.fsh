#version 430 core

in vec4 gl_FragCoord;
in vec3 direction;
in vec3 normal;
in vec2 uv;

uniform vec2 windowSize;

#define PROVIDER_View
#define PROVIDER_Sky

layout(binding = 4) uniform sampler2D g_NoiseTexture;

out vec3 color;

float pi = 3.14159265359;
//float iTime = sunTime;
vec3 sunDir;
vec2 sunDirFlat;

// Include ProvidersMarker
// Include BasicUtils.glsl
// Include SkyUtils.glsl
// Include CloudUtils.glsl



void main() 
{

	vec2 pos01 = gl_FragCoord.xy/windowSize;
    vec3 fragDir = normalize(direction);

	if (g_UseSkybox)
	{
		color = vec3(texture(g_Skybox, fragDir));
		return;
	}

	if (fragDir.z < 0)
	{
		discard;
	}

	vec3 pos = /*g_CameraPosition +*/ fragDir * (2.f + sin(sunTime));
	vec3 skyColor = GetSkyColor(fragDir, sunTime, sunOrientation);
	color = skyColor;
	// clouds    
	vec4 res = raymarch(pos, fragDir.xyz, color, ivec2(gl_FragCoord.xy));
	vec3 cloudColor = res.xyz;
	float nightFactor = saturate(sin(sunTime - (3.f * PI / 2.f)) + 0.15f);

	cloudColor = mix(skyColor, cloudColor, 0.4f + 0.6f * smoothstep(0.4, 0.6, nightFactor));
	color = color * (1.0 - res.w) + cloudColor;


}
