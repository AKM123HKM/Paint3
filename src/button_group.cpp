#include "button_group.h"

void ToggleButtons::add_button(ImageButton* button){
    button->selected = false;
    buttons.push_back(button);
}

void ToggleButtons::toggle_button(sf::RenderWindow& window,Mouse& mouse){
    for (auto button:buttons){
        if(compare_mouse_events(mouse.get_button_event(sf::Mouse::Button::Left),MouseButtonEvents::Click)){
            if(check_point_rect_collision(mouse.get_mouse_position(window),button->sprite.getGlobalBounds())){
                if(current_button == nullptr){
                    current_button = button;
                }
                else if(current_button == button){
                    current_button = nullptr;
                }
                else{
                    current_button->selected = false;
                    current_button = button;
                }
            }
        }
    }
}

ButtonGroup::ButtonGroup(const nlohmann::json data){
    id = data["id"];
    for(auto element:data["elements"]){
        if(element["type"] == "toggle_group"){
            toggle_groups.emplace_back(std::pair{element["id"],ToggleButtons()});
            for(auto toggle_element:element["elements"]){
                ImageButtonData button_data;
                button_data.pos = sf::Vector2f(toggle_element["pos"][0],toggle_element["pos"][1]);
                button_data.scale = sf::Vector2f(toggle_element["scale"][0],toggle_element["scale"][1]);
                button_data.base_file_path = toggle_element["texture"];
                button_data.highlighted_file_path = toggle_element["highlighted_texture"];
                button_data.selected_file_path = toggle_element["selected_texture"];
                buttons.emplace_back(std::pair{toggle_element["id"],std::make_unique<ImageButton>(button_data)});
                toggle_groups.back().second.add_button(buttons.back().second.get());
            }
        }
        else{
            ImageButtonData button_data;
            button_data.pos = sf::Vector2f(element["pos"][0],element["pos"][1]);
            button_data.scale = sf::Vector2f(element["scale"][0],element["scale"][1]);
            button_data.base_file_path = element["texture"];
            button_data.highlighted_file_path = element["highlighted_texture"];
            button_data.selected_file_path = element["selected_texture"];
            buttons.emplace_back(std::pair{element["id"],std::make_unique<ImageButton>(button_data)});
        }
    }
}

void ButtonGroup::update(sf::RenderWindow& window,Mouse& mouse){
    for(int i = 0;i < buttons.size();i++){
        buttons[i].second->update_button(window,mouse);
    }
    for(int i = 0;i < toggle_groups.size();i++){
        toggle_groups[i].second.toggle_button(window,mouse);
    }
}

ImageButton* ButtonGroup::get_button(const std::string id){
    for(int i = 0;i < buttons.size();i++){
        if(buttons[i].first == id){
            return buttons[i].second.get();
        }
    }
    throw std::runtime_error("The given id doesn't exist!");
}