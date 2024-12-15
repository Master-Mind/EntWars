#include <cassert>
#include <filesystem>
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "./Core/TaskThreads.h"
#include "Core/Input.h"
#include "Core/TheWindow.h"
#include "Editor/Editor.h"
#include "GameLogic/Level.h"
#include "Graphics/Camera.h"
#include <argparse/argparse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameLogic/PathWorker.h"

int main(int argc, char **argv)
{
    argparse::ArgumentParser program("EntWars");
    program.add_argument("--testastar")
        .help("run astar search many times for performance testing")
        .default_value(false)
        .implicit_value(true);
    program.add_argument("--testthreading")
        .help("run astar search many times on a seperate thread for performance testing")
        .default_value(false)
        .implicit_value(true);

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    if (program.get<bool>("--testastar"))
    {
        auto level = LoadLevel("./data/levels/default.json").value();

        auto start = std::chrono::high_resolution_clock::now();

        const int NumSearches = 10000;

        for (int i = 0; i < NumSearches; ++i)
        {
            level->tileMap->AStarSearch({ 140, 540 }, { 610, 19 });
        }

        auto dur = std::chrono::high_resolution_clock::now() - start;

		float secs = std::chrono::duration<float>(dur).count();

        std::cout << secs << " seconds per " << NumSearches << " Astar searches" << std::endl;

        return 0;
    }

    if (program.get<bool>("--testthreading"))
    {
        auto level = LoadLevel("./data/levels/default.json").value();
        auto start = std::chrono::high_resolution_clock::now();
        const int NumSearches = 10000;
        PathReciever pathRecievers[NumSearches];
		std::jthread pathWorker(PathWorker, *level->tileMap);

        for (int i = 0; i < NumSearches; ++i)
        {
            FindPath({ 140, 540 }, { 610, 19 }, &pathRecievers[i]);
        }

        DispatchTask();

		// wait for all paths to be calculated
        for (int i = 0; i < NumSearches; ++i)
        {
            while (!pathRecievers[i].ready)
            {
                CheckPaths();
            }
        }

        auto dur = std::chrono::high_resolution_clock::now() - start;

        float secs = std::chrono::duration<float>(dur).count();

        std::cout << secs << " seconds per " << NumSearches << " Astar searches" << std::endl;

        KillWorkers();

        return 0;
    }

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

void* operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
    (void)name;
    (void)flags;
    (void)debugFlags;
    (void)file;
    (void)line;
    return ::operator new[](size);
}

void* operator new[](size_t size, size_t alignment, size_t offset, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
    (void)name;
    (void)flags;
    (void)debugFlags;
    (void)file;
    (void)line;
    (void)alignment;
    (void)offset;
    return ::operator new[](size);
}