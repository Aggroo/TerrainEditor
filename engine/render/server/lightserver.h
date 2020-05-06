#pragma once
#include <gl/glew.h>
#include "core/singleton.h"
#include "foundation/math/vec4.h"
#include "foundation/util/array.h"

namespace Render
{
struct VisibleIndex
{
	int index;
};

class LightServer
{
__DeclareSingleton(LightServer)
public: //Structures
	struct PointLight
	{
		Math::vec4 position;
		Math::vec4 color;
		Math::vec4 radiusAndPadding;
	};

	struct DirectionalLight
	{
		Math::vec4 position;
		Math::vec4 color;
	};

	struct SpotLight
	{
		//Center of the cones bounding sphere and radius of the sphere
		Math::vec4 centerAndRadius;
		//Color of the light and center offset
		Math::vec4 colorAndCenterOffset;
		//XY: Direction, Z: cosine of cone angle and lightdir Z sign, W: Falloff radius
		Math::vec4 parameters;
	};

public: //Functions
	void AddPointLight(const PointLight& pLight);
	void RemovePointLight(PointLight* light);
	size_t GetNumPointLights() const { return this->pointLights.Size(); }
	PointLight& GetPointLightAtIndex(const int& indx);

	void AddSpotLight(const SpotLight& sLight);
	void RemoveSpotLight(SpotLight* light);
	size_t GetNumSpotLights() const { return this->spotLights.Size(); }
	SpotLight CalculateSpotLight(Math::vec4 position, Math::vec4 direction, Math::vec4 color, float length, float angle);
	SpotLight& GetSpotLightAtIndex(const int& indx);

	GLuint GetWorkGroupsX() const { return this->workGroupsX; }
	GLuint GetWorkGroupsY() const { return this->workGroupsY; }

	GLuint GetPointLightBuffer() const { return this->pointLightBuffer; }
	GLuint GetSpotLightBuffer() const { return this->spotLightBuffer; }
	GLuint GetVisiblePointLightIndexBuffer() const { return this->visiblePointLightIndexBuffer; }
	GLuint GetVisibleSpotLightIndexBuffer() const { return this->visibleSpotLightIndexBuffer; }

	GLuint GetTileLights() const { return this->tileLights; }

	void UpdatePointLightBuffer() const;
	void UpdateSpotLightBuffer() const;

	// Debug and Easier Access //
	LightServer::PointLight& CreatePointLight(Math::vec4 position, Math::vec4 color, const float& intensity, float radius);
	LightServer::SpotLight& CreateSpotLight(Math::vec4 position, Math::vec4 direction, Math::vec4 color, float length, float angle);

private:

	friend class Renderer;

	//Update the compute shader work groups
	void UpdateWorkGroups();

	//Storage buffers for the light data and light indices
	GLuint pointLightBuffer;
	GLuint spotLightBuffer;
	GLuint visiblePointLightIndexBuffer;
	GLuint visibleSpotLightIndexBuffer;

	//Work group dimensions for the light compute shader
	GLuint workGroupsX;
	GLuint workGroupsY;

	//Collections of all the lights
	Util::Array<PointLight> pointLights;
	Util::Array<SpotLight> spotLights;

	//Number of lights per tile
	GLuint tileLights = 512;
};
}
