#pragma once
#include "canvas.h"
#include "mouse_utility.h"
#include "button_utility.h"

class Paint{
    public:
    Canvas canvas;
    Mouse mouse;
    sf::Font font;

    Paint(sf::Font& aFont);
    void run(sf::RenderWindow& window);
};