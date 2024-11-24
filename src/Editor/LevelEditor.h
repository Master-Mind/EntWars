#pragma once
#include <memory>

#include "SFML/Graphics/RenderWindow.hpp"
struct Level;
bool EditLevel(std::unique_ptr<Level> &level, sf::RenderWindow& window);
