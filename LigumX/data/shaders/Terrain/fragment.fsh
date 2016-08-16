#version 410 core
in vec2 myTexCoord;
in float height;
//uniform sampler2D sampler;
//uniform sampler2D sampler1;
out vec4 color;
void main() {
    //color=vec4(1.0,1.0,1.0,1.0);
    //color = vec4(vec3(height/0.02),1.0);
    //float lowBound = 0.0005;
    //float highBound = /*0.011*/lowBound;
    //float interval = highBound- lowBound;
    //if (height > highBound) color = vec4(texture(sampler1, myTexCoord).xyz,1.0);
    //else if (height < lowBound) color = vec4(texture(sampler, myTexCoord).xyz,1.0);
    //else color = vec4(mix(texture(sampler1, myTexCoord).xyz, texture(sampler, myTexCoord).xyz, (highBound-height) / interval),1.0);
    float outColor = height;
    color = vec4(vec3(height), 1.0);
    //if (ID == 0.0)
    //	color = vec4(texture(sampler1, myTexCoord).xyz,1.0);
//
    //if (ID == 1.0) 
    //	color = vec4(texture(sampler, myTexCoord).xyz,1.0);
//
    //float c = ID+0.2;
    //color = vec4(vec3(c),1.0);

    }