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
	NewLevel,
	SaveLevel,
	OpenLevel,
	Select,
	Num0,
	Num1,
	Num2,
	Num3,
	Num4,
	Num5,
	Num6,
	Num7,
	Num8,
	Num9,
	Faster,
	Count
};

void HandeInput(sf::Event& _event);
bool InputDown(Input _input);
void InputEndFrame();