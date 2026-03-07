#include "tools.h"

PencilTool::PencilTool(int Asize){
    size = sf::Vector2i(Asize,Asize);
}

void PencilTool::update(sf::RenderWindow& window,Canvas& canvas,Mouse& mouse){
    sf::Vector2f mouse_pos = mouse.get_mouse_position(window);
    if(check_point_rect_collision(mouse_pos,sf::FloatRect(canvas.canvas_sprite.getGlobalBounds()))){
        if(compare_mouse_states(mouse.get_button_state(sf::Mouse::Button::Left),MouseButtonStates::Pressed)){
            add_rect(mouse_pos,canvas);
        }
        else{
            add_stroke();
        }
    }
    else{
        add_stroke();
    }
    draw(canvas);
}

void PencilTool::add_stroke(){
    rect_stroke.clear();
    temp_pos = sf::Vector2f(-1,-1);
}

void PencilTool::add_rect(sf::Vector2f mouse_pos,Canvas& canvas){
    if(!(temp_pos.x < 0) && !(temp_pos == mouse_pos)){
        sf::Vector2f dir = mouse_pos - temp_pos;
        float magnitude = sqrt(dir.x*dir.x + dir.y*dir.y);
        sf::Vector2f perpendicular_dir = get_perpendicular(mouse_pos,temp_pos);
        int loops = magnitude/size.x;
        for(int i = 0;i <= loops;i++){
            float mulitplier = i;
            sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(size));
            rect.setPosition(dir*mulitplier);
            rect.setFillColor(canvas.stroke_color);
            rect_stroke.push_back(rect);
        }
        temp_pos = mouse_pos;
    }
    else{
        temp_pos = mouse_pos;
    }
}

void PencilTool::draw(Canvas& canvas){
    for(auto rect:rect_stroke){
        canvas.canvas.draw(rect);
    }
}

EraserTool::EraserTool(int Asize):PencilTool(Asize){
}

void EraserTool::add_stroke(){
    rect_stroke.clear();
    sprite_stroke.clear();
    temp_pos = sf::Vector2f(-1,-1);
}

void EraserTool::add_rect(sf::Vector2f mouse_pos,Canvas& canvas){
    if(!(temp_pos.x < 0) && !(temp_pos == mouse_pos)){
        sf::Vector2f dir = mouse_pos - temp_pos;
        float magnitude = sqrt(dir.x*dir.x + dir.y*dir.y);
        sf::Vector2f perpendicular_dir = get_perpendicular(mouse_pos,temp_pos);
        int loops = magnitude/size.x;
        for(int i = 0;i <= loops;i++){
            float mulitplier = i;
            if(canvas.is_background){
                sf::Sprite restore(canvas.background);
                sf::IntRect rect(sf::Vector2i(canvas.canvas_sprite.getInverseTransform().transformPoint(dir * mulitplier)),size);
                restore.setTextureRect(rect);
                restore.setPosition(dir*mulitplier);
                sprite_stroke.push_back(restore);
                std::cout << "Hi" << std::endl;
            }
            else{
                sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(size));
                rect.setPosition(dir*mulitplier);
                rect.setFillColor(sf::Color::White);
                rect_stroke.push_back(rect);
            }
        }
        temp_pos = mouse_pos;
    }
    else{
        temp_pos = mouse_pos;
    }
}

void EraserTool::draw(Canvas& canvas){
    if(canvas.is_background){
        for(auto rect:sprite_stroke){
            canvas.canvas.draw(rect);
        }
    }
    else{
        for(auto rect:rect_stroke){
            canvas.canvas.draw(rect);
        }
    }
}