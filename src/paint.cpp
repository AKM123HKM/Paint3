#include "paint.h"

Paint::Paint(nlohmann::json data,sf::RenderTarget& target):ui(data,target){
    if(!font.openFromFile("../assets/PoetsenOne-Regular.ttf")){
        std::cout << "Font not loaded!" << std::endl;
    }
    layers.emplace_back(std::make_unique<Layer>(CANVAS_SIZE,CANVAS_POS));
    current_layer = layers[0].get();
    tools.eraser.changeThickness(20);
}

void Paint::run(sf::RenderWindow& window){
    sf::Vector2f mouse_pos = mouse.getMousePosition(window);
    mouse.updateButton(sf::Mouse::Button::Left);
    mouse.updateMousePosition(window);
    if(!(current_tool == nullptr)){
        current_tool->update(*current_layer,window,mouse);
    }
    current_layer->draw(window);
    ui.update(window,mouse,event_manager);
    CustomEvent event = event_manager.pollEvent();
    while(event != event_manager.null_event){
        if(event.type == Event::ButtonClicked){
            bindings[event.id]();
        }
        event = event_manager.pollEvent();
    }
}