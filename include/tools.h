#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "mouse_utility.h"
#include "layer.h"
#include "utility.h"

class Tool{
    public:
        virtual ~Tool() = default;
        virtual void addRectangleVertex(Layer& layer,sf::Vector2f mouse_pos) = 0;
        virtual void addStroke(Layer& layer) = 0;
        virtual void draw(sf::RenderWindow& window) = 0;
        virtual void update(Layer& layer,sf::RenderWindow& window,Mouse& mouse) = 0;

    protected:
        sf::VertexArray stroke = sf::VertexArray(sf::PrimitiveType::TriangleStrip);
        sf::Vector2f temp_point = sf::Vector2f(-1,-1);
};

class PencilTool:public Tool{
    public:
        void addRectangleVertex(Layer& layer,sf::Vector2f mouse_pos);
        void addStroke(Layer& layer);
        void draw(sf::RenderWindow& window);
        void changeColor(sf::Color color);
        void changeThickness(float Athickness);
        void update(Layer& layer,sf::RenderWindow& window,Mouse& mouse);

    protected:
        float stroke_thickness = 1;
        sf::Color stroke_color = sf::Color::Red;
};

class EraserTool:public PencilTool{
    public:
        void addStroke(Layer& layer);
        void update(Layer& layer,sf::RenderWindow& window,Mouse& mouse);
    protected:
        float stroke_thickness = 20;
        sf::BlendMode eraserBlend = sf::BlendMode(
            sf::BlendMode::Factor::Zero,
            sf::BlendMode::Factor::One,
            sf::BlendMode::Equation::Add,
            sf::BlendMode::Factor::Zero,
            sf::BlendMode::Factor::OneMinusSrcAlpha,
            sf::BlendMode::Equation::Add
        );
};