#include "button_utility.h"
#include <iostream>

Button::Button(sf::Vector2f size,sf::Vector2f pos,int Apadding){
    rect.setSize(size);
    rect.setPosition(pos);
    rect.setOutlineThickness(thickness);
    rect.setFillColor(sf::Color::Green);
    padding = Apadding;
}

void Button::update_button(sf::RenderWindow& window,Mouse& mouse){
    sf::Vector2f mouse_pos = mouse.get_mouse_position(window);
    if(check_point_rect_collision(mouse_pos,sf::FloatRect(rect.getPosition(),rect.getSize()))){
        rect.setFillColor(highlighted_base_color);
        rect.setOutlineColor(highlighted_outline_color);
    }
    else{
        rect.setFillColor(base_color);
        rect.setOutlineColor(highlighted_outline_color);
    }
    draw_button(window);
}

bool Button::check_collision(sf::Vector2f mouse_pos){
    return check_point_rect_collision(mouse_pos,sf::FloatRect(rect.getSize(),rect.getPosition()));
}

void Button::draw_button(sf::RenderWindow& window){
    window.draw(rect);
}

void Button::set_button_color(sf::Color Abase_color,sf::Color Aoutline_color){
    base_color = Abase_color;
    base_outline_color = Aoutline_color;
}

void Button::set_highlighted_button_color(sf::Color Abase_color,sf::Color Aoutline_color){
    base_color = Abase_color;
    base_outline_color = Aoutline_color;
}

Text_Button::Text_Button(sf::Vector2f size,sf::Vector2f pos,sf::Font& font,int Apadding): Button(size,pos,Apadding),text(font,"Button"){
    text.setPosition(pos);
    text.setString("Button");
}

void Text_Button::draw_button(sf::RenderWindow& window){
    window.draw(rect);
    window.draw(text);
}

Image_Button::Image_Button(sf::Vector2f scale,sf::Vector2f pos,std::string image_path,std::string highlighted_image_path):
                           texture(image_path),
                           sprite(texture),
                           highlighted_texture(highlighted_image_path){
    texture.setSmooth(true);
    highlighted_texture.setSmooth(true);
    sprite.setPosition(pos);
    original_scale = scale;
    sprite.setScale(original_scale);
}

void Image_Button::draw_button(sf::RenderWindow& window){
    window.draw(sprite);
}

void Image_Button::update_button(sf::RenderWindow& window,Mouse& mouse){
    sf::Vector2f mouse_pos = mouse.get_mouse_position(window);
    if(check_point_rect_collision(mouse_pos,sprite.getGlobalBounds())){
        sprite.setScale(original_scale + sf::Vector2f(0.1,0.1));
        sprite.setTexture(highlighted_texture);
    }
    else{
        sprite.setScale(original_scale);
        sprite.setTexture(texture);
    }
    draw_button(window);
}