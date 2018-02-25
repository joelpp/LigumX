#version 410 core

in vec4 gl_FragCoord;
in vec2 uv;

out vec4 FragColor;

#define PROVIDER_PostEffects

// Include ProvidersMarker

uniform sampler2D g_MainTexture;
uniform sampler2D g_GlowTexture;

void main() {

    // compute globals
	vec3 hdrColor = vec3(texture(g_MainTexture, uv).rgb);
	vec3 glow = vec3(texture(g_GlowTexture, uv).rgb);

	if (g_GammaCorrectionEnabled > 0)
	{
		hdrColor.rgb = pow(hdrColor.rgb, vec3(g_GammaCorrectionExponent));
		glow.rgb = pow(glow.rgb, vec3(g_GammaCorrectionExponent));
	}
    // reinhard tone mapping
    vec3 mapped = hdrColor  /*/ (hdrColor + vec3(1.0))*/;
    //// gamma correction 
    //mapped = pow(mapped, vec3(1.0 / gamma));
  
  	if (g_ToneMappingEnabled)
	{
		mapped = mapped / (mapped + vec3(1.0));
	}

    FragColor = vec4(mapped /*+ glow*/, 1.0);
	
	FragColor.rgb = pow(FragColor.rgb, vec3(1.0f / g_GammaCorrectionExponent));

	return;
}
