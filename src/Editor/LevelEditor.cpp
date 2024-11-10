#include "LevelEditor.h"
#include "../GameLogic/Level.h"

#include "imgui.h"

bool EditLevel(std::unique_ptr<Level>& level)
{
	bool changed = false;

	if (ImGui::Begin("Level Editor"))
	{
		ImGui::Text("Level Name: %s", level->name.c_str());

		
	}

	ImGui::End();

	return changed;
}
