
in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube skybox;

void main()
{    
	vec4 cubeMapColor = texture(skybox, TexCoords);
    FragColor =  cubeMapColor;
}