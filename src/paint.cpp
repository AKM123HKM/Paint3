#include "paint.h"

Paint::Paint(){
    if(!font.openFromFile("../assets/PoetsenOne-Regular.ttf")){
        std::cout << "Font not loaded!" << std::endl;
    }
    layers.emplace_back(std::make_unique<Layer>(CANVAS_SIZE,CANVAS_POS));
    current_layer = layers[0].get();
}

void Paint::handle_sidebar_buttons(){
    if(sidebar.eraser_button.is_clicked()){
        if(current_tool != &(tools.eraser)){
            current_tool = &(tools.eraser);
        }
    }
    else{
        if(current_tool != &(tools.pencil)){
            current_tool = &(tools.pencil);
        }
    }
}

void Paint::run(sf::RenderWindow& window){
    sf::Vector2f mouse_pos = mouse.get_mouse_position(window);
    mouse.updateButton(sf::Mouse::Button::Left);
    current_layer->draw(window);
    if(!(current_tool == nullptr)){
        current_tool->update(*current_layer,window,mouse);
    }
    sidebar.update(window,mouse);
    handle_sidebar_buttons();
}