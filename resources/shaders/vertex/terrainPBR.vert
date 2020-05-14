layout(location=0) in vec4 pos;
layout(location=1) in vec2 inCoord;
layout(location=2) in vec3 inNormal;
layout(location=3) in vec3 tangent;
layout(location=4) in vec3 biNormal;

uniform sampler2D heightmap;

layout (std140, binding = 1) uniform TerrainVariables
{
	float heightScale;
	float texUv0Multiplier;
	float texUv1Multiplier;
	float texUv2Multiplier;
	float slopeAngle;
	float heightFalloff;
	float height;
	float slopeAngle2;
	float heightFalloff2;
	float height2;
	float hardness1;
	float hardness2;
	float hardness3;
	vec2 points[10];
};  

out vec3 fragPos;
out vec2 texCoord;

out mat3 o_normal;
out vec3 o_toLight;
out vec3 o_toCamera;
out mat3 model33Out;

uniform vec3 lightPosition;

float CurveValue(float p, int maxpoints, const vec2[10] points)
{
	if (maxpoints < 2 || points.length() == 0)
		return 0;
	if (p < 0) return points[0].y;

	int left = 0;
	while (left < maxpoints && points[left].x < p && points[left].x != -1) left++;
	if (bool(left)) left--;

	if (left == maxpoints-1)
		return points[maxpoints - 1].y;

	float d = (p - points[left].x) / (points[left + 1].x - points[left].x);

	return points[left].y + (points[left + 1].y - points[left].y) * d;
}

void main()
{
	float height = texture2D(heightmap, inCoord).r;
	
	vec4 vertPos = pos;
	vertPos.y = CurveValue(height, 10, points)* heightScale;
	
	vec4 worldpos = Model*vertPos;

	mat3 model33 = mat3(Model);
		
	vec3 t = normalize(model33 * (tangent));
	vec3 b = normalize(model33 * (biNormal));
	vec3 n = normalize(model33 * (inNormal));

	fragPos = worldpos.xyz;
	
	o_normal = mat3(t,b,n);	
	o_toLight = normalize(lightPosition - worldpos.xyz);
	o_toCamera = normalize(CameraPosition.xyz - worldpos.xyz);
	texCoord = inCoord;
	model33Out = model33;
	
	gl_Position = Projection*View*worldpos;
}