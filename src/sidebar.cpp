#include "sidebar.h"

void ToggleButtons::add_button(Image_Button* button){
    buttons.push_back(button);
}

void ToggleButtons::toggle_button(Image_Button* button){
    // if(current_button){
        current_button->clicked = false;
        if(current_button == button){
            current_button = nullptr;
        }
        else{
            current_button = button;
            current_button->clicked = true;
        }
    // }
    // else{
    //     current_button = button;
    //     current_button-> clicked = true;
    // }
}

SideBar::SideBar():eraser_button(eraser_data),
                   save_button(save_data){
    group1.add_button(&eraser_button);
    group1.add_button(&eraser_button);
}

void SideBar::update(sf::RenderWindow& window,Mouse& mouse){
    eraser_button.update_button(window,mouse);
    save_button.update_button(window,mouse);
    sf::Vector2f mouse_pos = mouse.get_mouse_position(window);
    if(compare_mouse_events(mouse.get_button_event(sf::Mouse::Button::Left),MouseButtonEvents::Click)){
        if(check_point_rect_collision(mouse_pos,eraser_button.sprite.getGlobalBounds())){
            group1.toggle_button(&eraser_button);
            std::cout << "Eraser_Button" << std::endl;
        }
        else if(check_point_rect_collision(mouse_pos,save_button.sprite.getGlobalBounds())){
            group1.toggle_button(&save_button);
            std::cout << "Save_Button" << std::endl;
        }
    }
}