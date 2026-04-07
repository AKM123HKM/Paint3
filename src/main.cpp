#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include "mouse_utility.h"
#include "utility.h"
#include "layer.h"
#include <vector>
#include "paint.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Paint 3",sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    nlohmann::json data = loadJSON("../assets/ui/UI.json");
    Paint paint(data,window);

    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        
        window.clear();
        window.clear(BG_COLOR);
        paint.run(window);
        window.display();
    }
}
