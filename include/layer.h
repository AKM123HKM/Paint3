#pragma once
#include <SFML/Graphics.hpp>
#include "mouse_utility.h"
#include <vector>

class Layer{
    public:
        sf::Texture background;
        bool is_background = false;
        sf::RenderTexture texture;
        sf::Sprite texture_sprite;
        Layer(sf::Vector2f size,sf::Vector2f pos,std::string background_file_path = "");
        void draw(sf::RenderWindow& window);
};
