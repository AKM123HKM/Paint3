#include "button_utility.h"
#include <iostream>

ImageButtonData::ImageButtonData(){
}

ImageButtonData::ImageButtonData(nlohmann::json data){
    pos = sf::Vector2f(data["pos"][0],data["pos"][1]);
    scale = sf::Vector2f(data["scale"][0],data["scale"][1]);
    base_file_path = data["texture"];
    highlighted_file_path = data["highlighted_texture"];
    selected_file_path = data["selected_texture"];
}

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

void ImageButton::set_func(const std::function<void()>& Afunc){
    func = Afunc;
}

sf::Sprite ImageButton::get_sprite(){
    return sprite;
}

void ImageButton::update(sf::Vector2f mouse_pos,MouseButtonEvents left_button_event){
    sf::FloatRect sprite_bounds = sprite.getGlobalBounds();
    if(check_point_rect_collision(mouse_pos,sprite_bounds)){
        sprite.setScale(original_scale + sf::Vector2f(0.1,0.1));
        sprite.setTexture(highlighted_texture);
        if(compare_mouse_events(left_button_event,MouseButtonEvents::Click)){
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
}

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

ButtonGroup::ButtonGroup(const nlohmann::json data):texture(sf::Vector2u(data["size"][0],data["size"][1])),sprite(texture.getTexture()){
    id = data["id"];
    sf::Vector2f pos = sf::Vector2f(data["pos"][0],data["pos"][1]);
    sprite.setPosition(pos);
    color = sf::Color(data["color"][0],data["color"][1],data["color"][2]);
    for(auto element:data["elements"]){
        if(element["type"] == "toggle_group"){
            toggle_groups.emplace_back(std::pair{element["id"],ToggleButtons()});
            for(auto toggle_element:element["elements"]){
                ImageButtonData button_data(toggle_element);
                std::unique_ptr<UIElement> button = std::make_unique<ImageButton>(button_data);
                button->type = toggle_element["type"];
                toggle_groups.back().second.add_button(static_cast<ImageButton*>(button.get()));
                elements.emplace_back(std::pair{std::pair{toggle_element["id"],toggle_element["type"]},std::move(button)});
            }
        }
        else if (element["type"] == "button"){
            ImageButtonData button_data(element);
            std::unique_ptr<UIElement> button = std::make_unique<ImageButton>(button_data);
            button->type = element["type"];
            elements.emplace_back(std::pair{std::pair{element["id"],element["type"]},std::move(button)});
        }
        else if (element["type"] == "group"){
            std::unique_ptr<UIElement> group = std::make_unique<ButtonGroup>(element);
            elements.emplace_back(std::pair{std::pair{element["id"],element["type"]},std::move(group)});
        }
    }
}

sf::Sprite ButtonGroup::get_sprite(){
    return sprite;
}

void ButtonGroup::update(sf::Vector2f mouse_pos,MouseButtonEvents left_button_event){
    texture.clear(color);
    for(int i = 0;i < elements.size();i++){
        sf::Vector2f transformed_mouse_pos = sprite.getInverseTransform().transformPoint(mouse_pos);
        elements[i].second->update(my_view_transform(transformed_mouse_pos,&texture),left_button_event);
        texture.draw(elements[i].second->get_sprite());
    }
    texture.display();
    sprite.setTexture(texture.getTexture());
}

ImageButton* ButtonGroup::get_button(const std::string a_id){
    for(int i = 0;i < elements.size();i++){
        if(elements[i].first.second == "button"){
            if(elements[i].first.first == a_id){
                return static_cast<ImageButton*>(elements[i].second.get());
            }
        }
        else if(elements[i].first.second == "group"){
            ButtonGroup* group_ptr = static_cast<ButtonGroup*>(elements[i].second.get());
            ImageButton* button_ptr = group_ptr->get_button(a_id);
            if(!(button_ptr == nullptr)){
                return button_ptr;
            }
        }
    }
    return nullptr;
}

DynamicButtonGroup::DynamicButtonGroup(const nlohmann::json data):
                                        ButtonGroup(data){
    element_data = data["elements"];
    add_element();
    
}

void DynamicButtonGroup::add_element(){
    nlohmann::json data = element_data;
    for(int i = 0;i < data.size();i++){
        data[i]["pos"] = nlohmann::json::array({data[i]["pos"][0].get<int>() + (int)last_offset.x,data[i]["pos"][1].get<int>() + (int)last_offset.y});
        last_offset += element_offset;
    }

    for(nlohmann::json element:data){
        if(element["type"] == "group"){
            std::unique_ptr<UIElement> group = std::make_unique<ButtonGroup>(element);
            elements.emplace_back(std::pair{std::pair{element["id"],element["type"]},std::move(group)});
        }
        else if(element["type"] == "button"){
            ImageButtonData button_data(element);
            std::unique_ptr<UIElement> button = std::make_unique<ImageButton>(button_data);
            button->type = element["type"];
            elements.emplace_back(std::pair{std::pair{element["id"],element["type"]},std::move(button)});
        }
    }
}