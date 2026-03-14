#include "tools.h"

void PencilTool::add_rectangle_vertex(Canvas& canvas,sf::Vector2f mouse_pos){
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

void PencilTool::change_color(sf::Color color){
    stroke_color = color;
}

void PencilTool::add_stroke(Canvas& canvas){
    sf::VertexArray transformed_stroke = sf::VertexArray(sf::PrimitiveType::TriangleStrip);
    for(int i = 0;i < stroke.getVertexCount();i++){
        sf::Vertex transformed_vertex = sf::Vertex(canvas.canvas_sprite.getInverseTransform().transformPoint(stroke[i].position),stroke[i].color);
        transformed_stroke.append(transformed_vertex);
    }   
    canvas.canvas.draw(transformed_stroke);
    stroke.clear();
    temp_point = sf::Vector2f(-1,-1);
}

void PencilTool::draw(sf::RenderWindow& window){
    window.draw(stroke);
}

void PencilTool::update(Canvas& canvas,sf::RenderWindow& window,Mouse& mouse){
    MouseButtonStates left_button_state = mouse.get_button_state(sf::Mouse::Button::Left);
    MouseButtonEvents left_button_event = mouse.get_button_event(sf::Mouse::Button::Left);
    if(compare_mouse_states(left_button_state,MouseButtonStates::Pressed)){
        sf::Vector2f mouse_pos = mouse.get_mouse_position(window);
        if(check_point_rect_collision(mouse_pos,canvas.canvas_sprite.getGlobalBounds())){
            add_rectangle_vertex(canvas,mouse_pos);
        }
        else{
            add_stroke(canvas);
        }
    }
    if(compare_mouse_events(left_button_event,MouseButtonEvents::Release_Transition)){
        add_stroke(canvas);
    }
    draw(window);
}

void EraserTool::add_stroke(Canvas& canvas){
    stroke.clear();
    temp_point = sf::Vector2f(-1,-1);
}

void EraserTool::update(Canvas& canvas,sf::RenderWindow& window,Mouse& mouse){
    MouseButtonStates left_button_state = mouse.get_button_state(sf::Mouse::Button::Left);
    MouseButtonEvents left_button_event = mouse.get_button_event(sf::Mouse::Button::Left);
    if(compare_mouse_states(left_button_state,MouseButtonStates::Pressed)){
        sf::Vector2f mouse_pos = mouse.get_mouse_position(window);
        if(check_point_rect_collision(mouse_pos,canvas.canvas_sprite.getGlobalBounds())){
            add_rectangle_vertex(canvas,mouse_pos);
        }
        else{
            add_stroke(canvas);
        }
    }
    if(compare_mouse_events(left_button_event,MouseButtonEvents::Release_Transition)){
        add_stroke(canvas);
    }
    sf::VertexArray transformed_stroke = sf::VertexArray(sf::PrimitiveType::TriangleStrip);
    for(int i = 0;i < stroke.getVertexCount();i++){
        sf::Vertex transformed_vertex = sf::Vertex(canvas.canvas_sprite.getInverseTransform().transformPoint(stroke[i].position),stroke[i].color);
        transformed_stroke.append(transformed_vertex);
    }   
    canvas.canvas.draw(transformed_stroke,eraserBlend);
}