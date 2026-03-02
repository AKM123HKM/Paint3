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