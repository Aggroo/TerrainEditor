#version 430
layout(location=0) in vec3 fragPos;
layout(location=1) in vec2 uv;

in vec3 o_normal;
in vec3 o_toLight;
in vec3 o_toCamera;

layout(location = 0) out vec4 resColor;
//layout(location = 1) out vec3 normalColor;
uniform sampler2D textures[4];
//uniform sampler2D NormalMap;

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

void main()
{
	//vec3 normal = texture(NormalMap, vec2(uv.x/10.0, 1.0-(uv.y/10.0))).rgb;
	
	vec3 L = normalize(o_toLight);
    vec3 V = normalize(o_toCamera);
    vec3 N = normalize(o_normal);
	
	vec3 Iamb = ambientLighting();
    vec3 Idif = diffuseLighting(N, L);
    vec3 Ispe = specularLighting(N, L, V);
	
	vec4 vTexColor = vec4(0.0);
   
	float fScale = fragPos.y/60.0f;

	const float fRange1 = 0.01f;
	const float fRange2 = 0.02f;
	const float fRange3 = 0.12f;
	const float fRange4 = 0.2f;
	const float fRange5 = 0.42f;
	const float fRange6 = 0.5f;

	if(fScale >= 0.0 && fScale <= fRange1)vTexColor = texture2D(textures[0], uv);
	else if(fScale <= fRange2)
	{
	  fScale -= fRange1;
	  fScale /= (fRange2-fRange1);
	  
	  float fScale2 = fScale;
	  fScale = 1.0-fScale; 
	  
	  vTexColor += texture2D(textures[0], uv)*fScale;
	  vTexColor += texture2D(textures[1], uv)*fScale2;
	}
	else if(fScale <= fRange3) vTexColor = texture2D(textures[1], uv);
	else if(fScale <= fRange4)
	{
	  fScale -= fRange3;
	  fScale /= (fRange4-fRange3);
	  
	  float fScale2 = fScale;
	  fScale = 1.0-fScale; 
	  
	  vTexColor += texture2D(textures[1], uv)*fScale;
	  vTexColor += texture2D(textures[2], uv)*fScale2;      
	}
	else if(fScale <= fRange5) vTexColor = texture2D(textures[2], uv); 
	else if(fScale <= fRange6)
	{
	  fScale -= fRange5;
	  fScale /= (fRange6-fRange5);
	  
	  float fScale2 = fScale;
	  fScale = 1.0-fScale; 
	  
	  vTexColor += texture2D(textures[2], uv)*fScale;
	  vTexColor += texture2D(textures[3], uv)*fScale2;     
	}
	else vTexColor = texture2D(textures[3], uv); 
	
	//vec3 Color = texture(AlbedoMap, vec2(uv.x,1.0-uv.y)).rgb;
	
	resColor.xyz = vTexColor.xyz*(Iamb+Idif+Ispe);
	resColor.a = 1;
	//normalColor = N;
}

