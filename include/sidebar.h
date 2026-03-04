#pragma once
#include "mouse_utility.h"
#include "button_utility.h"
#include "utility.h"
#include <tuple>
#include <vector>

class ToggleButtons{
    public:
        std::vector<Image_Button*> buttons;
        Image_Button* current_button = nullptr;

        void add_button(Image_Button* button);
        void toggle_button(Image_Button* button);
};

class SideBar{
    public:
        ImageButtonData eraser_data{sf::Vector2f(1,1),sf::Vector2f(10,10),"../assets/eraser.png","../assets/highlighted_eraser.png","../assets/clicked_eraser.png"};
        ImageButtonData save_data{sf::Vector2f(1,1),sf::Vector2f(84,10),"../assets/save.png","../assets/highlighted_save.png","../assets/clicked_save.png"};
        Image_Button eraser_button;
        Image_Button save_button;

        ToggleButtons group1;

        SideBar();
        void update(sf::RenderWindow& window,Mouse& mouse);
};