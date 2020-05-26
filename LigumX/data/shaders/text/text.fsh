#version 410 core
in vec2 TexCoords;
flat in int VertexOutput_TextureCharacterIndex;

out vec4 color;

uniform sampler2D text;
uniform vec3 g_TextColor;
uniform vec4 g_DebugVec4;

int s_CharsPerSide = 8;
int s_TextureSize = 256;
float s_CharSize = float(s_TextureSize) / float(s_CharsPerSide);

ivec2 GetCharIndex(int c)
{
	return ivec2(c % s_CharsPerSide, c / s_CharsPerSide); // column, row
}

void main()
{
    vec3 myColor = g_TextColor;

	int character = VertexOutput_TextureCharacterIndex;
	ivec2 charIndex = GetCharIndex(character);
	//charIndex = ivec2(3, 3);

	vec2 uvScale = 1.f / vec2(s_CharsPerSide, s_CharsPerSide);
	vec2 uvOffset = vec2(charIndex) * uvScale;

	vec2 uv = TexCoords;

	uv.x += 0.35f;
	uv.x *= 0.57f;

	uv.y = 1 - uv.y;
	uv *= uvScale;
	uv += uvOffset;
	uv.y = 1 - uv.y;

	//uv.x *= g_DebugVec4.x;
	//uv.x += g_DebugVec4.z;

	//uv.y = 1 - uv.y;
	//uv.y *= g_DebugVec4.y;
	//uv.y += g_DebugVec4.w;
	//uv.y = 1 - uv.y;

    vec4 sampled = vec4(1.0, 1.0, 1.0, textureLod(text, uv, 0.f).r);
    color = vec4(myColor, 1.0) * sampled;
	color = sampled;
	//if (any(lessThan(abs(TexCoords - round(TexCoords)), vec2(0.1f, 0.1f))))
	//{
	//	color = vec4(0, 0, 0, 1.0);
	//}

	//color = vec4(1.f, 1.f, 1.f, 1.f);
    //color = vec4(1.0);
}
