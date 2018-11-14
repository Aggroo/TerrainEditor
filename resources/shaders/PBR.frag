
in vec3 fragPos;
in vec2 texCoord;

in mat3 o_normal;
in vec3 o_toLight;
in vec3 o_toCamera;

layout(location = 0) out vec4 resColor;
layout(location = 1) out vec3 normalColor;
layout(location = 2) out vec4 specularAndRoughness;

uniform sampler2D AlbedoMap;
uniform sampler2D NormalMap;
uniform sampler2D SpecularMap;
uniform sampler2D RoughnessMap;
uniform sampler2D aoMap;
uniform samplerCube environmentMap;

uniform float u_matShininess; // = 64;

const float kPi = 3.14159265;

#include("lights.frag")

void main()
{
	uint tileLights = 512;
	
	// Determine which tile this pixel belongs to
	ivec2 location = ivec2(gl_FragCoord.xy);
	ivec2 tileID = location / ivec2(TILE_SIZE, TILE_SIZE);
	uint index = tileID.y * LightTileWorkGroups.x + tileID.x;

	vec3 color = vec3(0.0, 0.0, 0.0);

	float reflectance = 0;
	
	vec4 albedo = texture(AlbedoMap,texCoord*0.1f).rgba;
	
	//Adjust to linear space
	albedo.rgb = pow(albedo.rgb, vec3(GAMMA));
		
	vec3 normal = texture(NormalMap, texCoord*0.1f).rgb;
	//vec3 spec = texture(SpecularMap, texCoord).rgb;
	float metallic = texture(SpecularMap, texCoord*0.1f).r;
	float roughness = texture(RoughnessMap, texCoord*0.1f).r;
	float ao = texture(aoMap, texCoord*0.1f).r;

	//vec3 Color = texture(AlbedoMap, vec2(uv.x,1.0-uv.y)).rgb;
	
	vec3 L = normalize(o_toLight);
    vec3 N = normalize(o_normal * ((normal*2.0f) - 1.0f));
	vec3 V = normalize(CameraPosition.xyz - fragPos.xyz);
	vec3 R = reflect(-V, N); 
	
	//F0 as 0.04 will usually look good for all dielectric (non-metal) surfaces
	vec3 F0 = vec3(0.04);
	//for metallic surfaces we interpolate between F0 and the albedo value with metallic value as our lerp weight
	F0 = mix(F0, albedo.rgb, metallic);
	
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
			vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0, roughness);

			//F denotes the specular contribution of any light that hits the surface
			//We set kS (specular) to F and because PBR requires the condition that our equation is
			//energy conserving, we can set kD (diffuse contribution) to 1 - kS directly
			//as kS represents the energy of light that gets reflected, the remeaining energy gets refracted, resulting in our diffure term
			vec3 kS = F;
			vec3 kD = vec3(1.0f) - kS;

			//Fully metallic surfaces won't refract any light
			kD *= 1.0f - metallic;			
						
			float NdotL = max(dot(L, N), 0.0f);
			float NdotV = max(dot(V, N), 0.0f);
			
			float NDF = NormalDistributionGGX(N, H, roughness);
			float G = GeometrySmith(NdotV, NdotL, roughness);
			
			//Calculate Cook-Torrance BRDF
			vec3 nominator = NDF * G * F;
			float denominator = 4 * NdotV * NdotL + 0.001f; //We add 0.001f in case dot ends up becoming zero.
			vec3 brdf = nominator / max(denominator, 0.001);
			
			lo += (kD * albedo.rgb / PI + brdf) * radiance * NdotL;
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
			vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0, roughness);

			//F denotes the specular contribution of any light that hits the surface
			//We set kS (specular) to F and because PBR requires the condition that our equation is
			//energy conserving, we can set kD (diffuse contribution) to 1 - kS directly
			//as kS represents the energy of light that gets reflected, the remeaining energy gets refracted, resulting in our diffure term
			vec3 kS = F;
			vec3 kD = vec3(1.0f) - kS;
			
			//Fully metallic surfaces won't refract any light
			kD *= 1.0f - metallic;			
						
			float NdotL = max(dot(L, N), 0.0f);
			float NdotV = max(dot(V, N), 0.0f);
			
			float NDF = NormalDistributionGGX(N, H, roughness);
			float G = GeometrySmith(NdotV, NdotL, roughness);
			
			//Calculate Cook-Torrance BRDF
			vec3 nominator = NDF * G * F;
			float denominator = 4 * NdotV * NdotL + 0.001f; //We add 0.001f in case dot ends up becoming zero.
			vec3 brdf = nominator / denominator;
			
			lo += (kD * albedo.rgb / PI + brdf) * radiance * NdotL;
		}
	}
	
	// ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlick(max(dot(N, V), 0.0), F0, roughness);
    
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;
	
	vec3 irradiance = texture(environmentMap, N).rgb;
    vec3 diffuse = irradiance * albedo.rgb;
	
	const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(environmentMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec3 specular = prefilteredColor;
	
	vec3 ambient = (kD * diffuse + specular) * ao;
	
	color.rgb = lo + ambient;

	color.rgb = color / (color + vec3(1.0f));
	color = pow(color, vec3(screenGamma));

	resColor.xyz = color.rgb;
	resColor.a = albedo.a;
	normalColor = N;
	specularAndRoughness.rgb = fresnelSchlick(max(dot(N, V), 0.0), F0, roughness);
	specularAndRoughness.a = roughness;
}
