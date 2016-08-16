#version 410 core
in vec2 myTexCoord;
in float height;

out vec4 color;
void main() {

	
    color = vec4(vec3(height * 50), 1);

    }