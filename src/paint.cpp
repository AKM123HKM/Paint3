#include "paint.h"

Paint::Paint(){
    if(!font.openFromFile("../assets/PoetsenOne-Regular.ttf")){
        std::cout << "Font not loaded!" << std::endl;
    }
}

void Paint::handle_sidebar_buttons(){
    if(sidebar.eraser_button.is_clicked()){
        canvas.stroke_color = eraser_color;
    }
    else{
        canvas.stroke_color = current_color;
    }
}

void Paint::run(sf::RenderWindow& window){
    sf::Vector2f mouse_pos = mouse.get_mouse_position(window);
    mouse.updateButton(sf::Mouse::Button::Left);
    canvas.update(window,mouse);
    sidebar.update(window,mouse);
    handle_sidebar_buttons();
}