#include "Input.h"

#include <bitset>
#include <SFML/Window/Event.hpp>

std::bitset<static_cast<size_t>(Input::Count)> inputs;

void HandeInput(sf::Event& _event)
{
	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::A || _event.key.code == sf::Keyboard::Left)
		{
			inputs[static_cast<size_t>(Input::Left)] = true;
		}
		else if (_event.key.code == sf::Keyboard::D || _event.key.code == sf::Keyboard::Right)
		{
			inputs[static_cast<size_t>(Input::Right)] = true;
		}
		else if (_event.key.code == sf::Keyboard::W || _event.key.code == sf::Keyboard::Up)
		{
			inputs[static_cast<size_t>(Input::Up)] = true;
		}
		else if (_event.key.code == sf::Keyboard::S || _event.key.code == sf::Keyboard::Down)
		{
			inputs[static_cast<size_t>(Input::Down)] = true;
		}
	}
	else if (_event.type == sf::Event::KeyReleased)
	{
		if (_event.key.code == sf::Keyboard::A || _event.key.code == sf::Keyboard::Left)
		{
			inputs[static_cast<size_t>(Input::Left)] = false;
		}
		else if (_event.key.code == sf::Keyboard::D || _event.key.code == sf::Keyboard::Right)
		{
			inputs[static_cast<size_t>(Input::Right)] = false;
		}
		else if (_event.key.code == sf::Keyboard::W || _event.key.code == sf::Keyboard::Up)
		{
			inputs[static_cast<size_t>(Input::Up)] = false;
		}
		else if (_event.key.code == sf::Keyboard::S || _event.key.code == sf::Keyboard::Down)
		{
			inputs[static_cast<size_t>(Input::Down)] = false;
		}
	}

	if (_event.type == sf::Event::MouseWheelScrolled)
	{
		if (_event.mouseWheelScroll.delta > 0)
		{
			inputs[static_cast<size_t>(Input::ZoomIn)] = true;
		}
		else if (_event.mouseWheelScroll.delta < 0)
		{
			inputs[static_cast<size_t>(Input::ZoomOut)] = true;
		}
	}
}

bool InputDown(Input _input)
{
	return inputs[static_cast<size_t>(_input)];
}

void InputEndFrame()
{
	inputs[static_cast<size_t>(Input::ZoomIn)] = false;
	inputs[static_cast<size_t>(Input::ZoomOut)] = false;
}