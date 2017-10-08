#version 410 core

in vec4 gl_FragCoord;
in vec2 uv;

out vec4 FragColor;

// Include Providers Marker

uniform sampler2D g_MainTexture;
uniform sampler2D g_GlowTexture;

void main() {

    // compute globals
	vec3 hdrColor = vec3(texture(g_MainTexture, uv).rgb);
	vec3 glow = vec3(texture(g_GlowTexture, uv).rgb);

	
    // reinhard tone mapping
    vec3 mapped = hdrColor + glow /*/ (hdrColor + vec3(1.0))*/;
    //// gamma correction 
    //mapped = pow(mapped, vec3(1.0 / gamma));
  
    FragColor = vec4(mapped, 1.0);

	return;
}
