#include "paint.h"

Paint::Paint(nlohmann::json data):sidebar(data){
    if(!font.openFromFile("../assets/PoetsenOne-Regular.ttf")){
        std::cout << "Font not loaded!" << std::endl;
    }
    layers.emplace_back(std::make_unique<Layer>(CANVAS_SIZE,CANVAS_POS));
    current_layer = layers[0].get();
    tools.eraser.change_thickness(20);

    for(std::pair binding:bindings){
        sidebar.get_button(binding.first)->set_func(binding.second);
    }
}

void Paint::run(sf::RenderWindow& window){
    sf::Vector2f mouse_pos = mouse.get_mouse_position(window);
    mouse.updateButton(sf::Mouse::Button::Left);
    sidebar.update(window,mouse);
    current_layer->draw(window);
    if(!(current_tool == nullptr)){
        current_tool->update(*current_layer,window,mouse);
    }
}