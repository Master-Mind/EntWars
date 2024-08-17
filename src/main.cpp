#include <cassert>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ent Wars");
    window.setVerticalSyncEnabled(true);

    assert(ImGui::SFML::Init(window));

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        sf::Clock deltaClock;

        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::ShowDemoWindow();
        ImGui::BeginMainMenuBar();

        if(ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
        window.clear();

        window.draw(sf::CircleShape(3));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}