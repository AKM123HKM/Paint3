#pragma once
#include <SFML/Graphics.hpp>
#include "mouse_utility.h"
#include "canvas.h"
#include "utility.h"
#include "cmath"
#include <vector>

// REWRITE THE TOOL TO BE USED IN CANVAS.H INSTEAD OF PAINT.H

class Tool{
    public:
        virtual void update(sf::RenderWindow& window,Canvas& canvas,Mouse& mouse) = 0;
        virtual void draw() = 0;
};

class PencilTool:public Tool{
    public:
        std::vector<sf::RectangleShape> rect_stroke;
        sf::Vector2f temp_pos = sf::Vector2f(-1,-1);
        sf::Vector2i size;
        PencilTool(int Asize);
        virtual void add_rect(sf::Vector2f mouse_pos,Canvas& canvas);
        virtual void add_stroke();
        virtual void draw(Canvas& canvas);
        virtual void update(sf::RenderWindow& window,Canvas& canvas,Mouse& mouse);
};

class EraserTool:public PencilTool{
    public:
        std::vector<sf::Sprite> sprite_stroke;
        EraserTool(int Asize);
        void add_rect(sf::Vector2f mouse_pos,Canvas& canvas);
        void add_stroke();
        void draw(Canvas& canvas);
};