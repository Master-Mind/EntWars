#include "Camera.h"
#include "../Core/Input.h"
#include <SFML/Window.hpp>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/View.hpp"

void CameraUpdate(sf::RenderWindow& window)
{
	static sf::View view(sf::FloatRect(0, 0, 400, 400));
	static float zoom = 1.0f;
	float cameraSpeed = 1.1f;

	if (InputDown(Input::Faster))
	{
		cameraSpeed *= 5;
	}

	view.setSize(window.getSize().x * zoom, window.getSize().y * zoom);

	if (InputDown(Input::Left))
	{
		view.move(-cameraSpeed * zoom, 0);
	}
	if (InputDown(Input::Right))
	{
		view.move(cameraSpeed * zoom, 0);
	}
	if (InputDown(Input::Up))
	{
		view.move(0, -cameraSpeed * zoom);
	}
	if (InputDown(Input::Down))
	{
		view.move(0, cameraSpeed * zoom);
	}

	if (InputDown(Input::ZoomIn))
	{
		zoom *=0.9f;
	}
	else if (InputDown(Input::ZoomOut))
	{
		zoom *= 1.1f;
	}

	window.setView(view);
}
