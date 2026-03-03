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
    sf::Font FONT;
    if(!FONT.openFromFile("../assets/PoetsenOne-Regular.ttf")){
        std::cout << "FONT not loaded" << std::endl;
    }
    Paint paint(FONT);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        
        window.clear(sf::Color::White);
        paint.run(window);
        window.display();
    }
}
