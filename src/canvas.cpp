#include "canvas.h"
#include "cmath"
#include "utility.h"
#include <iostream>

Canvas::Canvas(sf::Vector2f size,sf::Vector2f pos,std::string background_file_path):canvas(sf::Vector2u(size)),canvas_sprite(canvas.getTexture()){
    canvas_sprite.setPosition(pos);
    canvas.setSmooth(true);
    if(!(background_file_path.size() == 0)){
        if(!(background.loadFromFile(background_file_path))){
            std::cout << "Background not loaded!" << std::endl;
        }
        sf::Sprite background_sprite(background);
        is_background = true;
        canvas.draw(background_sprite);
    }
}

void Canvas::draw(sf::RenderWindow& window){
    canvas.display();
    canvas_sprite.setTexture(canvas.getTexture());
    window.draw(canvas_sprite);
}