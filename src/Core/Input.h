#pragma once

namespace sf
{
	class Event;
}

enum class Input
{
	Up,
	Down,
	Left,
	Right,
	ZoomIn,
	ZoomOut,
	Count

};

void HandeInput(sf::Event& _event);
bool InputDown(Input _input);
void InputEndFrame();