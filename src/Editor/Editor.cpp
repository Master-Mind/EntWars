#include "Editor.h"
#include <imgui-SFML.h>

#include "imgui.h"
#include "SFML/System/Clock.hpp"

bool EditorInit(sf::RenderWindow& _mainwindow)
{
	return ImGui::SFML::Init(_mainwindow);
}

void EditorProcessEvents(sf::Window& _mainwindow, sf::Event& _event)
{
    ImGui::SFML::ProcessEvent(_mainwindow, _event);
}

void EditorUpdate(sf::RenderWindow& _mainwindow)
{
	static sf::Clock deltaClock;

    ImGui::SFML::Update(_mainwindow, deltaClock.restart());
    ImGui::ShowDemoWindow();
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void EditorRender(sf::RenderWindow& _mainwindow)
{
    ImGui::SFML::Render(_mainwindow);
}

void EditorDeinit()
{
    ImGui::SFML::Shutdown();
}
