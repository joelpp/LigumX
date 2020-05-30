#version 410 core

in vec4 gl_FragCoord;
in vec3 direction;
in vec3 normal;
in vec2 uv;

uniform vec2 windowSize;

#define PROVIDER_View

#define PROVIDER_Sky

// Include ProvidersMarker
// Include BasicUtils.glsl
// Include SkyUtils.glsl

out vec3 color;

float pi = 3.14159265359;
vec3 sunDir;
vec2 sunDirFlat;


void main() 
{

	vec2 pos01 = gl_FragCoord.xy/windowSize;
    vec3 fragDir = normalize(direction);

	if (g_UseSkybox)
	{
		color = vec3(texture(g_Skybox, fragDir));
		return;
	}

	color = GetSkyColor(fragDir, sunTime, sunOrientation);

}
