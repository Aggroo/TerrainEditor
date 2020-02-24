
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

void main()
{
	mat4 view = mat4(mat3(View));
    TexCoords = aPos;
    vec4 pos = Projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
