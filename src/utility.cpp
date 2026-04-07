#include <iostream>
#include "utility.h"

std::ostream& operator<<(std::ostream& os, MouseButtonStates button_states){
    switch(button_states){
        case(MouseButtonStates::Idle) : return os << "Idle";
        case(MouseButtonStates::Held) : return os << "Held";
        case(MouseButtonStates::Dragging) : return os << "Dragging";
        case(MouseButtonStates::Pressed) : return os << "Pressed";
    }
    return os << "Unknown";
}

std::ostream& operator<<(std::ostream& os, MouseButtonEvents button_events){
    switch(button_events){
        case(MouseButtonEvents::Click): return os << "Click";
        case(MouseButtonEvents::Double_Click): return os << "Double_Click";
        case(MouseButtonEvents::None): return os << "None";
        case(MouseButtonEvents::Press_Transition): return os << "Press_Transition";
        case(MouseButtonEvents::Release_Transition): return os << "Release_Transition";
    }
    return os << "Unkown";
}

std::ostream& operator<<(std::ostream& os,sf::Vector2f vector){
    return os << "x:" << vector.x << " y:" << vector.y;
}

bool compare_mouse_states(MouseButtonStates a, MouseButtonStates b){
    switch(b){
        case(MouseButtonStates::Idle): if(a == MouseButtonStates::Idle){return true;}
        case(MouseButtonStates::Pressed): if(a == MouseButtonStates::Pressed){return true;}
        case(MouseButtonStates::Held): if(a == MouseButtonStates::Pressed || a == MouseButtonStates::Held){return true;}
        case(MouseButtonStates::Dragging): if(a == MouseButtonStates::Pressed || a == MouseButtonStates::Held || a == MouseButtonStates::Dragging){return true;}
    }
    return false;
}

bool compare_mouse_events(MouseButtonEvents a, MouseButtonEvents b){
    switch(b){
        case(MouseButtonEvents::None): if(a == MouseButtonEvents::None){return true;}
        case(MouseButtonEvents::Press_Transition): if(a == MouseButtonEvents::Press_Transition){return true;}
        case(MouseButtonEvents::Release_Transition): if(a == MouseButtonEvents::Release_Transition){return true;}
        case(MouseButtonEvents::Click): if(a == MouseButtonEvents::Release_Transition || a == MouseButtonEvents::Click){return true;}
        case(MouseButtonEvents::Double_Click): if(a == MouseButtonEvents::Release_Transition || a == MouseButtonEvents::Click || a == MouseButtonEvents::Double_Click){return true;}
    }
    return false;
}

bool check_point_rect_collision(sf::Vector2f pos,sf::FloatRect rect){
    if (pos.x >= rect.position.x && pos.x <= rect.position.x + rect.size.x){
        if(pos.y >= rect.position.y && pos.y <= rect.position.y + rect.size.y){
            return true;
        }
    }
    return false;
}

sf::Vector2f get_perpendicular(sf::Vector2f x,sf::Vector2f y){
    sf::Vector2f dir = x - y;
    float magnitude = sqrt(dir.x*dir.x + dir.y*dir.y);
    sf::Vector2f normalized_dir = dir / magnitude;
    return sf::Vector2f(normalized_dir.y * -1.f, normalized_dir.x);
}

nlohmann::json loadJSON(const std::string file_path){
    std::ifstream file(file_path);
    if(!file){
        throw std::runtime_error("Failed to load the file!");
    }
    nlohmann::json data;
    file >> data;
    return data;
}

sf::Vector2f my_view_transform(sf::Vector2f mouse_pos,sf::RenderTarget* target){
    sf::View original_view = target->getDefaultView();
    sf::View new_view = target->getView();
    sf::Vector2f new_view_center = new_view.getCenter();
    sf::Vector2f new_view_size = new_view.getSize();
    sf::Vector2f original_view_centre = original_view.getCenter();
    sf::Vector2f original_view_size = original_view.getSize();
    sf::Vector2f new_view_pos = new_view_center - (new_view_size/2.f);
    // new_x = start_x + (original_length/new_length) * old_x;
    float new_x  = new_view_pos.x + (new_view_size.x/original_view_size.x) * mouse_pos.x;
    float new_y = new_view_pos.y + (new_view_size.y/original_view_size.y) * mouse_pos.y;
    return sf::Vector2f(new_x,new_y);
}