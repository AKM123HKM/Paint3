#include "layer.h"
#include "cmath"
#include "utility.h"
#include <iostream>

Layer::Layer(sf::Vector2f size,sf::Vector2f pos,std::string background_file_path):texture(sf::Vector2u(size)),texture_sprite(texture.getTexture()){
    texture_sprite.setPosition(pos);
    texture.setSmooth(true);
    if(!(background_file_path.size() == 0)){
        if(!(background.loadFromFile(background_file_path))){
            std::cout << "Background not loaded!" << std::endl;
        }
        sf::Sprite background_sprite(background);
        is_background = true;
        texture.draw(background_sprite);
    }
}

void Layer::draw(sf::RenderWindow& window){
    texture.display();
    texture_sprite.setTexture(texture.getTexture());
    window.draw(texture_sprite);
}
