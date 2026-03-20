#pragma once
#include "mouse_utility.h"
#include "utility.h"
#include <functional>
#include <iostream>

struct ImageButtonData{
    sf::Vector2f scale;
    sf::Vector2f pos;
    std::string base_file_path;
    std::string highlighted_file_path;
    std::string selected_file_path;
};

class ImageButton{
    public:
        sf::Texture texture;
        sf::Texture highlighted_texture;
        sf::Texture selected_texture;
        sf::Sprite sprite = sf::Sprite(texture);
        sf::Vector2f original_scale;
        std::function<void()> func = [](){std::cout << "Clicked" << std::endl;}; 
        bool selected = false;

        ImageButton(ImageButtonData data);
        void draw_button(sf::RenderWindow& window);
        void set_func(const std::function<void()>& Afunc);
        void update_button(sf::RenderWindow& window,Mouse& mouse);
};