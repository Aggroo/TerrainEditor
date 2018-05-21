#pragma once
#include "foundation/math/mat4.h"

namespace Graphics
{
class Camera
{
public:
	Camera();
	~Camera();

	void LookAt(const Math::vec4& target, const Math::vec4& up);

	void SetViewMatrix(const Math::mat4& mat);
	void UpdateProjectionMatrix();

	Math::vec3 GetCameraPosition();

	Math::vec4 GetPosition();
	void SetPosition(const Math::vec4 & p);

	Math::mat4 GetView() const { return view; }
	Math::mat4 GetInvView() const { return invView; }

	Math::mat4 GetProjection() const { return projection; }
	Math::mat4 GetInvProjection() const { return invProjection; }

	Math::mat4 GetViewProjection() const { return viewProjection; }
	Math::mat4 GetInvViewProjection() const { return invViewProjection; }

	Math::mat4 GetViewToTextureSpaceMatrix() const { return viewToTextureSpaceMatrix; }

private:
	float fov;
	float nearZ;
	float farZ;
	float aspectRatio;

	Math::mat4 view;
	Math::mat4 invView;
	Math::mat4 projection;
	Math::mat4 invProjection;
	Math::mat4 viewProjection;
	Math::mat4 invViewProjection;

	//For converting viewspace coordinates to screen pixel coordinates.
	Math::mat4 viewToTextureSpaceMatrix;

};

class MainCamera : public Camera
{
private:
	MainCamera() {}
public:
	static MainCamera* Instance()
	{
		static MainCamera instance;
		return &instance;
	}

	// C++ 11
	// Delete the methods we don't want.
	MainCamera(MainCamera const&) = delete;
	void operator=(MainCamera const&) = delete;
	void operator=(Camera const&) = delete;
};
}
