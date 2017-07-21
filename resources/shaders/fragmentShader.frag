#version 430
layout(location=0) in vec2 uv;
out vec4 Color;
uniform sampler2D texSampler;
void main()
{
	vec4 tex = texture(texSampler,vec2(uv.x,1.0-uv.y)).rgba;
    vec4 white = vec4(1,1,1,1);
	Color = white;
}