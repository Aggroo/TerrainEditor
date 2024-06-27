
in vec3 fragPos;
in vec2 texCoord;

in mat3 o_normal;
in vec3 o_toLight;
in vec3 o_toCamera;
in mat3 model33Out;

layout(location = 0) out vec4 resColor;
layout(location = 1) out vec3 normalColor;
layout(location = 2) out vec3 SpecularMapOut;
layout(location = 3) out vec3 RoughnessMapOut;
layout(location = 4) out vec3 UVsOut;

layout (std140, binding = 1) uniform TerrainVariables
{
	float heightScale;
	float worldSizeX;
    float worldSizeZ;
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
	int numHeightmaps;
};  

uniform sampler2D AlbedoMap[3];
uniform sampler2D NormalMap[3];
uniform sampler2D SpecularMap[3];
uniform sampler2D RoughnessMap[3];
layout(binding = 15) uniform sampler2D splat;
layout(binding = 16) uniform sampler2D heightmap;
layout(binding = 17) uniform samplerCube environmentMap;
layout(binding = 18) uniform samplerCube irradianceMap;
layout(binding = 19) uniform sampler2D brdfLUT;

uniform float u_matShininess; // = 64;

const float kPi = 3.14159265;

#include("common/lights.glsl")
#include("common/common.glsl")

vec3 filterNormal(in vec2 uv, ivec3 offset, in float texelSize, in float texelAspect) 
{     
	float hl = texture2D(heightmap, (uv + texelSize * offset.xz)).r * texelAspect;
    float hr = texture2D(heightmap, (uv + texelSize * offset.yz)).r * texelAspect;
    float ht = texture2D(heightmap, (uv + texelSize * offset.zx)).r * texelAspect;
    float hb = texture2D(heightmap, (uv + texelSize * offset.zy)).r * texelAspect;
    vec3 normal = vec3(0, 0, 0);
    normal.x = (hl - hr);
    normal.y = 2.0f;
    normal.z = (ht - hb);
	return normalize(normal);
} 

// From https://iquilezles.org/articles/biplanar/
vec4 Biplanar( sampler2D sam, in vec3 p, in vec3 n, in float k )
{
    // grab coord derivatives for texturing
    vec3 dpdx = dFdx(p);
    vec3 dpdy = dFdy(p);
    n = abs(n);

    // major axis (in x; yz are following axis)
    ivec3 ma = (n.x>n.y && n.x>n.z) ? ivec3(0,1,2) :
               (n.y>n.z)            ? ivec3(1,2,0) :
                                      ivec3(2,0,1) ;
    // minor axis (in x; yz are following axis)
    ivec3 mi = (n.x<n.y && n.x<n.z) ? ivec3(0,1,2) :
               (n.y<n.z)            ? ivec3(1,2,0) :
                                      ivec3(2,0,1) ;
        
    // median axis (in x;  yz are following axis)
    ivec3 me = ivec3(3) - mi - ma;
    
    // project+fetch
    vec4 x = textureGrad( sam, vec2(   p[ma.y],   p[ma.z]), 
                               vec2(dpdx[ma.y],dpdx[ma.z]), 
                               vec2(dpdy[ma.y],dpdy[ma.z]) );
    vec4 y = textureGrad( sam, vec2(   p[me.y],   p[me.z]), 
                               vec2(dpdx[me.y],dpdx[me.z]),
                               vec2(dpdy[me.y],dpdy[me.z]) );
    
    // blend and return
    vec2 m = vec2(n[ma.x],n[me.x]);
    // optional - add local support (prevents discontinuty)
    m = clamp( (m-0.5773)/(1.0-0.5773), 0.0, 1.0 );
    // transition control
    m = pow( m, vec2(k/8.0) );
	return (x*m.x + y*m.y) / (m.x + m.y);
}

vec3 GetNormal(int index, in vec3 uvwPos, in vec3 normal)
{
	vec3 blend_weights = abs( normal );   // Tighten up the blending zone:
	blend_weights = (blend_weights - 0.2) * 7;
	blend_weights = max(blend_weights, 0);      // Force weights to sum to 1.0 (very important!)
	blend_weights /= (blend_weights.x + blend_weights.y + blend_weights.z ).xxx;
	
	vec2 bumpFetch0 = texture2D(NormalMap[index], uvwPos.yz).xy - 0.5;
	vec2 bumpFetch1 = texture2D(NormalMap[index], uvwPos.zx).xy - 0.5;
	vec2 bumpFetch2 = texture2D(NormalMap[index], uvwPos.xy).xy - 0.5;
	vec3 bump1 = vec3(0, bumpFetch0.x, bumpFetch0.y);
	vec3 bump2 = vec3(bumpFetch1.y, 0, bumpFetch1.x);
	vec3 bump3 = vec3(bumpFetch2.x, bumpFetch2.y, 0);
	
	return vec3(bump1.xyz * blend_weights.xxx +
			   bump2.xyz * blend_weights.yyy +
			   bump3.xyz * blend_weights.zzz);
}

float SlopeBlending(float angle, float worldNormal)
{
	//return 1 - worldNormal;
	return 1 - (clamp(worldNormal - angle, 0.0, 1.0) * (1.0/(1 - clamp(angle, 0.0, 0.99999))));
}

float HeightBlending(float height, float heightFalloff)
{
	return clamp((fragPos.y - (height - (heightFalloff * 0.5))) / heightFalloff, 0.0, 1.0);
}

float overlayBlend(float value1, float value2, float opacity) 
{     
	float blend = value1 < 0.5 ? 2*value1*value2 : 1 - 2*(1-value1)*(1-value2);     
	return mix(value1, blend, opacity); 
}

vec4 heightblend(vec4 input1, float height1, vec4 input2, float height2)
{
    float height_start = max(height1, height2) - 0.5f;
    float level1 = max(height1 - height_start, 0);
    float level2 = max(height2 - height_start, 0);
    return ((input1 * level1) + (input2 * level2)) / (level1 + level2);
}

vec4 heightlerp(vec4 input1, float height1, vec4 input2, float height2, float t)
{
    t = clamp(t, 0, 1);
    return heightblend(input1, height1 * (1 - t), input2, height2 * t);
}

void main()
{
	uint tileLights = 512;
	// Determine which tile this pixel belongs to
	ivec2 location = ivec2(gl_FragCoord.xy);
	ivec2 tileID = location / ivec2(TILE_SIZE, TILE_SIZE);
	uint index = tileID.y * LightTileWorkGroups.x + tileID.x;

	vec3 color = vec3(0.0, 0.0, 0.0);

	float reflectance = 0;

	//vec3 splatTex = texture(splat, vec2(texCoord.x, 1.0-(texCoord.y))).rgb;
	float tSize = 1.0 / float(textureSize(heightmap, 0));
	ivec3 normalOffset = ivec3(-1, 1, 0);
	vec3 norm = filterNormal(texCoord, normalOffset, tSize, heightScale);
	vec3 weights = norm*norm;
	
	vec4 albedo0 = Biplanar(AlbedoMap[0], texUv0Multiplier * fragPos, norm, 8.0);		
	vec4 albedo1 = Biplanar(AlbedoMap[1], texUv1Multiplier * fragPos, norm, 8.0);
	vec4 albedo2 = Biplanar(AlbedoMap[2], texUv2Multiplier * fragPos, norm, 8.0);	
	
	vec3 normal0 = GetNormal(0, texUv0Multiplier * fragPos, weights);
	vec3 normal1 = GetNormal(1, texUv1Multiplier * fragPos, weights);
	vec3 normal2 = GetNormal(2, texUv2Multiplier * fragPos, weights);
	
	vec3 specularMap0 = Biplanar(SpecularMap[0], texUv0Multiplier * fragPos, norm, 8.0).xyz;
	vec3 specularMap1 = Biplanar(SpecularMap[1], texUv1Multiplier * fragPos, norm, 8.0).xyz;
	vec3 specularMap2 = Biplanar(SpecularMap[2], texUv2Multiplier * fragPos, norm, 8.0).xyz;
	
	vec3 roughnessMap0 = Biplanar(RoughnessMap[0], texUv0Multiplier * fragPos, norm, 8.0).xyz;
	vec3 roughnessMap1 = Biplanar(RoughnessMap[1], texUv1Multiplier * fragPos, norm, 8.0).xyz;
	vec3 roughnessMap2 = Biplanar(RoughnessMap[2], texUv2Multiplier * fragPos, norm, 8.0).xyz;
	
    mat3 tbn = PlaneTBN(norm);

	float heightValue = texture2D(heightmap, texCoord + tSize).r;
	float terrainHeight = heightValue * heightScale;
	float heightCutoff = clamp(max(0, terrainHeight - height) / 25.0f, 0.0, 1.0);
	
	float slopeBlend = SlopeBlending(slopeAngle, dot(norm, vec3(0, 1, 0)));
	float slopeBlend2 = SlopeBlending(slopeAngle2, dot(norm, vec3(0, 1, 0)));
	vec4 albedoSum = vec4(0.0, 0.0, 0.0, 1.0);
	float specularSum = 0.0;
	float roughnessSum = 0.0;
	vec3 normalSum = vec3(0.0, 0.0, 0.0);
	
	vec3 blendNormal = vec3(0.0);
	
	if(heightCutoff == 0.0f)
	{
		albedoSum = albedo0 * (1.0 - slopeBlend);
		albedoSum += albedo1 * slopeBlend;
		specularSum = specularMap0.r * (1.0 - slopeBlend);
		specularSum += specularMap1.r * slopeBlend;
		roughnessSum = roughnessMap0.r * (1.0 - slopeBlend);
		roughnessSum += roughnessMap1.r * slopeBlend;
		normalSum = normal0 * (1.0 - slopeBlend);
		normalSum += normal1 * slopeBlend;
		blendNormal += normalSum;
		
		blendNormal.xy = blendNormal.xy * 2.0f - 1.0f;
		blendNormal.z = clamp(sqrt(1.0f - dot(blendNormal.xy, blendNormal.xy)), 0.0, 1.0);
		normalSum = (tbn * blendNormal);
	}
	else
	{
		albedoSum = albedo2 * (1.0 - slopeBlend2) * heightCutoff;
		albedoSum += albedo1 * slopeBlend2 * heightCutoff;
		albedoSum += albedo0 * (1.0 - slopeBlend) * (1.0-heightCutoff); 
		albedoSum += albedo1 * slopeBlend * (1.0-heightCutoff);
		
		specularSum = specularMap2.r * (1.0 - slopeBlend2) * heightCutoff;
		specularSum += specularMap1.r * slopeBlend2 * heightCutoff;
		specularSum += specularMap0.r * (1.0 - slopeBlend) * (1.0 - heightCutoff);
		specularSum += specularMap1.r * slopeBlend * (1.0 - heightCutoff);
		
		roughnessSum = roughnessMap2.r * (1.0 - slopeBlend2) * heightCutoff;
		roughnessSum += roughnessMap1.r * slopeBlend2 * heightCutoff;
		roughnessSum += roughnessMap0.r * (1.0 - slopeBlend)* (1.0 - heightCutoff);
		roughnessSum += roughnessMap1.r * slopeBlend * (1.0 - heightCutoff);
		
		normalSum = normal2 * (1.0 - slopeBlend2);
		normalSum += normal1 * slopeBlend2;
		blendNormal += normalSum * heightCutoff;
		normalSum += normal0 * (1.0 - slopeBlend);
		normalSum += normal1 * slopeBlend;
		blendNormal += normalSum * (1.0 - heightCutoff);
		
		blendNormal.xy = blendNormal.xy * 2.0f - 1.0f;
		blendNormal.z = clamp(sqrt(1.0f - dot(blendNormal.xy, blendNormal.xy)), 0.0, 1.0);
		normalSum = (tbn * blendNormal);
	}
	
	roughnessSum = 0;
	albedoSum = vec4(1.0);

	vec3 L = normalize(o_toLight);
    vec3 V = normalize(o_toCamera);
    vec3 N = normalize(normalSum);
	vec3 R = reflect(-V, N); 
	
	//F0 as 0.04 will usually look good for all dielectric (non-metal) surfaces
	vec3 F0 = vec3(0.04);
	//for metallic surfaces we interpolate between F0 and the AlbedoMap value with metallic value as our lerp weight
	F0 = mix(F0, albedoSum.rgb, specularSum);
	
	vec3 lo = vec3(0.0f, 0.0f, 0.0f);
	
	uint offset = index * tileLights;   
	
	//Calculate lights
	CalculatePointLights(lo, V, N, F0, albedoSum, specularSum, roughnessSum, offset, tileLights);
	CalculateSpotLights(lo, V, N, F0, albedoSum, specularSum, roughnessSum, offset, tileLights);
	
	float cosLo = max(0.0, dot(N, V));
	
	// ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(cosLo, F0,  roughnessSum);

    vec3 kD = vec3(1.0) - F;
	kD *= 1.0 - specularSum;
	
	vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedoSum.rgb;
	
	const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(environmentMap, R,  roughnessSum * MAX_REFLECTION_LOD).rgb;   
	vec2 envBRDF  = texture(brdfLUT, vec2(cosLo, roughnessSum)).rg;	
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);
	
	vec3 ambient = (kD * diffuse + specular);
	
	color.rgb = ambient + lo;
	
	//float slopeVal = SlopeBlending(slopeAngle, 1-(norm.y);
	//color.rgb = vec3(slopeVal, slopeVal, slopeVal);
	
	//color.rgb = vec3(slopeBlend);
	
	resColor.xyz = color.rgb;
	resColor.a = 1.0;
	normalColor = N;
	SpecularMapOut.rgb = specular;
	RoughnessMapOut = vec3(roughnessSum);
	UVsOut = vec3(texCoord.xy, float(gl_PrimitiveID + 1));
}

