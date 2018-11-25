
struct PointLight
{
	vec4 position;
	vec4 color;
	vec4 radiusAndPadding;
};

struct SpotLight 
{
	//centerAndRadius:
	//Contains the center of the cone bounding sphere and the radius of it's sphere.
	vec4 centerAndRadius;
	
	//colorAndCenterOffset:
	//Color of the light and center offset
	//if angle is greater than 50 (tan(50) = 1) the distance from the cornerpoints to the center is greater than the distance to the spotlight position.
	//In this special case we need to apply a center offset for the position of the spotlight
    vec4 colorAndCenterOffset;
	
	//params:
	//Contains direction, angle and the falloff radius of the cone
	//XY is direction (we can reconstruct Z because we know the direction is normalized and we know the sign of Z dir)
	//Z is cosine of cone angle and lightdir Z sign (Sign bit is used to store sign for the z component of the light direction)
	//W is falloff radius
	vec4 params;
	
	//We can reconstruct position of spotlight by knowing that the top of the cone will be the 
	//bounding spheres radius away from it's center in the direction of the spotlight 
};

struct VisibleIndex 
{
	int index;
};

// Shader storage buffer objects
layout(std430, binding = 1) readonly buffer PointLightBuffer 
{
	PointLight data[];
} pointLightBuffer;

layout(std430, binding = 11) readonly buffer SpotLightBuffer 
{
	SpotLight data[];
} spotLightBuffer;

layout(std430, binding = 3) readonly buffer VisiblePointLightIndicesBuffer 
{
	VisibleIndex data[];
} visiblePointLightIndicesBuffer;

layout(std430, binding = 12) readonly buffer VisibleSpotLightIndicesBuffer 
{
	VisibleIndex data[];
} visibleSpotLightIndicesBuffer;

// parameters of the light and possible values
const vec3 u_lightAmbientIntensity = vec3(0.05f, 0.05f, 0.05f);

// Attenuate the point light intensity
float attenuate(vec3 lightDirection, float radius)
{
	vec3 l = lightDirection / radius;
    float atten = max(0.0, 1.0 - dot(l,l));

	return atten;
}

//Inverse gamma
#define GAMMA 2.2f
const float screenGamma = 1.0f / GAMMA;

#define PI 3.14159265358979323846

//returns the ratio between specular and diffuse reflection or how much the surface reflects light versus how much it refracts light
//creates a rim-lighting effect, as the closer we get to zero incidence, the more the light the material will reflect.
vec3 fresnelSchlickRoughness(in float cosTheta, in vec3 F0, float roughness)
{
	//Spherical Gaussian approximation
	//return F0 + (1.0f - F0) * pow(2,((-5.55473f*cosTheta - 6.98316f) * cosTheta));
	
	//With roughness
	//return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(2,((-5.55473f*cosTheta - 6.98316f) * cosTheta));
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
	
	//Old, yields artifacts every now and then
	//return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, in vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

//We use GGX for our normal distribution function (NDF)
//This approximates the ratio of microfacets aligned to given halfway vector H
float NormalDistributionGGX(in vec3 N, in vec3 H, in float roughness)
{
	float a = pow(roughness, 4);
	
	float NdotH = max(dot(N, H), 0.0f);
	NdotH = NdotH * NdotH;
	
	float denom = (NdotH * (a - 1.0f) + 1.0f);
	denom = PI * denom * denom;
	
	return (a / (denom + 0.00001f));
}

//We use Smith with Schlick-GGX for our geometry function
//This statistically approximates the ratio of microfacets that overshadow each other
//TODO: These two functions can be optimized
float GeometrySchlickGGX(in float NdotV, in float roughness)
{
	//Optimization: Moved to GeometrySmith, so that we don't calculate it twice
	//float r = roughness + 1.0f;
	//float k = (r*r) * 0.125f; // (r^2 / 8)
	float k = roughness;
	
	float denom = NdotV * (1.0f - k) + k;
	return NdotV / denom;
}

float GeometrySmith(in float NdotV, in float NdotL, in float roughness)
{	
	//Optimization: Moved here from GeometrySchlickGGX so that we don't calculate it twice
	float r = roughness + 1.0f;
	float k = (r*r) * 0.125f; // (r^2 / 8)
	
	float ggxNV = GeometrySchlickGGX(NdotV, k);
	float ggxNL = GeometrySchlickGGX(NdotL, k);
	
	return ggxNV * ggxNL;
}
