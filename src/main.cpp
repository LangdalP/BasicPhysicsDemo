#include <iostream>     // For debugging
#include <algorithm>    // For min
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <Box2D/Box2D.h>

#include "constants.h"
#include "logging.h"
#include "world.h"
#include "gfx.h"

const static int VELOCITY_ITERATIONS = 8;
const static int POSITION_ITERATIONS = 3;

const static sf::Color col_bg(30, 30, 30);

// Semi-fixed timestep ( http://gafferongames.com/game-physics/fix-your-timestep/ )
void main_loop(sf::RenderWindow &ren, b2World *world)
{
    const float dt = 1000.0/60;
    sf::Clock frameClock;
    sf::Clock loopClock;
    uint32_t frame_counter = 0;

    while (ren.isOpen()) {
        // Event handling
        sf::Event event;
        while (ren.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                ren.close();
        }

        // World update
        sf::Time frameTime = frameClock.restart(); // Restart from 0 and return elapsed

        while ( frameTime.asSeconds() > 0 ) {
            float deltaTime = std::min( frameTime.asSeconds(), dt );
            // Update world here
            world->Step(deltaTime, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
            frameTime = frameTime - sf::seconds(deltaTime);
        }
        
        // Rendering
        ren.clear(col_bg);
        draw_world_polygons(ren, world);
        ren.display();
        ++frame_counter;
    }

    float loopDuration = loopClock.getElapsedTime().asSeconds();
    float avgFPS = (float)frame_counter/loopDuration;

    printf("Avg. fps: %f\n", avgFPS);

}

int main(int argc, char *argv[])
{
    sf::VideoMode videoMode(DEF_WIDTH, DEF_HEIGHT);
    sf::RenderWindow window(videoMode,"Drop the Bass");
    window.setVerticalSyncEnabled(true);

    std::cout << "Window: " << window.getSize().x << ", " << window.getSize().y << std::endl;

    // World and bodies are never cleaned up... should they?
    b2World *world = create_world_with_test_objects();
    main_loop(window, world);

    return 0;
}

