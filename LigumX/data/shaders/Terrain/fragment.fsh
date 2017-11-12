#version 410 core
in float v_Height;
in vec2 v_TexCoord;
in vec3 v_Normal;
in float v_maxHeight;



out vec4 color;
void main() 
{

	vec3 lightDirection = normalize(vec3(0.f, 0.f, 1.0f));
    float lightPower = dot(v_Normal, lightDirection);

	vec3 diffuse = vec3(1.0f, 1.0f, 1.0f);

    color = vec4(diffuse * lightPower, 1.0);

	
    color = vec4(0.5f * (v_Normal + vec3(1,1,1)), 1.0);
	//color = vec4(v_Normal, 1.0f);
	//color.z = 0;
   
}