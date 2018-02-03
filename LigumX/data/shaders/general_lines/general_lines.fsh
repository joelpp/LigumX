#version 410 core

#define PROVIDER_Material

// Include ProvidersMarker

#define HIGHWAY_TRUNK		 1 << 0
#define HIGHWAY_PRIMARY		 1 << 1
#define HIGHWAY_SECONDARY	 1 << 2
#define HIGHWAY_TERTIARY	 1 << 3
#define HIGHWAY_RESIDENTIAL	 1 << 4
#define HIGHWAY_SERVICE		 1 << 5
#define HIGHWAY_UNCLASSIFIED 1 << 6
#define SIDEWALK			 1 << 7

#define CONTOUR				 1 << 10

#define BUILDING_UNMARKED	 1 << 15
#define BUILDING_SCHOOL		 1 << 16
#define BUILDING_ADDRINTERP	 1 << 17

#define BOUNDARY			 1 << 21
#define LEISURE_PARK		 1 << 22
#define NATURAL_WOOD		 1 << 23
#define NATURAL_WATER		 1 << 24
#define LANDUSE				 1 << 25
#define RAILWAY_SUBWAY		 1 << 26

#define aDEBUG				 1 << 27
#define ADDR_INTERPOLATION	 1 << 28
#define NOT_IMPLEMENTED		 1 << 29
#define ANY_TYPE			 1 << 30
#define GRID_LINE			 1 << 31

out vec4 OutputColor;

in VertexData
{
	vec3 m_WorldPosition;
	flat int m_WayType;
} inData;

uniform int g_DisplayFlags;

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

#define ELSE_COLOR(type, color) else if ((wayID & type) != 0)	{ return color ; }



vec3 GetLineColor(int wayID)
{
	if ((wayID & ADDR_INTERPOLATION) != 0)
	{
		return vec3(1,0,0);
	}
    ELSE_COLOR(HIGHWAY_TRUNK, vec3(1,1,1))
    ELSE_COLOR(HIGHWAY_PRIMARY, vec3(0.9,0.9,0.9))
    ELSE_COLOR(HIGHWAY_SECONDARY, vec3(0.8,0.8,0.8))
    ELSE_COLOR(HIGHWAY_TERTIARY, vec3(0.7,0.7,0.7))
    ELSE_COLOR(HIGHWAY_RESIDENTIAL,	vec3(0.6,0.6,0.6))
    ELSE_COLOR(HIGHWAY_UNCLASSIFIED, vec3(0.5,0.5,0.5))
    ELSE_COLOR(HIGHWAY_SERVICE, vec3(0.4,0.4,0.4))
    ELSE_COLOR(BUILDING_UNMARKED, vec3(0,0,1))
    ELSE_COLOR(BUILDING_SCHOOL, vec3(0,0,1))
    ELSE_COLOR(BUILDING_ADDRINTERP,	vec3(0.4,1,1))
    ELSE_COLOR(RAILWAY_SUBWAY, vec3(1,0,1))
    ELSE_COLOR(NATURAL_WOOD, vec3(0,0.5,0))
    ELSE_COLOR(NATURAL_WATER, vec3(0,0,0.5))
    ELSE_COLOR(LEISURE_PARK, vec3(0,1,0))
    ELSE_COLOR(ADDR_INTERPOLATION, vec3(1,0,0))
    ELSE_COLOR(GRID_LINE, vec3(0.4,0.4,0.4))
    ELSE_COLOR(aDEBUG, vec3(1.0,0,1.0))
    ELSE_COLOR(LANDUSE, vec3(1.0,1.0,1.0))
    ELSE_COLOR(BOUNDARY, vec3(1.0,1.0,1.0))
    ELSE_COLOR(CONTOUR, vec3(1.0,0.1,0.1))



	return vec3(0, 0, 0);
}

void main()
{
	int wayID = inData.m_WayType;
	vec3 color = GetLineColor(wayID);

	if ((wayID & g_DisplayFlags) == 0)
	{
		discard;
	}

    OutputColor = vec4(color, 1.0f);
}