mat3 PlaneTBN(vec3 normal)
{
    vec3 tangent = cross(normal.xyz, vec3(0, 0, 1));
    tangent = normalize(cross(normal.xyz, tangent));
    vec3 binormal = normalize(cross(normal.xyz, tangent));
    return mat3(tangent, binormal, normal.xyz);
}