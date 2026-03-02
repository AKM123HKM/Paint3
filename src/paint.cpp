#include "paint.h"

Paint::Paint(sf::Font& aFont){
    font = aFont;
}

void Paint::run(sf::RenderWindow& window){
    sf::Vector2f mouse_pos = mouse.get_mouse_position(window);
    mouse.updateButton(sf::Mouse::Button::Left);
    canvas.update(window,mouse);
}