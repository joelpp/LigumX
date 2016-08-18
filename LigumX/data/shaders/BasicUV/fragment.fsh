#version 410 core
in vec2 myTexCoord;
in float height;

out vec4 color;
void main() {

	
    color = vec4(myTexCoord, 0, 1);

    }