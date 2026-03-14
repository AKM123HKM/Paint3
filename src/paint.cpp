#include "paint.h"

Paint::Paint():canvas(CANVAS_SIZE,CANVAS_POS),background(sf::Vector2u(CANVAS_SIZE)){
    if(!font.openFromFile("../assets/PoetsenOne-Regular.ttf")){
        std::cout << "Font not loaded!" << std::endl;
    }
    sf::Texture shoya_image;
    shoya_image.loadFromFile("../assets/shoya.jpg");
    sf::Sprite shoya_sprite(shoya_image);
    background.draw(shoya_sprite);
}

void Paint::handle_sidebar_buttons(){
    if(sidebar.eraser_button.is_clicked()){
        if(current_tool != &(tools.eraser)){
            current_tool = &(tools.eraser);
        }
        // tools.pencil.change_color(sf::Color(0,255,0,100));
    }
    else{
        if(current_tool != &(tools.pencil)){
            current_tool = &(tools.pencil);
        }
        // tools.pencil.change_color(sf::Color::Red);
    }
}

void Paint::run(sf::RenderWindow& window){
    sf::Vector2f mouse_pos = mouse.get_mouse_position(window);
    mouse.updateButton(sf::Mouse::Button::Left);
    background.display();
    sf::Sprite background_sprite(background.getTexture());
    background_sprite.setPosition(CANVAS_POS);
    window.draw(background_sprite);
    canvas.draw(window);
    if(!(current_tool == nullptr)){
        current_tool->update(canvas,window,mouse);
    }
    sidebar.update(window,mouse);
    handle_sidebar_buttons();
}