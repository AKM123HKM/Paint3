#include <SFML/Graphics.hpp>
#include <iostream>
#include "button_utility.h"

class UI{
    public:
        ButtonGroup sidebar;
        DynamicButtonGroup layers;
        bool toggle_layers = false;
        UI(nlohmann::json data,sf::RenderTarget& target);
        void update(sf::RenderWindow& window,Mouse& mouse);
};