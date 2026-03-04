#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include "mouse_utility.h"
#include "utility.h"
#include "paint.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Paint 3");
    window.setFramerateLimit(60);
    sf::Vector2f size = sf::Vector2f(50,50);
    sf::Vector2f pos = sf::Vector2f(100,100);
    Paint paint;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        
        window.clear(BG_COLOR);
        paint.run(window);
        window.display();
    }
}
