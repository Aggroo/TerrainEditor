
in vec2 TexCoords;

layout(location = 0) out vec4 resColor;

layout (std140, binding = 1) uniform PostProcessOptions
{
	float toneMapping;
	float exposure;
	float gamma;
};

uniform sampler2D hdrBuffer;
uniform sampler2D AO;

vec3 Reinhard(in vec3 color) 
{
	return color / (vec3(1) + color);
}

const vec3 a = vec3(2.51);
const vec3 b = vec3(0.03);
const vec3 c = vec3(2.43);
const vec3 d = vec3(0.59);
const vec3 e = vec3(0.14);

vec3 saturate(vec3 val) 
{
    return max(vec3(0), min(vec3(1), val));
}

vec3 Uncharted2(in vec3 color)
{
    return saturate((color * (a * color + b)) / (color * (c * color + d) + e));
}

//== ACESFitted ===========================
//  Baking Lab
//  by MJP and David Neubelt
//  http://mynameismjp.wordpress.com/
//  All code licensed under the MIT license
//=========================================

// sRGB => XYZ => D65_2_D60 => AP1 => RRT_SAT
const mat3 ACESInputMat = mat3
(
    0.59719, 0.07600, 0.02840,
	0.35458, 0.90834, 0.13383,
	0.04823, 0.01566, 0.83777
);

// ODT_SAT => XYZ => D60_2_D65 => sRGB
const mat3 ACESOutputMat = mat3
(
     1.60475, -0.10208, -0.00327,
	 -0.53108, 1.10813, -0.07276, 
	 -0.07367, -0.00605, 1.07602
);

vec3 RRTAndODTFit(vec3 v)
{
    vec3 a = v * (v + 0.0245786f) - 0.000090537f;
    vec3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}

vec3 ACESFitted(vec3 color)
{
    color = ACESInputMat * color;

    // Apply RRT and ODT
    color = RRTAndODTFit(color);

    color = ACESOutputMat * color;

    // Clamp to [0, 1]
    color = clamp(color, 0.0, 1.0);

    return color;
}

void main()
{    
	vec3 mapped = vec3(0.0);
	float ao = texture(AO, TexCoords).r;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb * ao;
	
	//hdrColor = pow(abs(hdrColor), vec3(1.0f / gamma));
	
	hdrColor *= exposure;
  
	if(toneMapping == 0.0)
	{
		mapped = hdrColor;
	}
	else if(toneMapping == 1.0)
	{
		//ACES Tone mapping
		mapped = pow(abs(hdrColor), vec3(0.933f));
		mapped *= 1.07f;
		mapped = ACESFitted(mapped);
	}
	else if(toneMapping == 2.0)
	{
		// Uncharted2 tone mapping
		mapped = Uncharted2(hdrColor);
	}
	else if(toneMapping == 3.0)
	{
		// Reinhard tone mapping
		mapped = Reinhard(hdrColor);
	}
  
    //resColor = vec4(pow(mapped, vec3(1.0/gamma)), 1.0);
    resColor = vec4(mapped, 1.0);
}