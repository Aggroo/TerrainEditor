#include "config.h"
#include "widgetinspector.h"
#include "application/basegamefeatures/entitymanager.h"
#include "application/game/entitybase.h"
#include "render/server/lightserver.h"
#include "IconsFontAwesome5_c.h"

namespace UI
{
Inspector::Inspector(Display::Window* app) : Widget(app)
{
	Util::String title = ICON_FA_INFO_CIRCLE;
	title.Append(" Inspector");
	this->title = title;
}

void Inspector::Update()
{
	if (Render::LightServer::Instance()->GetNumPointLights() > 0)
	{
		Render::LightServer::PointLight& light = Render::LightServer::Instance()->GetPointLightAtIndex(0);
		this->light.pos = light.position;
		this->light.col = light.color;
		this->light.radius = light.radiusAndPadding.x();

		if (ImGui::CollapsingHeader("Light"))
		{
			if (ImGui::DragFloat4("Position", (float*)&this->light.pos))
			{
				light.position = this->light.pos;
				Render::LightServer::Instance()->UpdatePointLightBuffer();
			}
			if (ImGui::DragFloat4("Color", (float*)&this->light.col, 0.01f))
			{
				light.color = this->light.col;
				Render::LightServer::Instance()->UpdatePointLightBuffer();
			}
			if (ImGui::DragFloat("Radius", &this->light.radius, 0.1f))
			{
				light.radiusAndPadding[0] = this->light.radius;
				Render::LightServer::Instance()->UpdatePointLightBuffer();
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
