#include "button_utility.h"
#include <iostream>

ImageButton::ImageButton(ImageButtonData data):
                           texture(data.base_file_path),
                           sprite(texture),
                           highlighted_texture(data.highlighted_file_path),
                           selected_texture(data.selected_file_path){
    texture.setSmooth(true);
    highlighted_texture.setSmooth(true);
    selected_texture.setSmooth(true);
    sprite.setPosition(data.pos);
    original_scale = data.scale;
    sprite.setScale(original_scale);
}

void ImageButton::draw_button(sf::RenderWindow& window){
    window.draw(sprite);
}

void ImageButton::set_func(const std::function<void()>& Afunc){
    func = Afunc;
}

void ImageButton::update_button(sf::RenderWindow& window,Mouse& mouse){
    sf::Vector2f mouse_pos = mouse.get_mouse_position(window);
    if(check_point_rect_collision(mouse_pos,sprite.getGlobalBounds())){
        sprite.setScale(original_scale + sf::Vector2f(0.1,0.1));
        sprite.setTexture(highlighted_texture);
        if(compare_mouse_events(mouse.get_button_event(sf::Mouse::Button::Left),MouseButtonEvents::Click)){
            selected = !selected;
            func();
        }
    }
    else{
        sprite.setScale(original_scale);
        sprite.setTexture(texture);
    }
    if(selected){
        sprite.setTexture(selected_texture);
    }
    draw_button(window);
}