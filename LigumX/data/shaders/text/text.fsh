#version 410 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 g_TextColor;
uniform int g_CurrentCharacter;

int s_CharsPerSide = 8;
int s_TextureSize = 256;
float s_CharSize = float(s_TextureSize) / float(s_CharsPerSide);

ivec2 GetCharIndex(int c)
{
	return ivec2(c / s_CharsPerSide, c % s_CharsPerSide);
}

void main()
{
    vec3 myColor = g_TextColor;

	ivec2 charIndex = GetCharIndex(g_CurrentCharacter);

	vec2 uvScale = 1.f / vec2(s_CharSize, s_CharSize);
	vec2 uvOffset = vec2(charIndex) * uvScale;

	vec2 uv = TexCoords;
	//uv *= uvScale;
	//uv += uvOffset;

    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, uv).r);
    color = vec4(myColor, 1.0) * sampled;
    //color = vec4(1.0);
}
