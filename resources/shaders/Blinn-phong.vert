
layout(location=0) in vec4 pos;
layout(location=1) in vec2 inCoord;
layout(location=2) in vec3 inNormal;

layout(location=0) out vec3 fragPos;
layout(location=1) out vec2 texCoord;

out vec3 o_normal;
out vec3 o_toLight;
out vec3 o_toCamera;

uniform mat4 transMatrix;
uniform mat4 Vmat;
uniform mat4 Pmat;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main()
{
	vec4 worldpos = transMatrix*pos;

	
	fragPos = worldpos.xyz;
	
	o_normal = (transMatrix*vec4(inNormal, 1)).xyz;
	
	o_toLight = normalize(lightPosition - worldpos.xyz);
	o_toCamera = normalize(cameraPosition - worldpos.xyz);
	
	texCoord = inCoord*10.0f;
	
	gl_Position = Projection*View*worldpos;
}