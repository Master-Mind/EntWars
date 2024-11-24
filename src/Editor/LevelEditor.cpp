#include "LevelEditor.h"
#include "../GameLogic/Level.h"

#include "imgui.h"
#include <SFML/Graphics/Shape.hpp>

#include "../Core/Input.h"
#include "SFML/Window/Mouse.hpp"

bool EditLevel(std::unique_ptr<Level>& level, sf::RenderWindow &window)
{
	bool changed = false;

	if (ImGui::Begin("Level Editor"))
	{
		ImGui::Text("Level Name: %s", level->name.c_str());
		ImGui::Text("Tile Map");

		int newWidth = level->tileMap->width;
		int newHeight = level->tileMap->height;

		ImGui::InputInt("Width", &newWidth);
		ImGui::InputInt("Height", &newHeight);

		static int curTile = 0;
		ImGui::Combo("Tile", &curTile, "Empty\0Wall\0");

		if (InputDown(Input::Num0))
		{
			curTile = 0;
		}
		else if (InputDown(Input::Num1))
		{
			curTile = 1;
		}

		if (newWidth < 0)
		{
			newWidth = 0;
		}

		if (newHeight < 0)
		{
			newHeight = 0;
		}

		if (level->tileMap->width != newWidth || level->tileMap->height != newHeight)
		{
			level->tileMap->Resize(newWidth, newHeight);
			changed = true;
		}

		if (level->tileMap->width > 0 && level->tileMap->height > 0)
		{
			//draw grid for tilemap with sfml lines
			std::vector<sf::Vertex> verts(static_cast<size_t>(level->tileMap->width + level->tileMap->height + 2) * 2);

			for (int x = 0; x <= level->tileMap->width; x++)
			{
				verts[x * 2] = sf::Vertex(sf::Vector2f(x * level->tileMap->tileSize, 0), sf::Color::White);
				verts[x * 2 + 1] = sf::Vertex(sf::Vector2f(x * level->tileMap->tileSize, level->tileMap->height * level->tileMap->tileSize), sf::Color::White);
			}

			for (int y = 0; y <= level->tileMap->height; y++)
			{
				verts[(level->tileMap->width + y + 1) * 2] = sf::Vertex(sf::Vector2f(0, y * level->tileMap->tileSize), sf::Color::White);
				verts[(level->tileMap->width + y + 1) * 2 + 1] = sf::Vertex(sf::Vector2f(level->tileMap->width * level->tileMap->tileSize, y * level->tileMap->tileSize), sf::Color::White);
			}

			window.draw(verts.data(), verts.size(), sf::Lines);

			//draw square for mouse position
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

			int gridx = static_cast<int>(std::floor(worldPos.x / level->tileMap->tileSize));
			int gridy = static_cast<int>(std::floor(worldPos.y / level->tileMap->tileSize)); //hittin the gridy for ukraine

			sf::Vertex square [5] = {
				sf::Vertex(sf::Vector2f(gridx * level->tileMap->tileSize, gridy * level->tileMap->tileSize), sf::Color::Red),
				sf::Vertex(sf::Vector2f((gridx + 1) * level->tileMap->tileSize, gridy * level->tileMap->tileSize), sf::Color::Red),
				sf::Vertex(sf::Vector2f((gridx + 1) * level->tileMap->tileSize, (gridy + 1) * level->tileMap->tileSize), sf::Color::Red),
				sf::Vertex(sf::Vector2f(gridx * level->tileMap->tileSize, (gridy + 1) * level->tileMap->tileSize), sf::Color::Red),
				sf::Vertex(sf::Vector2f(gridx * level->tileMap->tileSize, gridy * level->tileMap->tileSize), sf::Color::Red)
			};

			window.draw(square, 5, sf::LineStrip);

			if (InputDown(Input::Select) && level->tileMap->IsInBounds(gridx, gridy) && !ImGui::IsWindowHovered())
			{
				level->tileMap->SetTile(gridx, gridy, static_cast<TileType>(curTile));
				changed = true;
			}
		}


		
	}

	ImGui::End();

	return changed;
}
