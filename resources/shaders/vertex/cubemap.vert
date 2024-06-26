layout (location = 0) in vec3 aPos;

out vec3 WorldPos;

void main()
{
	mat4 view = mat4(mat3(View));
    WorldPos = aPos;
    gl_Position =  Projection * view * vec4(aPos, 1.0);
}