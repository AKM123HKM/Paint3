#pragma once
#include "canvas.h"
#include "mouse_utility.h"
#include "button_utility.h"
#include "sidebar.h"

class Paint{
    public:
    Canvas canvas;
    Mouse mouse;
    sf::Font font;
    SideBar sidebar;

    Paint(sf::Font& aFont);
    void run(sf::RenderWindow& window);
};