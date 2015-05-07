#version 410 core
in vec2 texCoord;
in float ID;
uniform sampler2D sampler;
uniform sampler2D sampler1;

out vec4 color;
void main() {
    //color = vec3(1,1,1);
    vec2 coord = mod(500*texCoord, 1);
    //color = vec3(coord.x, coord.y,1);
    //color = texture(sampler, coord).xyz;
if (ID == 1.0) 
    color = vec4(texture(sampler1, coord).xyz,1.0);
if (ID == 2.0)  
    color = vec4(texture(sampler, coord).xyz,1.0);
    //color = vec4(0.5,0.2,0.2,1.0);
//color = vec4(vec3(ID / 2.0), 1.0);

}