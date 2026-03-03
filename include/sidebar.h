#pragma once
#include "mouse_utility.h"
#include "button_utility.h"
#include "utility.h"
#include <tuple>
#include <vector>

struct ImageButtonData{
    sf::Vector2f size;
    sf::Vector2f pos;
    std::string image_path;
};

class SideBar{
    public:
        Image_Button eraser_button;
        Image_Button save_button;
        SideBar();
        void update(sf::RenderWindow& window,Mouse& mouse);
};