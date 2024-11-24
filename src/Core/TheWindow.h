#pragma once
#include <SFML/Graphics.hpp>

inline sf::RenderWindow& GetWindow()
{
	//I know singletons are bad, so sue me.
	// TODO: Might have to wrap in a mutex
	static sf::RenderWindow window(sf::VideoMode(1600, 1200), "Ent Wars");

	return window;
}