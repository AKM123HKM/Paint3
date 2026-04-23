#include "tools.h"

void PencilTool::addRectangleVertex(Layer& layer,sf::Vector2f mouse_pos){
    if(!(temp_point.x == -1) && !(temp_point == mouse_pos)){
        sf::Vector2f perpendicular_dir = getPerpendicular(mouse_pos,temp_point);
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

void PencilTool::changeColor(sf::Color color){
    stroke_color = color;
}

void PencilTool::addStroke(Layer& layer){
    sf::VertexArray transformed_stroke = sf::VertexArray(sf::PrimitiveType::TriangleStrip);
    for(int i = 0;i < stroke.getVertexCount();i++){
        sf::Vertex transformed_vertex = sf::Vertex(layer.texture_sprite.getInverseTransform().transformPoint(stroke[i].position),stroke[i].color);
        transformed_stroke.append(transformed_vertex);
    }   
    layer.texture.draw(transformed_stroke);
    stroke.clear();
    temp_point = sf::Vector2f(-1,-1);
}

void PencilTool::draw(sf::RenderWindow& window){
    window.draw(stroke);
}

void PencilTool::changeThickness(float Athickness){
    if(Athickness < 50 && Athickness > 1){
        stroke_thickness = Athickness;
    }
    else{
        std::cout << "THICKNESS IS OUT OF BOUNDS!" << std::endl;
    }
}

void PencilTool::update(Layer& layer,sf::RenderWindow& window,Mouse& mouse){
    MouseButtonStates left_button_state = mouse.getButtonState(sf::Mouse::Button::Left);
    MouseButtonEvents left_button_event = mouse.getButtonEvent(sf::Mouse::Button::Left);
    if(compareMouseStates(left_button_state,MouseButtonStates::Pressed)){
        sf::Vector2f mouse_pos = mouse.getMousePosition(window);
        if(checkPointRectCollision(mouse_pos,layer.texture_sprite.getGlobalBounds())){
            addRectangleVertex(layer,mouse_pos);
        }
        else{
            addStroke(layer);
        }
    }
    if(compareMouseEvents(left_button_event,MouseButtonEvents::Release_Transition)){
        addStroke(layer);
    }
    draw(window);
}

void EraserTool::addStroke(Layer& layer){
    stroke.clear();
    temp_point = sf::Vector2f(-1,-1);
}

void EraserTool::update(Layer& layer,sf::RenderWindow& window,Mouse& mouse){
    MouseButtonStates left_button_state = mouse.getButtonState(sf::Mouse::Button::Left);
    MouseButtonEvents left_button_event = mouse.getButtonEvent(sf::Mouse::Button::Left);
    if(compareMouseStates(left_button_state,MouseButtonStates::Pressed)){
        sf::Vector2f mouse_pos = mouse.getMousePosition(window);
        if(checkPointRectCollision(mouse_pos,layer.texture_sprite.getGlobalBounds())){
            addRectangleVertex(layer,mouse_pos);
        }
        else{
            addStroke(layer);
        }
    }
    if(compareMouseEvents(left_button_event,MouseButtonEvents::Release_Transition)){
        addStroke(layer);
    }
    sf::VertexArray transformed_stroke = sf::VertexArray(sf::PrimitiveType::TriangleStrip);
    for(int i = 0;i < stroke.getVertexCount();i++){
        sf::Vertex transformed_vertex = sf::Vertex(layer.texture_sprite.getInverseTransform().transformPoint(stroke[i].position),stroke[i].color);
        transformed_stroke.append(transformed_vertex);
    }   
    layer.texture.draw(transformed_stroke,eraserBlend);
}