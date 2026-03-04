#pragma once
#include "canvas.h"
#include "mouse_utility.h"
#include "button_utility.h"
#include "sidebar.h"

const sf::Color BG_COLOR = sf::Color::White;

class Paint{
    public:
    Canvas canvas;
    Mouse mouse;
    sf::Font font;
    SideBar sidebar;
    sf::Color current_color = sf::Color::Red;
    sf::Color eraser_color = BG_COLOR;

    Paint();
    void handle_sidebar_buttons();
    void run(sf::RenderWindow& window);
};