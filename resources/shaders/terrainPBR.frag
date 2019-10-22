
in vec3 fragPos;
in vec2 texCoord;

in mat3 o_normal;
in vec3 o_toLight;
in vec3 o_toCamera;

layout(location = 0) out vec4 resColor;
layout(location = 1) out vec3 normalColor;
layout(location = 2) out vec3 specularOut;
layout(location = 3) out vec3 roughnessOut;

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

uniform sampler2D textures[3];
uniform sampler2D normals[3];
uniform sampler2D specular[3];
uniform sampler2D roughness[3];
uniform sampler2D splat;
uniform samplerCube environmentMap;
uniform samplerCube irradianceMap;
uniform sampler2D brdfLUT;

uniform float u_matShininess; // = 64;

const float kPi = 3.14159265;

#include("lights.frag")

vec4 GetTexture(int index, in vec3 uvwPos, in vec3 weights)
{
	return vec4(weights.xxx * texture2D(textures[index], uvwPos.yz).rgb +
				weights.yyy * texture2D(textures[index], uvwPos.zx).rgb +
				weights.zzz * texture2D(textures[index], uvwPos.xy).rgb, 
				weights.x * texture2D(textures[index], uvwPos.yz).a + 
				weights.y * texture2D(textures[index], uvwPos.zx).a + 
				weights.z * texture2D(textures[index], uvwPos.xy).a);
}

vec3 GetSpecular(int index, in vec3 uvwPos, in vec3 weights)
{
	return vec3(weights.xxx * texture2D(specular[index], uvwPos.yz).rgb +
				weights.yyy * texture2D(specular[index], uvwPos.zx).rgb +
				weights.zzz * texture2D(specular[index], uvwPos.xy).rgb).rgb;
}

vec3 GetRoughness(int index, in vec3 uvwPos, in vec3 weights)
{
	return vec3(weights.xxx * texture2D(roughness[index], uvwPos.yz).rgb +
				weights.yyy * texture2D(roughness[index], uvwPos.zx).rgb +
				weights.zzz * texture2D(roughness[index], uvwPos.xy).rgb).rgb;
}

vec3 GetNormal(int index, in vec3 uvwPos)
{
	vec3 blend_weights = abs( o_normal[2].xyz );   // Tighten up the blending zone:
	blend_weights = (blend_weights - 0.2) * 7;
	blend_weights = max(blend_weights, 0);      // Force weights to sum to 1.0 (very important!)
	blend_weights /= (blend_weights.x + blend_weights.y + blend_weights.z ).xxx;
	
	vec2 bumpFetch0 = texture2D(normals[index], uvwPos.yz).xy - 0.5;
	vec2 bumpFetch1 = texture2D(normals[index], uvwPos.zx).xy - 0.5;
	vec2 bumpFetch2 = texture2D(normals[index], uvwPos.xy).xy - 0.5;
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
float scaleContrast(float value, float contrast) 
{     
	return clamp((value-0.5)*contrast+0.5, 0.0, 1.0); 
} 

float hash( float n ) { return fract(sin(n) * 753.5453123); }

float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f * f * (3.0 - 2.0 * f);
	
    float n = p.x + p.y * 157.0 + 113.0 * p.z;
    return mix(mix(mix( hash(n +   0.0), hash(n +   1.0), f.x),
                   mix( hash(n + 157.0), hash(n + 158.0), f.x), f.y),
               mix(mix( hash(n + 113.0), hash(n + 114.0), f.x),
                   mix( hash(n + 270.0), hash(n + 271.0), f.x), f.y), f.z);
}

// Fractal Brownian Motion Noise
float fbm(vec3 pp){
    float f = 0.0;
    mat3 m = mat3( 0.00,  0.80,  0.60,
                  -0.80,  0.36, -0.48,
                  -0.60, -0.48,  0.64 ) * 2;
    f  = 0.5000 * noise( pp ); pp = m*pp;
    f += 0.2500 * noise( pp ); pp = m*pp;
    f += 0.1250 * noise( pp ); pp = m*pp;
    f += 0.0625 * noise( pp ); pp = m*pp;
    f += 0.03125 * noise( pp ); pp = m*pp;
    f += 0.0150625 * noise( pp ); pp = m*pp;
    return f;
};

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
	
	vec3 norm = o_normal[2].xyz;
	vec3 weights =  norm*norm;
	
	vec4 tex0 = GetTexture(0, texUv0Multiplier * fragPos, weights);			
	vec4 tex1 = GetTexture(1, texUv1Multiplier * fragPos, weights);					
	vec4 tex2 = GetTexture(2, texUv2Multiplier * fragPos, weights);		
	
	vec3 normal0 = GetNormal(0, texUv0Multiplier * fragPos);
	vec3 normal1 = GetNormal(1, texUv1Multiplier * fragPos);
	vec3 normal2 = GetNormal(2, texUv2Multiplier * fragPos);
	
	vec3 specular0 = GetSpecular(0, texUv0Multiplier * fragPos, weights);
	vec3 specular1 = GetSpecular(1, texUv1Multiplier * fragPos, weights);
	vec3 specular2 = GetSpecular(2, texUv2Multiplier * fragPos, weights);
	
	vec3 roughness0 = GetRoughness(0, texUv0Multiplier * fragPos, weights);
	vec3 roughness1 = GetRoughness(1, texUv1Multiplier * fragPos, weights);
	vec3 roughness2 = GetRoughness(2, texUv2Multiplier * fragPos, weights);

	//vec3 normalSum = splatTex.r * normal2 + splatTex.g * normal1 + splatTex.b * normal0;
	//float specularSum = (splatTex.r * specular2 + splatTex.g * specular1 + splatTex.b * specular0).r;
	//float roughnessSum = (splatTex.r * roughness2 + splatTex.g * roughness1 + splatTex.b * roughness0).r;
	
	float slopeBlend = pow(SlopeBlending(slopeAngle, 1-norm.y), hardness1);
	float slopeBlend2 = pow(1-SlopeBlending(slopeAngle2, 1-norm.y), hardness2);
	float blendAmount = clamp(slopeBlend * HeightBlending(height, heightFalloff), 0.0, 1.0);
	float blendAmount2 = pow(clamp(slopeBlend2 * HeightBlending(height2, heightFalloff2), 0.0, 1.0), hardness3);

	vec4 vTexColor = vec4(0.0);
	
	vTexColor = mix(mix(tex1,tex0, blendAmount), tex2, blendAmount2);
	float specularSum = mix(mix(specular0, specular1, blendAmount), specular2, blendAmount2).r;
	float roughnessSum = mix(mix(roughness0, roughness1, blendAmount), roughness2, blendAmount2).r;
	vec3 normalSum = mix(mix(normal0, normal1, blendAmount), normal2, blendAmount2);
	
	vec3 L = normalize(o_toLight);
    vec3 V = normalize(o_toCamera);
    vec3 N = normalize(o_normal[2] + normalSum);
	vec3 R = reflect(-V, N); 
	
	//F0 as 0.04 will usually look good for all dielectric (non-metal) surfaces
	vec3 F0 = vec3(0.04);
	//for metallic surfaces we interpolate between F0 and the albedo value with metallic value as our lerp weight
	F0 = mix(F0, vTexColor.rgb, specularSum);
	
	vec3 lo = vec3(0.0f, 0.0f, 0.0f);
	
	/// Loop for Point Lights
	uint offset = index * tileLights;   
	for (uint i = 0; i < tileLights && visiblePointLightIndicesBuffer.data[offset + i].index != -1; i++)
	{
		uint lightIndex = visiblePointLightIndicesBuffer.data[offset + i].index;
		PointLight light = pointLightBuffer.data[lightIndex];
		
		vec3 L = light.position.xyz - fragPos.xyz;

		float lightDistance = length(L);
		float lightRadius = light.radiusAndPadding.x;
		
		if( lightDistance < lightRadius )
		{
			float x = lightDistance / lightRadius;
            // fast inverse squared falloff for a bit more accurate falloff. This is only approximative though
			//This is borrowed from AMD and their DX11 example of spotlights
            // -(1/k)* (1-(k+1) / (1+k*x^2))
            // k=20: -(1/20)*(1 - 21/(1+20*x^2))
            float attenuation = -0.05 + 1.05/(1+20*x*x);
			
			vec3 radiance = light.color.rgb * attenuation;
			
			L = normalize(L);
			vec3 H = normalize(V + L);
			
			//if we are using IBL we should use dot(N,V) for cosTheta. 
			//The correct way of doing it for direct lighting is using the halfway H for each lightsource
			vec3 F = fresnelSchlickRoughness(max(dot(H, V), 0.0), F0, roughnessSum);

			//F denotes the specular contribution of any light that hits the surface
			//We set kS (specular) to F and because PBR requires the condition that our equation is
			//energy conserving, we can set kD (diffuse contribution) to 1 - kS directly
			//as kS represents the energy of light that gets reflected, the remeaining energy gets refracted, resulting in our diffure term
			vec3 kS = F;
			vec3 kD = vec3(1.0f) - kS;

			//Fully metallic surfaces won't refract any light
			kD *= (1.0f - specularSum);			
						
			float NdotL = max(dot(L, N), 0.0f);
			float NdotV = max(dot(V, N), 0.0f);
			
			float NDF = NormalDistributionGGX(N, H, roughnessSum);
			float G = GeometrySmith(NdotV, NdotL, roughnessSum);
			
			//Calculate Cook-Torrance BRDF
			vec3 nominator = NDF * G * F;
			float denominator = 4 * NdotV * NdotL + 0.001f; //We add 0.001f in case dot ends up becoming zero.
			vec3 brdf = nominator / denominator;
			
			lo += (kD * vTexColor.rgb / PI) * radiance * NdotL;
		}
		
	}

	/// Loop for SpotLights
	for (uint i = 0; i < tileLights && visibleSpotLightIndicesBuffer.data[offset + i].index != -1; i++)
	{
		uint lightIndex = visibleSpotLightIndicesBuffer.data[offset + i].index;
		SpotLight light = spotLightBuffer.data[lightIndex];
		
		vec3 spotDir;
		spotDir.xy = light.params.xy;
		
		//Reconstruct z component
		//Because we know that our direction is normalized, we can easily calculate Z. however, we still don't know the sign of it.
		spotDir.z = sqrt(1 - spotDir.x*spotDir.x - spotDir.y*spotDir.y);
		//sign bit for cone is used for storing the sign of direction Z
		spotDir.z = (light.params.z > 0) ? spotDir.z : -spotDir.z;
		//Spotlight direction is now reconstructed
		
		//We can reconstruct position of spotlight by knowing that the top of the cone will be the 
		//bounding spheres radius away from it's center in the direction of the spotlight 
		vec3 spotPos = light.centerAndRadius.xyz - ((light.colorAndCenterOffset.a) * spotDir);
		
		/// Light Direction
		vec3 L = spotPos - fragPos.xyz;
		float distance = length(L);
		//As we already have distance, we can normalize by dividing by it.
		//normalize
		L = L / distance;
		
		float cosineOfCurrentConeAngle = dot(-L, spotDir);
		
		//Falloff radius gives us more freedom to fiddle with attenuation.
		float falloffRadius = light.params.w;
		
		//params.z is angle
		float cosineOfConeAngle = (light.params.z > 0) ? light.params.z : -light.params.z;
		
		// If we're outside of distance or not within the cone, we can just discard doing any more work
		if (distance < falloffRadius && cosineOfCurrentConeAngle > cosineOfConeAngle)
        {
			float radialAttenuation = (cosineOfCurrentConeAngle - cosineOfConeAngle) / (1.0 - cosineOfConeAngle);
			//square the attenuation
			radialAttenuation = radialAttenuation * radialAttenuation;
			
			float x = distance / falloffRadius;
			// fast inverse squared falloff for a bit more accurate falloff. This is only approximative though
			//This is borrowed from AMD and their DX11 example of spotlights
            // -(1/k)* (1-(k+1) / (1+k*x^2))
            // k=20: -(1/20)*(1 - 21/(1+20*x^2))
			float falloff = -0.05 + 1.05/(1+20*x*x);
			
			vec3 H = normalize(V + L);
			
			vec3 radiance = light.colorAndCenterOffset.rgb * radialAttenuation * falloff;
			
			//if we are using IBL we should use dot(N,V) for cosTheta. 
			//The correct way of doing it for direct lighting is using the halfway H for each lightsource
			vec3 F = fresnelSchlickRoughness(max(dot(H, V), 0.0), F0, roughnessSum);

			//F denotes the specular contribution of any light that hits the surface
			//We set kS (specular) to F and because PBR requires the condition that our equation is
			//energy conserving, we can set kD (diffuse contribution) to 1 - kS directly
			//as kS represents the energy of light that gets reflected, the remeaining energy gets refracted, resulting in our diffure term
			vec3 kS = F;
			vec3 kD = vec3(1.0f) - kS;
			
			//Fully metallic surfaces won't refract any light
			kD *= 1.0f - specularSum;			
						
			float NdotL = max(dot(L, N), 0.0f);
			float NdotV = max(dot(V, N), 0.0f);
			
			float NDF = NormalDistributionGGX(N, H, roughnessSum);
			float G = GeometrySmith(NdotV, NdotL, roughnessSum);
			
			//Calculate Cook-Torrance BRDF
			vec3 nominator = NDF * G * F;
			float denominator = 4 * NdotV * NdotL + 0.001f; //We add 0.001f in case dot ends up becoming zero.
			vec3 brdf = nominator / denominator;
			
			lo += (kD * vTexColor.rgb / PI + brdf) * radiance * NdotL;
		}
	}
	
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
	specularOut.rgb = specular;
	roughnessOut = vec3(roughnessSum);
}

