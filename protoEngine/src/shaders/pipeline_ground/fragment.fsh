#version 430 core
in vec3 vColor;
in vec2 myTexCoord;
in float height;
out vec4 color;
uniform layout(location=0) sampler2D sampler;
uniform layout(location=1) sampler2D sampler1;
void main() {
    float lowBound = 0.005;
    float highBound = 0.011;
    float interval = highBound- lowBound;
    if (height > highBound) color = vec4(texture(sampler1, myTexCoord).xyz,1.0);
    else if (height < lowBound) color = vec4(texture(sampler, myTexCoord).xyz,1.0);
    else color = vec4(mix(texture(sampler1, myTexCoord).xyz, texture(sampler, myTexCoord).xyz, (highBound-height) / interval),1.0);
}
