#include "canvas.h"
#include "cmath"
#include "utility.h"
#include <iostream>

Canvas::Canvas(sf::Vector2f size,sf::Vector2f pos,std::string background_file_path):canvas(sf::Vector2u(size)),canvas_sprite(canvas.getTexture()){
    canvas_sprite.setPosition(pos);
    canvas.setSmooth(true);
    if(!(background_file_path.size() == 0)){
        if(!(background.loadFromFile(background_file_path))){
            std::cout << "Background not loaded!" << std::endl;
        }
        sf::Sprite background_sprite(background);
        is_background = true;
        canvas.draw(background_sprite);
    }
}

void Canvas::update(sf::RenderWindow& window,Mouse& mouse){
    MouseButtonStates left_button_state = mouse.get_button_state(sf::Mouse::Button::Left);
    MouseButtonEvents left_button_event = mouse.get_button_event(sf::Mouse::Button::Left);
    if(compare_mouse_states(left_button_state,MouseButtonStates::Pressed)){
        sf::Vector2f mouse_pos = mouse.get_mouse_position(window);
        if(check_point_rect_collision(mouse_pos,canvas_sprite.getGlobalBounds())){
            add_rectangle_vertexes(mouse_pos);
        }
        else{
            add_stroke();
        }
    }
    if(compare_mouse_events(left_button_event,MouseButtonEvents::Release_Transition)){
        add_stroke();
    }
    draw_strokes(window);
}

void Canvas::add_rectangle_vertexes(sf::Vector2f mouse_pos){
    if(!(temp_point.x == -1) && !(temp_point == mouse_pos)){
        sf::Vector2f perpendicular_dir = get_perpendicular(mouse_pos,temp_point);
        sf::Vector2f pos1 = sf::Vector2f((perpendicular_dir * (stroke_thickness/2)) + temp_point);
        sf::Vector2f pos2 = sf::Vector2f(((perpendicular_dir * -1.f) * (stroke_thickness/2)) + temp_point);
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
    sf::VertexArray transformed_stroke = sf::VertexArray(sf::PrimitiveType::TriangleStrip);
    for(int i = 0;i < stroke.getVertexCount();i++){
        sf::Vertex transformed_vertex = sf::Vertex(canvas_sprite.getInverseTransform().transformPoint(stroke[i].position),stroke[i].color);
        transformed_stroke.append(transformed_vertex);
    }   
    canvas.draw(transformed_stroke);
    stroke.clear();
    temp_point = sf::Vector2f(-1,-1);
}

void Canvas::draw_strokes(sf::RenderWindow& window){
    canvas.display();
    canvas_sprite.setTexture(canvas.getTexture());
    window.draw(canvas_sprite);
    window.draw(stroke);
}