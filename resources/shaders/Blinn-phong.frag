
in vec3 fragPos;
in vec2 texCoord;

in vec3 o_normal;
in vec3 o_toLight;
in vec3 o_toCamera;

layout(location = 0) out vec4 resColor;
//layout(location = 1) out vec3 normalColor;
uniform sampler2D textures[3];
uniform sampler2D splat;

uniform float tex0UvMultiplier;
uniform float tex1UvMultiplier;
uniform float tex2UvMultiplier;

// parameters of the light and possible values
uniform vec3 u_lightAmbientIntensity; // = vec3(0.6, 0.3, 0);
uniform vec3 u_lightDiffuseIntensity; // = vec3(1, 0.5, 0);
uniform vec3 u_lightSpecularIntensity; // = vec3(0, 1, 0);

// parameters of the material and possible values
uniform vec3 u_matAmbientReflectance; // = vec3(1, 1, 1);
uniform vec3 u_matDiffuseReflectance; // = vec3(1, 1, 1);
uniform vec3 u_matSpecularReflectance; // = vec3(1, 1, 1);
uniform float u_matShininess; // = 64;

vec3 ambientLighting()
{
   return u_matAmbientReflectance * u_lightAmbientIntensity;
}

// returns intensity of diffuse reflection
vec3 diffuseLighting(in vec3 N, in vec3 L)
{
   // calculation as for Lambertian reflection
   float diffuseTerm = clamp(dot(N, L), 0, 1) ;
   return u_matDiffuseReflectance * u_lightDiffuseIntensity * diffuseTerm;
}

// returns intensity of specular reflection
vec3 specularLighting(in vec3 N, in vec3 L, in vec3 V)
{
   float specularTerm = 0;

   // calculate specular reflection only if
   // the surface is oriented to the light source
   if(dot(N, L) > 0)
   {
      // half vector
      vec3 H = normalize(L + V);
      specularTerm = pow(max(dot(H, N), 0.0), u_matShininess);
   }
   return u_matSpecularReflectance * u_lightSpecularIntensity * specularTerm;
}

vec4 GetTexture(int index, in vec3 uvwPos, in vec3 weights)
{

	return vec4(weights.xxx * texture2D(textures[index], uvwPos.yz).rgb +
				weights.yyy * texture2D(textures[index], uvwPos.zx).rgb +
				weights.zzz * texture2D(textures[index], uvwPos.xy).rgb, 
				weights.x * texture2D(textures[index], uvwPos.yz).a + 
				weights.y * texture2D(textures[index], uvwPos.zx).a + 
				weights.z * texture2D(textures[index], uvwPos.xy).a);
}

vec4 blend(vec4 texture1, float a1, vec4 texture2, float a2)
{
    float depth = 0.2;
    float ma = max(texture1.a + a1, texture2.a + a2) - depth;

    float b1 = max(texture1.a + a1 - ma, 0);
    float b2 = max(texture2.a + a2 - ma, 0);

    return vec4((texture1.rgb * b1 + texture2.rgb * b2) / (b1 + b2), 1.0);
}

void main()
{
	//vec3 normal = texture(NormalMap, vec2(uv.x/10.0, 1.0-(uv.y/10.0))).rgb;
	vec3 splatTex = texture(splat, vec2(texCoord.x/10.0, 1.0-(texCoord.y/10.0))).rgb;

	
	vec3 L = normalize(o_toLight);
    vec3 V = normalize(o_toCamera);
    vec3 N = normalize(o_normal);
	
	vec3 Iamb = ambientLighting();
    vec3 Idif = diffuseLighting(N, L);
    vec3 Ispe = specularLighting(N, L, V);
   
	float fScale = fragPos.y/60.0f;

	//float blendAmount;
	//float slope = 1.0-o_normal.y;
	vec3 weights = o_normal * o_normal;
	
	vec4 tex0 = GetTexture(0, tex0UvMultiplier * fragPos, weights);			
	vec4 tex1 = GetTexture(1, tex1UvMultiplier * fragPos, weights);					
	vec4 tex3 = GetTexture(2, tex2UvMultiplier * fragPos, weights);					

	
	//vTexColor = blend(snow, splatTex.r, rock, splatTex.g) + splatTex.b * grass;
	vec4 vTexColor = splatTex.r * tex3 + splatTex.g * tex1 + splatTex.b * tex0;
	
	vec4 vFinalTexColor = vTexColor;
	
	//vec3 Color = texture(AlbedoMap, vec2(uv.x,1.0-uv.y)).rgb;
	
	resColor.xyz = vFinalTexColor.xyz*(Iamb+Idif+Ispe);
	resColor.a = 1;
	//normalColor = N;
}

