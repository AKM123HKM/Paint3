#include <SFML/Graphics.hpp>
#include "mouse_utility.h"
#include <vector>

class Canvas{
    public:
        std::vector<sf::VertexArray> strokes = {sf::VertexArray(sf::PrimitiveType::TriangleStrip)};
        sf::Color stroke_color = sf::Color::Red;
        float stroke_thickness = 10;
        sf::Vector2f temp_point = sf::Vector2f(-1,-1);
        void draw_strokes(sf::RenderWindow& window);
        void add_rectangle_vertexes(sf::Vector2f mouse_pos);
        void add_stroke();
        void update(sf::RenderWindow& window,Mouse& mouse);
};