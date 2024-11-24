#include "Input.h"

#include <bitset>
#include <SFML/Window/Event.hpp>

std::bitset<static_cast<size_t>(Input::Count)> inputs;

void HandeInput(sf::Event& _event)
{
	if (_event.type == sf::Event::KeyPressed)
	{
		switch (_event.key.code)
		{
		case sf::Keyboard::A:
		case sf::Keyboard::Left:
			inputs[static_cast<size_t>(Input::Left)] = true;
			break;
		case sf::Keyboard::D:
		case sf::Keyboard::Right:
			inputs[static_cast<size_t>(Input::Right)] = true;
			break;
		case sf::Keyboard::W:
		case sf::Keyboard::Up:
			inputs[static_cast<size_t>(Input::Up)] = true;
			break;
		case sf::Keyboard::S:
			if (_event.key.control)
			{
				inputs[static_cast<size_t>(Input::SaveLevel)] = true;
				break;
			}
			_FALLTHROUGH;
		case sf::Keyboard::Down:
			inputs[static_cast<size_t>(Input::Down)] = true;
			break;
		case sf::Keyboard::N:
			if (_event.key.control)
			{
				inputs[static_cast<size_t>(Input::NewLevel)] = true;
			}
			break;
		case sf::Keyboard::O:
			if (_event.key.control)
			{
				inputs[static_cast<size_t>(Input::OpenLevel)] = true;
			}
			break;
			default:
				break;
		}
	}
	else if (_event.type == sf::Event::KeyReleased)
	{
		switch (_event.key.code)
		{
		case sf::Keyboard::A:
		case sf::Keyboard::Left:
			inputs[static_cast<size_t>(Input::Left)] = false;
			break;
		case sf::Keyboard::D:
		case sf::Keyboard::Right:
			inputs[static_cast<size_t>(Input::Right)] = false;
			break;
		case sf::Keyboard::W:
		case sf::Keyboard::Up:
			inputs[static_cast<size_t>(Input::Up)] = false;
			break;
		case sf::Keyboard::S:
		case sf::Keyboard::Down:
			inputs[static_cast<size_t>(Input::Down)] = false;
			inputs[static_cast<size_t>(Input::SaveLevel)] = false;
			break;
		case sf::Keyboard::N:
			inputs[static_cast<size_t>(Input::NewLevel)] = false;
			break;
		case sf::Keyboard::O:
			inputs[static_cast<size_t>(Input::OpenLevel)] = false;
			break;
		default:
			break;
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