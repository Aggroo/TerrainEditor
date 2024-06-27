
in vec3 fragPos;
in vec2 texCoord;

in mat3 o_normal;
in vec3 o_toLight;
in vec3 o_toCamera;

layout(location = 0) out vec4 resColor;
layout(location = 1) out vec3 normalColor;
layout(location = 2) out vec3 specularOut;
layout(location = 3) out vec3 roughnessOut;

uniform sampler2D AlbedoMap;
uniform sampler2D NormalMap;
uniform sampler2D SpecularMap;
uniform sampler2D RoughnessMap;
uniform samplerCube environmentMap;
uniform samplerCube irradianceMap;
uniform sampler2D brdfLUT;

#include("common/lights.glsl")

void main()
{
	uint tileLights = 512;
	
	// Determine which tile this pixel belongs to
	ivec2 location = ivec2(gl_FragCoord.xy);
	ivec2 tileID = location / ivec2(TILE_SIZE, TILE_SIZE);
	uint index = tileID.y * LightTileWorkGroups.x + tileID.x;
	vec4 albedo = texture(AlbedoMap,texCoord);
	vec3 normal = (texture(NormalMap, texCoord).rgb * 2.0f) - 1.0f;
	//vec3 spec = texture(SpecularMap, texCoord).rgb;
	float metallic = 1.0 - texture(SpecularMap, texCoord).r;
	float roughness = texture(RoughnessMap, texCoord).r;

	//vec3 Color = texture(AlbedoMap, vec2(uv.x,1.0-uv.y)).rgb;
	
    vec3 N = normalize(o_normal * normal);
	vec3 V = normalize(o_toCamera);
	vec3 R = reflect(-V, N); 
	
	//F0 as 0.04 will usually look good for all dielectric (non-metal) surfaces
	vec3 F0 = vec3(0.04);
	//for metallic surfaces we interpolate between F0 and the albedo value with metallic value as our lerp weight
	F0 = mix(F0, albedo.rgb, metallic);
	
	vec3 lo = vec3(0.0f);
	
	/// Loop for Point Lights
	uint offset = index * tileLights;
		//Calculate lights
	//CalculatePointLights(lo, V, N, F0, vec4(albedo.rgb, 1.0), metallic, roughness, offset, tileLights);
	//CalculateSpotLights(lo, V, N, F0, vec4(albedo.rgb, 1.0), metallic, roughness, offset, tileLights);
	
	float cosLo = max(0.0, dot(N, V));
	
	// ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(cosLo, F0, roughness);

    vec3 kD = vec3(1.0) - F;
	kD *= 1.0 - metallic;
	
	vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo.rgb;
	
	const float MAX_REFLECTION_LOD = 8.0;
    vec3 prefilteredColor = textureLod(environmentMap, R,  roughness * MAX_REFLECTION_LOD).rgb;   
	vec2 envBRDF  = texture(brdfLUT, vec2(cosLo, roughness)).rg;	
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);
	
	vec3 ambient = (kD * diffuse + specular);
	
	vec3 color = ambient + lo;

	//color = color / (color + vec3(1.0f));
	//color = pow(color, vec3(screenGamma));
	
	if(albedo.a < 0.5)
		discard;

	resColor = vec4(color, 1.0);
	normalColor = N;
	specularOut.rgb = vec3(metallic);
	roughnessOut = vec3(roughness);
}
