#pragma once
#include <SFML/Graphics.hpp>
#include "mouse_utility.h"
#include <vector>

class Canvas{
    public:
        sf::Texture background;
        bool is_background = false;
        sf::RenderTexture canvas;
        sf::Sprite canvas_sprite;
        Canvas(sf::Vector2f size,sf::Vector2f pos,std::string background_file_path = "");
        void draw(sf::RenderWindow& window);
};