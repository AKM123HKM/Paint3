#include "canvas.h"
#include "cmath"
#include "utility.h"
#include <iostream>

Canvas::Canvas(sf::Vector2f size,sf::Vector2f pos){
    canvas.resize(sf::Vector2u(size));
    canvas_pos = pos;
    canvas.setSmooth(true);
}

void Canvas::update(sf::RenderWindow& window,Mouse& mouse){
    MouseButtonStates left_button_state = mouse.get_button_state(sf::Mouse::Button::Left);
    MouseButtonEvents left_button_event = mouse.get_button_event(sf::Mouse::Button::Left);
    if(compare_mouse_states(left_button_state,MouseButtonStates::Pressed)){
        add_rectangle_vertexes(mouse.get_mouse_position(window));
    }
    if(compare_mouse_events(left_button_event,MouseButtonEvents::Release_Transition)){
        add_stroke();
    }
    draw_strokes(window);
}

void Canvas::add_rectangle_vertexes(sf::Vector2f mouse_pos){
    if(!(temp_point.x == -1) && !(temp_point == mouse_pos)){
        sf::Vector2f prev_pos = temp_point;
        sf::Vector2f dir = mouse_pos - prev_pos;
        float magnitude = sqrt(dir.x*dir.x + dir.y*dir.y);
        sf::Vector2f normalized_dir = dir / magnitude;
        sf::Vector2f perpendicular_dir = sf::Vector2f(normalized_dir.y * -1.f, normalized_dir.x);
        sf::Vector2f pos1 = sf::Vector2f((perpendicular_dir * (stroke_thickness/2)) + prev_pos);
        sf::Vector2f pos2 = sf::Vector2f(((perpendicular_dir * -1.f) * (stroke_thickness/2)) + prev_pos);
        sf::Vector2f pos3 = sf::Vector2f((perpendicular_dir * (stroke_thickness/2)) + mouse_pos);
        sf::Vector2f pos4 = sf::Vector2f(((perpendicular_dir * -1.f) * (stroke_thickness/2)) + mouse_pos);

        stroke.append(sf::Vertex(pos1,stroke_color));
        stroke.append(sf::Vertex(pos2,stroke_color));
        stroke.append(sf::Vertex(pos3,stroke_color));
        
        stroke.append(sf::Vertex(pos2,stroke_color));
        stroke.append(sf::Vertex(pos3,stroke_color));
        stroke.append(sf::Vertex(pos4,stroke_color));

        temp_point = mouse_pos;
    }
    else{
        temp_point = mouse_pos;
    }
}

void Canvas::add_stroke(){
    canvas.draw(stroke);
    stroke.clear();
    temp_point = sf::Vector2f(-1,-1);
}

void Canvas::draw_strokes(sf::RenderWindow& window){
    canvas.display();
    sf::Sprite sprite(canvas.getTexture());
    sprite.setPosition(canvas_pos);
    window.draw(sprite);
    window.draw(stroke);
}