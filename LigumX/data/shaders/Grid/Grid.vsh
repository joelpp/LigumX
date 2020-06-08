#version 410 core
layout(location=0) in vec3 pos;
layout(location=1) in vec2 texCoord;
layout(location=2) in vec3 v_Normal;

uniform mat4 g_ModelToWorldMatrix;

#define PROVIDER_View

// Include ProvidersMarker

out vec2 myTexCoord;
out vec3 vNormalWS;
out vec4 vWorldPosition;
out float height;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	//vWorldPosition = g_ModelToWorldMatrix * vec4(pos, 1);
	//vWorldPosition = vec4(pos, 1);
	//gl_Position = g_ProjectionMatrix * g_WorldToViewMatrix * vWorldPosition;
	    switch(gl_VertexID) 
	{
    case 0:
        gl_Position.xy = vec2(-1,-1);
		myTexCoord = vec2(0,0);
        break;
    case 1:
        gl_Position.xy = vec2(1,-1);
		myTexCoord = vec2(1,0);
        break;
    case 2:
        gl_Position.xy = vec2(-1,1);
		myTexCoord = vec2(0,1);
        break;
    case 3:
        gl_Position.xy = vec2(1,1);
		myTexCoord = vec2(1,1);
        break;
    }
    gl_Position.z  = 0;
    gl_Position.w  = 1;

	vec3 screenPos = 2.f * pos;
	//gl_Position = vec4(screenPos, 1);
	height = pos.z;
	vNormalWS = mat3(transpose(inverse(g_ModelToWorldMatrix))) * v_Normal;

}
