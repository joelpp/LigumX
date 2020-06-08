#version 410 core
#extension GL_ARB_shading_language_include : require

in vec2 myTexCoord;
in vec3 vNormalWS;
in vec4 vWorldPosition;
in float height;

#define PROVIDER_View
#define PROVIDER_Light
#define PROVIDER_Debug
#define PROVIDER_PostEffects
#define PROVIDER_Material

// Include ProvidersMarker

out vec4 FinalColor;

void main() 
{
			float depth = gl_FragCoord.z;
		FinalColor = vec4(depth, depth, 0.8, 1.f);
		return ;

	vec3 fNormalWS = normalize(vNormalWS);
	

	if (g_DebugNormalsEnabled > 0)
	{
		FinalColor = GetDebugNormalColor(fNormalWS);
		return ;
	}

	//if (g_DebugDepthEnabled > 0)
	{
		//float depth = 0;
		//if (g_DebugLinearizeDepth > 0)
		//{
		//	depth = LinearizeDepth(gl_FragCoord.z) / g_CameraFarPlane;
		//}
		//else
		//{
			depth = gl_FragCoord.z;
		//}
		FinalColor = vec4(depth, depth, 0.8, 1.f);
		return ;
	}

	// todo : when i do proper attenuation i need to re-visit gamma


}