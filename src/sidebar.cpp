#include "sidebar.h"

SideBar::SideBar():eraser_button(sf::Vector2f(1,1),sf::Vector2f(10,10),"../assets/eraser.png","../assets/highlighted_eraser.png"),
                   save_button(sf::Vector2f(1,1),sf::Vector2f(84,10),"../assets/save.png","../assets/highlighted_save.png"){
}

void SideBar::update(sf::RenderWindow& window,Mouse& mouse){
    eraser_button.update_button(window,mouse);
    save_button.update_button(window,mouse);
}