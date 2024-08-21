#include <cassert>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include "Editor/Editor.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ent Wars");
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
            if (event.type == sf::Event::Closed)
                window.close();
        }

        EditorUpdate(window);

        window.clear();

        window.draw(sf::CircleShape(3));
        EditorRender(window);
        window.display();
    }

    EditorDeinit();

    return 0;
}