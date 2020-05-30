#ifndef GLSL_BASIC_UTILS
#define GLSL_BASIC_UTILS

vec4 SampleTexture(sampler2D theTexture, vec2 uv)
{
	vec4 samplingResult = texture(theTexture, uv);

	return samplingResult;
}

// adapted from http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/
mat3 rotateAroundAxis(vec3 axis, float angle)
{
	axis = normalize(axis);
	float s = sin(angle);
	float c = cos(angle);
	float oc = 1.0 - c;

	return mat3(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s,
		oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s,
		oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c);
}

#endif