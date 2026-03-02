#pragma once
#include "canvas.h"
#include "mouse_utility.h"

class Paint{
    public:
    Canvas canvas;
    Mouse mouse;

    void run(sf::RenderWindow& window);
};