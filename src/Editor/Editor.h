#pragma once

namespace sf
{
	class RenderWindow;
	class Window;
	class Event;
}

bool EditorInit(sf::RenderWindow &_mainwindow);
void EditorProcessEvents(sf::Window& _mainwindow, sf::Event& _event);
void EditorUpdate(sf::RenderWindow& _mainwindow);
void EditorRender(sf::RenderWindow& _mainwindow);
void EditorDeinit();