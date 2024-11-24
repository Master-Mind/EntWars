#include "Editor.h"
#include "../GameLogic/Level.h"
#include "../Core/fileutils.hpp"
#include "../Core/Input.h"

#include <imgui-SFML.h>
#include "imgui.h"
#include "SFML/System/Clock.hpp"
#include <portable-file-dialogs.h>

#include "LevelEditor.h"

static std::unique_ptr<Level> editedLevel;

bool EditorInit(sf::RenderWindow& _mainwindow)
{
	if (std::filesystem::exists("./data/levels/default.json"))
	{
		editedLevel = LoadLevel("./data/levels/default.json").value();
	}

	return ImGui::SFML::Init(_mainwindow);
}

void EditorProcessEvents(sf::Window& _mainwindow, sf::Event& _event)
{
    ImGui::SFML::ProcessEvent(_mainwindow, _event);
}

void newLevel()
{
	auto dest = pfd::save_file("Make A New Level File", ".", { "JSON", "*.json" }).result();

	if (!dest.empty())
	{
		editedLevel = MakeLevel();
		editedLevel->name = std::filesystem::path(dest).filename().string();
		editedLevel->path = dest;
		MakeNewFile(dest);
		editedLevel->tileMapPath = "./data/tilemaps/" + (editedLevel->name + ".tmap");
		MakeNewFile(editedLevel->tileMapPath);
	}
}

void openLevel()
{
	auto src = pfd::open_file("Open A Level File", ".", { "JSON", "*.json" }).result();

	if (!src.empty())
	{
		editedLevel = LoadLevel(*src.begin()).value();
	}
}

void saveLevel()
{
	bool succ = SaveLevel(editedLevel->path, editedLevel).value();
	assert(succ);
}

void EditorUpdate(sf::RenderWindow& _mainwindow)
{
	static sf::Clock deltaClock;

	editedLevel->tileMap->Render(_mainwindow);

    ImGui::SFML::Update(_mainwindow, deltaClock.restart());
    ImGui::ShowDemoWindow();
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
		if (ImGui::Button("New Level (Ctrl+N)"))
		{
			newLevel();
		}

		if (ImGui::Button("Open Level (Ctrl+0)"))
		{
			openLevel();
		}

		if (ImGui::Button("Save Level (Ctrl+S)"))
		{
			saveLevel();
		}

        ImGui::EndMenu();
    }

	if (InputDown(Input::NewLevel))
	{
		newLevel();
	}

	if (InputDown(Input::OpenLevel))
	{
		openLevel();
	}

	if (InputDown(Input::SaveLevel))
	{
		saveLevel();
	}

    ImGui::EndMainMenuBar();

	if (editedLevel)
	{
		EditLevel(editedLevel, _mainwindow);
	}
}

void EditorRender(sf::RenderWindow& _mainwindow)
{
    ImGui::SFML::Render(_mainwindow);
}

void EditorDeinit()
{
    ImGui::SFML::Shutdown();
}
