#version 410 core

//in int gl_VertexID;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    switch(gl_VertexID) {
    case 0:
        gl_Position.xy = vec2(-1,-1);
        break;
    case 1:
        gl_Position.xy = vec2(1,-1);
        break;
    case 2:
        gl_Position.xy = vec2(-1,1);
        break;
    case 3:
        gl_Position.xy = vec2(1,1);
        break;
    }
    gl_Position.z  = 0;
    gl_Position.w  = 1;
}
