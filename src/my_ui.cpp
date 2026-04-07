#include "my_ui.h"

UI::UI(nlohmann::json data,sf::RenderTarget& target):sidebar(data["groups"][0]),layers(data["groups"][1]){
   
}

void UI::update(sf::RenderWindow& window,Mouse& mouse){
    sf::Vector2f mouse_pos = mouse.get_mouse_position(window);
    sidebar.update(mouse_pos,mouse.get_button_event(sf::Mouse::Button::Left));
    window.draw(sidebar.get_sprite());
    if(toggle_layers){
        // std::cout << layers.get_size() << std::endl;
        layers.update(mouse_pos,mouse.get_button_event(sf::Mouse::Button::Left));
        window.draw(layers.get_sprite());
    }
}