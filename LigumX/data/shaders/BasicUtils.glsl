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

float saturate(float value)
{
	return clamp(value, 0.f, 1.f);
}

float LinearizeDepth(float nonLinearDepth, float cameraNear, float cameraFar)
{
	float near = cameraNear;
	float far = cameraFar;

	float z = nonLinearDepth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

vec3 EncodeNormal(vec3 wsNormal)
{
	return 0.5f * (wsNormal + vec3(1.f));
}
vec3 DecodeNormal(vec3 codedNormal)
{
	return (codedNormal * 2.f) - vec3(1.f);
}



#endif