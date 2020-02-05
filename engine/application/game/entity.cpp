#include "config.h"
#include "entity.h"
#include "render/server/shaderserver.h"
#include "render/server/frameserver.h"
#include "render/camera/camera.h"
#include "imgui.h"
#include "ImSequencer.h"
#include "ImCurveEdit.h"
#include "imgui_internal.h"
#include "render/render/renderer.h"
#include "foundation/input/inputmanager.h"


namespace Game
{
__ImplementClass(Game::Entity, 'ENTY', Game::EntityBase);
Entity::Entity() : entityName("")
{
	this->mesh = Render::Model::Create();
	this->shader = Render::ShaderObject::Create();
	this->textures = Render::TextureNode::Create();
}

Entity::~Entity()
{
}

void Entity::Activate()
{
	this->shader->BindProgram();
	this->shader->setupUniformInt("AlbedoMap", (GLuint)Render::TextureIndex::albedo0);
	this->shader->setupUniformInt("NormalMap", (GLuint)Render::TextureIndex::normal0);
	this->shader->setupUniformInt("SpecularMap", (GLuint)Render::TextureIndex::specular0);
	this->shader->setupUniformInt("RoughnessMap", (GLuint)Render::TextureIndex::roughness0);
	this->shader->setupUniformInt("aoMap", (GLuint)Render::TextureIndex::ao0);

	EntityBase::Activate();
}

void Entity::Deactivate()
{
	EntityBase::Deactivate();
}

void Entity::Update()
{
	this->shader->BindProgram();
	this->textures->BindTextures();
	this->shader->setupMatrix4fv("Model", this->transform);

	//if (this->mesh->IsRenderable())
	this->mesh->Draw();
}

void Entity::OnUI()
{
	const Util::String s = GetName() + " Entity";
	
	if (ImGui::CollapsingHeader(s.AsCharPtr()))
	{
		if (Input::InputManager::Instance()->GetButtonKeyboard(Input::KeyCode::Q))
			gizmo.mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (Input::InputManager::Instance()->GetButtonKeyboard(Input::KeyCode::E))
			gizmo.mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (Input::InputManager::Instance()->GetButtonKeyboard(Input::KeyCode::R))
			gizmo.mCurrentGizmoOperation = ImGuizmo::SCALE;

		const Util::String translateName = "Translate##" + GetName();
		const Util::String rotateName = "Rotate##" + GetName();
		const Util::String scaleName = "Scale##" + GetName();
		if (ImGui::RadioButton(translateName.AsCharPtr(), gizmo.mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			gizmo.mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton(rotateName.AsCharPtr(), gizmo.mCurrentGizmoOperation == ImGuizmo::ROTATE))
			gizmo.mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton(scaleName.AsCharPtr(), gizmo.mCurrentGizmoOperation == ImGuizmo::SCALE))
			gizmo.mCurrentGizmoOperation = ImGuizmo::SCALE;

		const Util::String tr = "Tr##" + GetName();
		const Util::String rt = "Rt##" + GetName();
		const Util::String sc = "Sc##" + GetName();
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		Math::mat4 mat = Math::mat4::Transpose(this->transform);
		ImGuizmo::DecomposeMatrixToComponents(mat.getPointer(), matrixTranslation, matrixRotation, matrixScale);
		ImGui::InputFloat3(tr.AsCharPtr(), matrixTranslation, 3);
		ImGui::InputFloat3(rt.AsCharPtr(), matrixRotation, 3);
		ImGui::InputFloat3(sc.AsCharPtr(), matrixScale, 3);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, mat.getPointer());

		if (gizmo.mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			const Util::String localName = "Local##" + GetName();
			const Util::String worldName = "World##" + GetName();
			if (ImGui::RadioButton(localName.AsCharPtr(), gizmo.mCurrentGizmoMode == ImGuizmo::LOCAL))
				gizmo.mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton(worldName.AsCharPtr(), gizmo.mCurrentGizmoMode == ImGuizmo::WORLD))
				gizmo.mCurrentGizmoMode = ImGuizmo::WORLD;
		}
		if (ImGui::IsKeyPressed(83))
			gizmo.useSnap = !gizmo.useSnap;

		const Util::String snapCheckName = "##Snap" + GetName();
		ImGui::Checkbox(snapCheckName.AsCharPtr(), &gizmo.useSnap);
		ImGui::SameLine();

		const Util::String snapName = "Snap##" + GetName();
		const Util::String snapAngleName = "Angle Snap##" + GetName();
		const Util::String snapScaleName = "Scale Snap##" + GetName();
		switch (gizmo.mCurrentGizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			ImGui::InputFloat3(snapName.AsCharPtr(), &gizmo.snap[0]);
			break;
		case ImGuizmo::ROTATE:
			ImGui::InputFloat(snapAngleName.AsCharPtr(), &gizmo.snap[0]);
			break;
		case ImGuizmo::SCALE:
			ImGui::InputFloat(snapScaleName.AsCharPtr(), &gizmo.snap[0]);
			break;
		}

		Math::mat4 view = Graphics::MainCamera::Instance()->GetView();

		///Invert the entire Y-Axis of the view matrix to work with ImGuizmo
		view[4] = -view[4];
		view[5] = -view[5];
		view[6] = -view[6];
		view[7] = -view[7];

		ImGuizmo::Manipulate(Math::mat4::Transpose(view).getPointer(),
			Math::mat4::Transpose(Graphics::MainCamera::Instance()->GetProjection()).getPointer(), 
			gizmo.mCurrentGizmoOperation,
			gizmo.mCurrentGizmoMode, 
			mat.getPointer(), 
			NULL, 
			gizmo.useSnap ? &gizmo.snap[0] : NULL);

		this->transform = Math::mat4::Transpose(mat);

	}
}

void Entity::SetMesh(Util::String filename)
{
	this->mesh->LoadModel(filename.AsCharPtr());
}

Ptr<Render::Model> Entity::GetMesh()
{
	return this->mesh;
}

void Entity::SetTextures(Util::String albedo, Util::String normal, Util::String metallic, Util::String roughness, Util::String ao)
{
	//TODO: FIX THIS WHEN SHADER SETTINGS ARE HANDLED IN FILES
	this->textures->AddTexture(Render::TextureIndex::albedo0, albedo.AsCharPtr());
	this->textures->AddTexture(Render::TextureIndex::normal0, normal.AsCharPtr());
	this->textures->AddTexture(Render::TextureIndex::specular0, metallic.AsCharPtr());
	this->textures->AddTexture(Render::TextureIndex::roughness0, roughness.AsCharPtr());

	this->textures->AddTexture(Render::TextureIndex::ao0, Render::FrameServer::Instance()->GetSSAOPass()->GetSSAOBuffer());

}

void Entity::SetShaders(Util::String vertexShader, Util::String fragmentShader, const char* name)
{
	GLuint vert = Render::ShaderServer::Instance()->LoadVertexShader(vertexShader);
	GLuint frag = Render::ShaderServer::Instance()->LoadFragmentShader(fragmentShader);

	this->shader->AddShader(vert);
	this->shader->AddShader(frag);
	this->shader->LinkShaders();

	Render::ShaderServer::Instance()->AddShaderObject(name, this->shader);
}

Ptr<Render::ShaderObject> Entity::GetShader()
{
	return this->shader;
}

void Entity::SetName(Util::String name)
{
	this->entityName = name;
}

Util::String Entity::GetName()
{
	return entityName;
}

void Entity::SetIBLMaps(Ptr<Render::TextureResource> envmapID, Ptr<Render::TextureResource> irID, Ptr<Render::TextureResource> brdfID)
{
	this->environmentMap = envmapID;
	textures->AddTexture(Render::TextureIndex::environmentMap, this->environmentMap);
	this->shader->setupUniformInt("environmentMap", (GLuint)Render::TextureIndex::environmentMap);

	this->irradiance = irID;
	textures->AddTexture(Render::TextureIndex::irradiance, this->irradiance);
	this->shader->setupUniformInt("irradiance", (GLuint)Render::TextureIndex::irradiance);

	this->brdf = brdfID;
	textures->AddTexture(Render::TextureIndex::brdf, this->brdf);
	this->shader->setupUniformInt("brdfLUT", (GLuint)Render::TextureIndex::brdf);

	
}

Ptr<Render::TextureResource> Entity::GetEnvironmentMap()
{
	return this->environmentMap;
}
}
