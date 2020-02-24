#version 430
layout(location=1) in vec2 pos;
layout(location=0) in vec2 inCoord;
layout(location=0) out vec2 texCoord;

void main()
{
	gl_Position = vec4(pos,-10,1);

	texCoord = inCoord;
}