
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

layout (std140, binding = 1) uniform TerrainVariables
{
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

uniform float heightScale;

uniform float u_matShininess; // = 64;

const float kPi = 3.14159265;

#include("lights.frag")

vec3 filterNormal(in vec2 uv, in float texelSize, in float texelAspect) 
{     
	vec4 h;
	h[0] = textureLod(heightmap, uv + texelSize*vec2( 0,-1), 0).r * texelAspect;
	h[1] = textureLod(heightmap, uv + texelSize*vec2(-1, 0), 0).r * texelAspect;
	h[2] = textureLod(heightmap, uv + texelSize*vec2( 1, 0), 0).r * texelAspect;
	h[3] = textureLod(heightmap, uv + texelSize*vec2( 0, 1), 0).r * texelAspect;
	
	vec3 n;
	n.z = h[0] - h[3];
	n.x = h[1] - h[2];
	n.y = 2;
	return normalize(n);
} 

vec4 GetTexture(int index, in vec3 uvwPos, in vec3 weights)
{
	return vec4(weights.xxx * texture2D(AlbedoMap[index], uvwPos.yz).rgb +
				weights.yyy * texture2D(AlbedoMap[index], uvwPos.zx).rgb +
				weights.zzz * texture2D(AlbedoMap[index], uvwPos.xy).rgb, 
				weights.x * texture2D(AlbedoMap[index], uvwPos.yz).a + 
				weights.y * texture2D(AlbedoMap[index], uvwPos.zx).a + 
				weights.z * texture2D(AlbedoMap[index], uvwPos.xy).a);
}

vec3 GetSpecular(int index, in vec3 uvwPos, in vec3 weights)
{
	return vec3(weights.xxx * texture2D(SpecularMap[index], uvwPos.yz).rgb +
				weights.yyy * texture2D(SpecularMap[index], uvwPos.zx).rgb +
				weights.zzz * texture2D(SpecularMap[index], uvwPos.xy).rgb).rgb;
}

vec3 GetRoughness(int index, in vec3 uvwPos, in vec3 weights)
{
	return vec3(weights.xxx * texture2D(RoughnessMap[index], uvwPos.yz).rgb +
				weights.yyy * texture2D(RoughnessMap[index], uvwPos.zx).rgb +
				weights.zzz * texture2D(RoughnessMap[index], uvwPos.xy).rgb).rgb;
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
	return 1 - (clamp(worldNormal - angle, 0.0, 1.0) * (1.0/(1 - angle)));
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
	
	float tSize = 1.0 / float(textureSize(heightmap,0));
	vec3 norm = filterNormal(texCoord, tSize, heightScale);
	vec3 weights = norm*norm;
	
	vec4 tex0 = GetTexture(0, texUv0Multiplier * fragPos, weights);			
	vec4 tex1 = GetTexture(1, texUv1Multiplier * fragPos, weights);
	vec4 tex2 = GetTexture(2, texUv2Multiplier * fragPos, weights);		
	
	vec3 normal0 = GetNormal(0, texUv0Multiplier * fragPos, norm);
	vec3 normal1 = GetNormal(1, texUv1Multiplier * fragPos, norm);
	vec3 normal2 = GetNormal(2, texUv2Multiplier * fragPos, norm);
	
	vec3 SpecularMap0 = GetSpecular(0, texUv0Multiplier * fragPos, weights);
	vec3 SpecularMap1 = GetSpecular(1, texUv1Multiplier * fragPos, weights);
	vec3 SpecularMap2 = GetSpecular(2, texUv2Multiplier * fragPos, weights);
	
	vec3 RoughnessMap0 = GetRoughness(0, texUv0Multiplier * fragPos, weights);
	vec3 RoughnessMap1 = GetRoughness(1, texUv1Multiplier * fragPos, weights);
	vec3 RoughnessMap2 = GetRoughness(2, texUv2Multiplier * fragPos, weights);

	//vec3 NormalMapum = splatTex.r * normal2 + splatTex.g * normal1 + splatTex.b * normal0;
	//float SpecularMapSum = (splatTex.r * SpecularMap2 + splatTex.g * SpecularMap1 + splatTex.b * SpecularMap0).r;
	//float RoughnessMapSum = (splatTex.r * RoughnessMap2 + splatTex.g * RoughnessMap1 + splatTex.b * RoughnessMap0).r;
	
	float slopeBlend = pow(SlopeBlending(slopeAngle, 1-(norm.y)), hardness1);
	float slopeBlend2 = pow(overlayBlend(SlopeBlending(slopeAngle2, 1-(norm.y + normal1.y)), texture2D(RoughnessMap[1], texCoord*40.0).r, 0.8), hardness2);
	float blendAmount = clamp(slopeBlend*(1-HeightBlending(height, heightFalloff)), 0.0, 1.0);
	float blendAmount2 = clamp(slopeBlend2*HeightBlending(height2, heightFalloff2), 0.0, 1.0);

	vec4 vTexColor = tex1 * (1-blendAmount) * (1-blendAmount2)+ tex0 * blendAmount + tex2 * blendAmount2;
	float specularSum = (SpecularMap1 * (1-blendAmount) * (1-blendAmount2) + SpecularMap0 * blendAmount + SpecularMap2 * blendAmount2).r;
	float roughnessSum = (RoughnessMap1 * (1-blendAmount) * (1-blendAmount2) + RoughnessMap0 * blendAmount + RoughnessMap2 * blendAmount2).r;
	vec3 normalSum = (normal1 * (1-blendAmount) * (1-blendAmount2) + normal0 * blendAmount + normal2 * blendAmount2);
	
	vec3 L = normalize(o_toLight);
    vec3 V = normalize(o_toCamera);
    vec3 N = normalize(norm + normalSum);
	vec3 R = reflect(-V, N); 
	
	//F0 as 0.04 will usually look good for all dielectric (non-metal) surfaces
	vec3 F0 = vec3(0.04);
	//for metallic surfaces we interpolate between F0 and the AlbedoMap value with metallic value as our lerp weight
	F0 = mix(F0, vTexColor.rgb, specularSum);
	
	vec3 lo = vec3(0.0f, 0.0f, 0.0f);
	
	
	uint offset = index * tileLights;   
	
	//Calculate lights
	CalculatePointLights(lo, V, N, F0, vTexColor, specularSum, roughnessSum, offset, tileLights);
	CalculateSpotLights(lo, V, N, F0, vTexColor, specularSum, roughnessSum, offset, tileLights);
	
	float cosLo = max(0.0, dot(N, V));
	
	// ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(cosLo, F0, roughnessSum);

    vec3 kD = mix(vec3(1.0) - F, vec3(0.0), specularSum);
	
	vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * vTexColor.rgb;
	
	const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(environmentMap, R,  roughnessSum * MAX_REFLECTION_LOD).rgb;   
	vec2 envBRDF  = texture(brdfLUT, vec2(cosLo, roughnessSum)).rg;	
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);
	
	vec3 ambient = (kD * diffuse + specular) ;
	
	color.rgb = ambient + lo;

	resColor.xyz = color.rgb;
	resColor.a = 1;
	normalColor = N;
	SpecularMapOut.rgb = specular;
	RoughnessMapOut = vec3(roughnessSum);
}

