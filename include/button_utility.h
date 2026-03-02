#pragma once
#include "mouse_utility.h"
#include "utility.h"

class Button{
    public:
        sf::RectangleShape rect;
        sf::Color base_color = sf::Color(200,200,200);
        sf::Color base_outline_color = sf::Color(50,50,50);
        sf::Color highlighted_base_color = sf::Color(80,80,80);
        sf::Color highlighted_outline_color = base_outline_color;
        float thickness = 5;
        int padding;

        Button(sf::Vector2f size,sf::Vector2f pos,int Apadding = 0);
        void set_button_color(sf::Color Abase_color,sf::Color Aoutline_color);
        void set_highlighted_button_color(sf::Color Abase_color,sf::Color Aoutline_color);
        bool check_collision(sf::Vector2f mouse_pos);
        void update_button(sf::RenderWindow& window,Mouse& mouse);
        virtual void draw_button(sf::RenderWindow& window);
};

class Text_Button:public Button{
    public:
        sf::Text text;

        Text_Button(sf::Vector2f size,sf::Vector2f pos,sf::Font& font,int Apadding = 0);
        void draw_button(sf::RenderWindow& window);
};

class Image_Button:public Button{
    public:
        sf::Texture texture;

        Image_Button(sf::Vector2f size,sf::Vector2f pos,std::string file_path,int Apadding = 0);
        void draw_button(sf::RenderWindow& window);
};