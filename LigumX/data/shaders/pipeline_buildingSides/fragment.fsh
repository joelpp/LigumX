#version 410 core
uniform sampler2D sampler;

in vec2 gTexCoord;
out vec3 color;
void main() {
//            vec2 coord = mod(500*texCoord, 1);
    //color = vec3(coord.x, coord.y,1);
    color = texture(sampler, gTexCoord).xyz;
//            color = vec3(gTexCoord.x, gTexCoord.y, 0.5);
}