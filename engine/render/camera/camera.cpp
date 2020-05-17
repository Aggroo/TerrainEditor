#include "camera.h"
#include "config.h"
#include "camera.h"
#include "render/render/renderer.h"

namespace Graphics
{
__ImplementClass(Graphics::Camera, 'CAMB', Core::RefCounted);
Camera::Camera() :
	aspectRatio(0.0f),
	fov(1.5708f),
	nearZ(0.5f),
	farZ(3000.0f)
{
	this->view = Math::mat4::identity();
	this->UpdateProjectionMatrix();
	this->SetViewMatrix(Math::mat4::identity());

}

Camera::~Camera()
{
}

void Camera::LookAt(const Math::vec4& target, const Math::vec4& up)
{
	this->SetViewMatrix(Math::mat4::LookAt(view.GetPosition(), target, up));
}

void Camera::ResetPosition()
{
	this->SetViewMatrix(Math::mat4::LookAt(Math::vec4(0.0f, 0.0f, 0.0f), Math::vec4(0.0f, 0.0f, 1.0f), Math::vec4(0.0f, 1.0f, 0.0f)));
}

void Camera::SetViewMatrix(const Math::mat4& mat)
{
	this->view = mat;
	this->invView = Math::mat4::inverse(this->view);
	this->viewProjection = this->view * this->projection;
	this->invViewProjection = this->invView * this->invProjection;
}

void Camera::UpdateProjectionMatrix()
{
	Render::Resolution res = Render::Renderer::Instance()->GetRenderResolution();
	this->aspectRatio =  (float)res.x / (float) res.y;

	this->projection = Math::mat4::perspectiveMatrix(-this->fov, this->aspectRatio, this->nearZ, this->farZ);
	this->invProjection = Math::mat4::inverse(this->projection);
	this->viewProjection = this->view * this->projection;
	this->invViewProjection = this->invView * this->invProjection;

	float sx = (float)res.x / 2.0f;
	float sy = (float)res.y / 2.0f;

	Math::mat4  scrScale = Math::mat4(sx, 0.0f, 0.0f, 0.0f,
									  0.0f, sy, 0.0f, 0.0f,
									  0.0f, 0.0f, 1.0f, 0.0f,
									  sx, sy, 0.0f, 1.0f);

	this->viewToTextureSpaceMatrix = this->projection * scrScale;
}

Math::vec4 Camera::GetCameraPosition()
{
	return this->view.GetPosition();
}

Math::vec4 Camera::GetPosition()
{
	return this->invView.GetPosition();
}

void Camera::SetPosition(const Math::vec4& p)
{
	this->view.SetPosition(p);
	this->invView = Math::mat4::inverse(view);
	this->viewProjection = this->view * this->projection;
	this->invViewProjection = this->invView * this->invProjection;
}

}
