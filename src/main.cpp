#include <cassert>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <iostream>

#include "Editor/Editor.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Input.h"
#include "Graphics/Camera.h"
#include "Core/TheWindow.h"

int main()
{
	std::cout << "Running at: " << std::filesystem::current_path() << std::endl;
	sf::RenderWindow& window = GetWindow();
    window.setVerticalSyncEnabled(true);

    bool editorInited = EditorInit(window);

    assert(editorInited);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;

        while (window.pollEvent(event))
        {
            EditorProcessEvents(window, event);
            // "close requested" event: we close the window
            switch (event.type)
            {
            case sf::Event::Closed:
				window.close();
                break;
            case sf::Event::Resized: break;
            case sf::Event::LostFocus: break;
            case sf::Event::GainedFocus: break;
            case sf::Event::TextEntered: break;
            case sf::Event::KeyPressed:
            case sf::Event::KeyReleased:
            case sf::Event::MouseWheelMoved:
            case sf::Event::MouseWheelScrolled:
            case sf::Event::MouseButtonPressed:
            case sf::Event::MouseButtonReleased:
            case sf::Event::JoystickButtonPressed: 
            case sf::Event::JoystickButtonReleased: 
            case sf::Event::JoystickMoved: 
            case sf::Event::TouchBegan: 
            case sf::Event::TouchMoved:
            case sf::Event::TouchEnded:
            case sf::Event::MouseMoved:
				HandeInput(event);
            	break;
            case sf::Event::MouseEntered: break;
            case sf::Event::MouseLeft: break;
            case sf::Event::JoystickConnected: break;
            case sf::Event::JoystickDisconnected: break;
            case sf::Event::SensorChanged: break;
            case sf::Event::Count: break;
            default: ;
            }
        }

		CameraUpdate(window);
        window.clear(sf::Color(234, 182, 118));

        EditorUpdate(window);
        EditorRender(window);
        window.display();
		InputEndFrame();
    }

    EditorDeinit();

    return 0;
}
