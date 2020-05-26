#version 430 core

struct TextInstance_VertexData
{
	vec2 m_Position;
	float m_Scale;
	int m_TextureCharIndex;
};


layout (location = 0) in vec3 vertex; // <vec2 pos, vec2 tex>
layout(location = 1) in vec2 uvs;

layout(std430, binding = 2) buffer g_VertexInstanceData
{
	TextInstance_VertexData g_InstanceVertexData[];
};

//layout (location = 2) in TextInstance_VertexData uvs;

out vec2 TexCoords;
flat out int VertexOutput_TextureCharacterIndex;
//uniform vec2 g_Position;
//uniform float g_Scale;


//uniform TextInstance_VertexData g_InstanceVertexData[100];

uniform mat4 projection;
out gl_PerVertex {
    vec4 gl_Position;
};
void main()
{
	uint instanceID = gl_InstanceID;
	TextInstance_VertexData vertexData = g_InstanceVertexData[instanceID];

	//gl_Position = projection * vec4(vertex.xyz, 1.0);
	vec2 outPosition = vertex.xy * vertexData.m_Scale;
	outPosition += vertexData.m_Position.xy;

	VertexOutput_TextureCharacterIndex = vertexData.m_TextureCharIndex;

	gl_Position = vec4(outPosition, 0.f, 1.f);
    TexCoords = uvs;
}
