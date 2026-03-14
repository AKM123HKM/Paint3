#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "mouse_utility.h"
#include "canvas.h"
#include "utility.h"

class Tool{
    public:
        virtual ~Tool() = default;
        virtual void add_rectangle_vertex(Canvas& canvas,sf::Vector2f mouse_pos) = 0;
        virtual void add_stroke(Canvas& canvas) = 0;
        virtual void draw(sf::RenderWindow& window) = 0;
        virtual void update(Canvas& canvas,sf::RenderWindow& window,Mouse& mouse) = 0;

    protected:
        sf::VertexArray stroke = sf::VertexArray(sf::PrimitiveType::TriangleStrip);
        sf::Vector2f temp_point = sf::Vector2f(-1,-1);
};

class PencilTool:public Tool{
    public:
        void add_rectangle_vertex(Canvas& canvas,sf::Vector2f mouse_pos);
        void add_stroke(Canvas& canvas);
        void draw(sf::RenderWindow& window);
        void change_color(sf::Color color);
        void update(Canvas& canvas,sf::RenderWindow& window,Mouse& mouse);

    protected:
        float stroke_thickness = 10;
        sf::Color stroke_color = sf::Color::Red;
};

class EraserTool:public PencilTool{
    public:
        void add_stroke(Canvas& canvas);
        void update(Canvas& canvas,sf::RenderWindow& window,Mouse& mouse);
    protected:
        float stroke_thickness = 10;
        sf::Color stroke_color = sf::Color::Red;
        sf::BlendMode eraserBlend = sf::BlendMode(
            sf::BlendMode::Factor::Zero,
            sf::BlendMode::Factor::One,
            sf::BlendMode::Equation::Add,
            sf::BlendMode::Factor::Zero,
            sf::BlendMode::Factor::OneMinusSrcAlpha,
            sf::BlendMode::Equation::Add
        );
};