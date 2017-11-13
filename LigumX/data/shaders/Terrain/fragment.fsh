#version 410 core
in float v_Height;
in vec2 v_TexCoord;
in vec3 v_Normal;
in float v_maxHeight;

//uniform sampler2D g_SplatMapTexture;


out vec4 o_PSOutput;
void main() 
{

	vec3 lightDirection = normalize(vec3(0.5f, 0.5f, 1.0f));
    float lightPower = 0.4f *  dot(v_Normal, lightDirection);

	vec3 ambient = vec3(0.1f);
	vec3 diffuse = vec3(0.1f, 0.8f, 0.1f);

	vec3 finalColor = diffuse * (ambient + lightPower);

    o_PSOutput = vec4(finalColor, 1.0);

	
    //o_PSOutput = vec4(0.5f * (v_Normal + vec3(1,1,1)), 1.0);
	//color = vec4(v_Normal, 1.0f);
	//color.z = 0;
   
}