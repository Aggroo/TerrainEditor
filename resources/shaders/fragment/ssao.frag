out float FragColor;

in vec2 TexCoords;

// parameters
layout (std140, binding = 1) uniform SSAOOptions
{
	int kernelSize;
	float radius;
	float bias;
};

uniform sampler2D depth;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];


// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(ScreenSize.x/4.0, ScreenSize.y/4.0); 

void main()
{
    // get input for SSAO algorithm
    vec3 fragPos = texture(depth, TexCoords).xyz;
    vec3 normal =  texture(gNormal, TexCoords).rgb;
    vec3 randomVec = texture(texNoise, TexCoords * noiseScale).xyz;
    // create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // get sample position
        vec3 singleSample = TBN * samples[i]; // from tangent to view-space
        singleSample = fragPos + singleSample * radius; 
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(singleSample, 1.0);
        offset = Projection * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        // get sample depth
        float sampleDepth = texture(depth, offset.xy).z; // get depth value of kernel sample
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= singleSample.z + bias ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion = 1.0 - (occlusion / kernelSize);
    FragColor = pow(occlusion, 2.0);
}