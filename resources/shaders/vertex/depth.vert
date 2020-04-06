layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout(location=3) in vec3 tangent;
layout(location=4) in vec3 biNormal;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;


void main()
{
    vec4 worldPos = Model * vec4(aPos, 1.0);
    FragPos = (worldPos).xyz; 
    TexCoords = aTexCoords;
    
    mat3 normalMatrix = mat3(Model);
    Normal = normalMatrix * aNormal;
	
    gl_Position = Projection * View * worldPos;
}
