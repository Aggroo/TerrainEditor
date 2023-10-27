#include "config.h"
#include "widgetinspector.h"
#include "application/basegamefeatures/entitymanager.h"
#include "application/game/entitybase.h"
#include "render/server/lightserver.h"
#include "render/camera/camera.h"
#include "IconsFontAwesome5_c.h"

namespace UI
{
Inspector::Inspector(Display::Window* app) : Widget(app)
{
	this->icon = ICON_FA_INFO_CIRCLE;
	this->title = "Inspector";
}

void Inspector::Update()
{
	if (Render::LightServer::Instance()->GetNumPointLights() > 0)
	{
		for(int i = 0; i < Render::LightServer::Instance()->GetNumPointLights(); i++)
		{			
			Render::LightServer::PointLight& light = Render::LightServer::Instance()->GetPointLightAtIndex(i);

			if (ImGui::CollapsingHeader("Point Light##"+i))
			{

				Math::mat4 mat = Math::mat4::Transpose(Math::mat4::translationMatrix(light.position));

				float matrixTranslation[3], matrixRotation[3], matrixScale[3];
				ImGuizmo::DecomposeMatrixToComponents(mat.getPointer(), matrixTranslation, matrixRotation, matrixScale);
				ImGui::InputFloat3("Position##" + i, matrixTranslation);
				ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, mat.getPointer());

				ImGui::Separator();

				if (ImGui::ColorEdit4("Color##" + i, (float*)&light.color))
				{
					//light.color = this->light.col;
					Render::LightServer::Instance()->UpdatePointLightBuffer();
				}
				if (ImGui::DragFloat("Intensity", (float*)&light.radiusAndPadding[1], 0.1f))
				{
					Render::LightServer::Instance()->UpdatePointLightBuffer();
				}
				if (ImGui::DragFloat("Radius##" + i, &light.radiusAndPadding[0], 1.0f))
				{
					//light.radiusAndPadding[0] = this->light.radius;
					Render::LightServer::Instance()->UpdatePointLightBuffer();
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

				if (ImGuizmo::IsUsing())
				{
					light.position = Math::mat4::Transpose(mat).GetPosition();
					Render::LightServer::Instance()->UpdatePointLightBuffer();
				}
				
			}
		}

	}

	auto entities = BaseGameFeature::EntityManager::Instance()->GetEntityList();

	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		it->second->OnUI();
	}
}

}
