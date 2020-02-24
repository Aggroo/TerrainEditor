#version 430
layout(location=0) in vec4 pos;
layout(location=1) in vec3 inNormal;
layout(location=2) in vec2 inCoord;
layout(location=3) in vec4 tangent;
layout(location=4) in vec4 biNormal;
layout(location=7) in vec4 inWeight;
layout(location=8) in vec4 inJointIndex;

layout(location=0) out vec2 texCoord;

out vec3 o_normal;
out vec3 o_toLight;
out vec3 o_toCamera;
//out mat3 TBN;

uniform mat4 transMatrix;
uniform mat4 VPmat;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform	mediump int BoneCount;
uniform highp mat4 BoneMatrixArray[96];


void main()
{
    mediump ivec4 boneIndex = ivec4(inJointIndex);
    mediump vec4 boneWeights = inWeight;

    highp vec4 position = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 objectNormal = vec4(0.0, 0.0, 0.0, 0.0);

    for(lowp int i = 0; i < 4; i++)
    {

        highp mat4 boneMatrix = BoneMatrixArray[boneIndex.x];
        mat4 normalMatrix = transpose(BoneMatrixArray[boneIndex.x]);

        position += boneMatrix * pos * boneWeights.x;
        objectNormal += normalMatrix * vec4(inNormal,0.0) * boneWeights.x;

        // "rotate" the vectors
        boneIndex = boneIndex.yzwx;
        boneWeights = boneWeights.yzwx;
    }

	vec4 worldpos = transMatrix*position;

    o_normal = normalize(transMatrix * vec4(objectNormal.xyz, 0.0)).xyz;

/*	vec3 T = normalize(vec3(transMatrix * tangent));
    vec3 B = normalize(vec3(transMatrix * biNormal));
    vec3 N = normalize(transMatrix * vec4(objectNormal.xyz, 0.0)).xyz;

	TBN = mat3(T,B,N);*/

	o_toLight = normalize(lightPosition - worldpos.xyz);
	o_toCamera = normalize(cameraPosition - worldpos.xyz);
	
	texCoord = inCoord;
	
	gl_Position = VPmat*worldpos;
}