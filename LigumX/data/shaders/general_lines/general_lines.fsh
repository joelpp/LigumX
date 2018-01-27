#version 410 core

#define PROVIDER_Material

// Include ProvidersMarker

out vec4 OutputColor;

in VertexData
{
  vec3 m_WorldPosition;
} inData;

float GetLineWidth(vec2 wsPosition, float gridExtent)
{
	vec2 grid = abs(fract( (wsPosition.xy / gridExtent) - 0.5) - 0.5) / fwidth(wsPosition.xy / gridExtent);
	float line = min(grid.x, grid.y);
	return line;
}

float GetLineAlpha(float lineWidth, float maximum)
{
	return maximum - min(lineWidth, maximum);
}


void main()
{
    OutputColor = vec4(g_Material.m_DiffuseColor, 1.0f);
}