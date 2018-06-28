#include "config.h"
#include "lightserver.h"
#include "render/render/renderer.h"
#include "foundation/math/math.h"

namespace Render
{
LightServer::LightServer()
{
	// Generate our shader storage buffers
	glGenBuffers(1, &pointLightBuffer);
	glGenBuffers(1, &spotLightBuffer);
	glGenBuffers(1, &visiblePointLightIndexBuffer);
	glGenBuffers(1, &visibleSpotLightIndexBuffer);
}

void LightServer::AddPointLight(const PointLight& pLight)
{
	this->pointLights.Append(pLight);
	this->UpdatePointLightBuffer();
}

void LightServer::RemovePointLight(PointLight* light)
{
	this->pointLights.EraseSwap(light);
}

LightServer::PointLight& LightServer::GetPointLightAtIndex(const int& indx)
{
	return this->pointLights[indx];
}

void LightServer::AddSpotLight(const SpotLight& sLight)
{
	this->spotLights.Append(sLight);
}

void LightServer::RemoveSpotLight(SpotLight* light)
{
	this->spotLights.EraseSwap(light);
}

LightServer::SpotLight LightServer::CalculateSpotLight(Math::vec4 position, Math::vec4 direction, Math::vec4 color, float length, float angle)
{
	// calculate the radius of the bottom circle
	float radians = Math::Deg2Rad(angle);
	float radius = (float)tan(radians) * length;

	Math::vec4 endPoint = position + Math::vec4(direction) * length;

	/// Get perpendicular direction
	Math::vec4 m = Math::vec4::Normalize(Math::vec4::Orthogonal(direction)) * radius;
	Math::vec4 Q1 = endPoint - m;
	/// Get perpendicular, -direction
	//m = Math::vec4::normalize(Math::vec4::cross3(direction * -1.0f, position));
	Math::vec4 Q2 = endPoint + m;

	/// Calculate the Mid Point of the Sphere
	const float oneOverThree = 1.0f / 3.0f;
	Math::vec4 centerAndRadius = (position + Q1 + Q2) * oneOverThree;
	centerAndRadius[3] = 1.0f;

	GLfloat centerOffset = (centerAndRadius - position).vecLength();

	//TODO: This isn't quite right, but it works for now. the sphere radius is a bit too big of angle is greater than 50
	//if the angle is greater than 50 degrees we cant use center-position as our radius length
	if (angle > 50)
	{
		centerAndRadius[3] = ((centerAndRadius - Q1).vecLength());
	}
	else
	{
		centerAndRadius[3] = centerOffset;
	}

	Math::vec4 colorAndCenterOffset = color;
	colorAndCenterOffset[3] = centerOffset;

	SpotLight spot;
	spot.centerAndRadius = centerAndRadius;
	spot.colorAndCenterOffset = colorAndCenterOffset;

	//spot.colorAndCenterOffset.set_w(centerOffset);

	//Make sure direction is normalized
	Math::vec4 normalizedDir = Math::vec4::Normalize(direction);
	//XY direction
	spot.parameters[0] = normalizedDir.x();
	spot.parameters[1] = normalizedDir.y();
	//Z is cosine of angle and sign of direction Z component
	spot.parameters[2] = cos(radians) * Math::Sign(normalizedDir.z());
	//W is falloff radius. TEMPORARY: Set to radius for now
	spot.parameters[3] = radius;


	return spot;
}

LightServer::SpotLight& LightServer::GetSpotLightAtIndex(const int& indx)
{
	return this->spotLights[indx];
}

void LightServer::UpdatePointLightBuffer() const
{
	//Bind pointlight buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, pointLightBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, this->pointLights.Size() * sizeof(PointLight), &this->pointLights[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void LightServer::UpdateSpotLightBuffer() const
{
	//Bind spotlight buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, spotLightBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, this->spotLights.Size() * sizeof(SpotLight), &this->spotLights[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

LightServer::PointLight& LightServer::CreatePointLight(Math::vec4 position, Math::vec4 color, float radius)
{
	PointLight pLight;
	pLight.position = position;
	pLight.color = color;
	pLight.radiusAndPadding[0] = radius;
	this->AddPointLight(pLight);
	return this->pointLights[this->pointLights.Size() - 1];
}

LightServer::SpotLight& LightServer::CreateSpotLight(Math::vec4 position, Math::vec4 direction, Math::vec4 color, float length, float angle)
{
	auto light = CalculateSpotLight(position, direction, color, length, angle);
	this->AddSpotLight(light);
	return this->spotLights[this->spotLights.Size() - 1];
}

void LightServer::UpdateWorkGroups()
{
	//Calculate number of work groups based of screen and tile size
	workGroupsX = (GLuint)(Renderer::Instance()->GetRenderResolution().x + (Renderer::Instance()->GetRenderResolution().x % 16)) / 16;
	workGroupsY = (GLuint)(Renderer::Instance()->GetRenderResolution().y + (Renderer::Instance()->GetRenderResolution().y % 16)) / 16;

	auto numTiles = workGroupsX * workGroupsY;

	//Bind the light buffers
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, visiblePointLightIndexBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numTiles * sizeof(VisibleIndex)*tileLights, 0, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, visibleSpotLightIndexBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numTiles * sizeof(VisibleIndex)*tileLights, 0, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

}
