#version 410 core

in vec4 gl_FragCoord;
in vec2 uv;

out vec3 color;

uniform sampler2D g_Texture;

void main() {

    // compute globals
	color = vec3(texture(g_Texture, uv).rgb) * 3;
	//color = vec3(uv, 0.f);
	return;
}
