float rand(float n) { return fract(sin(n) * 43758.5453123); }

float noise2(float p) {
	float fl = floor(p);
	float fc = fract(p);
	return mix(rand(fl), rand(fl + 1.0), fc);
}

float noise3(in vec3 x)
{
	vec3 p = floor(x);
	vec3 f = fract(x);
	f = f * f * (3.0 - 2.0 * f);

#if 1
	vec2 uv = (p.xy + vec2(37.0, 239.0) * p.z) + f.xy;
	vec2 rg = textureLod(g_NoiseTexture, (uv + 0.5) / 256.f, 0.0).yx;
	//rg.y = 1.f - rg.y;
#else
	ivec3 q = ivec3(p);
	ivec2 uv = q.xy + ivec2(37, 239) * q.z;

	vec2 rg = mix(mix(texelFetch(iChannel0, (uv) & 255, 0),
		texelFetch(iChannel0, (uv + ivec2(1, 0)) & 255, 0), f.x),
		mix(texelFetch(iChannel0, (uv + ivec2(0, 1)) & 255, 0),
			texelFetch(iChannel0, (uv + ivec2(1, 1)) & 255, 0), f.x), f.y).yx;
#endif    
	return -1.0 + 2.0 * mix(rg.x, rg.y, f.z);
}
vec3 cloudDirection = normalize(vec3(1.f, 0.7f, 0.01f));
float map5(in vec3 p)
{
	vec3 q = p - cloudDirection * sunTime;
	float f;
	f = 0.50000 * noise3(q); q = q * 2.02;
	f += 0.25000 * noise3(q); q = q * 2.03;
	f += 0.12500 * noise3(q); q = q * 2.01;
	f += 0.06250 * noise3(q); q = q * 2.02;
	f += 0.03125 * noise3(q);
	return clamp(/*1.5 - p.y - 2.0 + 1.75 **/ f, 0.0, 1.0);
}
float map4(in vec3 p)
{
	vec3 q = p - cloudDirection * sunTime;
	float f;
	f = 0.50000 * noise3(q); q = q * 2.02;
	f += 0.25000 * noise3(q); q = q * 2.03;
	f += 0.12500 * noise3(q); q = q * 2.01;
	f += 0.06250 * noise3(q);
	return clamp(/*1.5 - p.y - 2.0 + 1.75 **/ f, 0.0, 1.0);
}
float map3(in vec3 p)
{
	vec3 q = p - cloudDirection * sunTime;
	float f;
	f = 0.50000 * noise3(q); q = q * 2.02;
	f += 0.25000 * noise3(q); q = q * 2.03;
	f += 0.12500 * noise3(q);
	return clamp(/*1.5 - p.y - 2.0 + 1.75 * */ f, 0.0, 1.0);
}
float map2(in vec3 p)
{
	vec3 q = p - cloudDirection * sunTime;
	float f;
	f = 0.50000 * noise3(q); q = q * 2.02;
	f += 0.25000 * noise3(q);;
	return clamp(/*1.5 - p.y - 2.0 + 1.75 * */ f, 0.0, 1.0);
}

//vec3 sundir = normalize(vec3(-1.0, 0.0, -1.0));

#define MARCH(STEPS,MAPLOD)\
for(int i=0; i<STEPS; i++)\
{\
   vec3 pos = ro + t*rd;\
   /*if( pos.y<-3.0 || pos.y>2.0 || sum.a>0.99 ) break;*/\
   float den = MAPLOD( pos );\
   if( den> (0.2f + 0.1f * (sin(sunTime / 100.f))))\
   {\
     float dif = clamp((den - MAPLOD(pos+0.3*sunDir))/0.6, 0.0, 1.0 );\
     vec3  lin = vec3(0.65,0.7,0.75)*1.4 + vec3(1.0,0.6,0.3)*dif;\
     vec4  col = vec4( mix( vec3(1.0,0.95,0.8), vec3(0.25,0.3,0.35), den ), den );\
     col.xyz *= lin;\
     col.xyz = mix( col.xyz, bgcol, 1.0-exp(-0.003*t*t) );\
     col.w *= 0.4;\
     \
     col.rgb *= col.a;\
     sum += col*(1.0-sum.a);\
   }\
   t += max(0.05,0.02*t);\
}
vec4 raymarch(in vec3 ro, in vec3 rd, in vec3 bgcol, in ivec2 px)
{
	vec4 sum = vec4(0.0);

	float t = 0.0;//0.05*texelFetch( iChannel0, px&255, 0 ).x;

	//MARCH(40, map5);
	MARCH(40, map4);
	MARCH(30, map3);
	MARCH(30, map2);

	return clamp(sum, 0.0, 1.0);
}
