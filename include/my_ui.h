#include <SFML/Graphics.hpp>
#include <iostream>
#include "button_utility.h"
#include "event_utility.h"

class UI{
    public:
        ElementGroup sidebar;
        DynamicElementGroup layers;
        ScrollBar scrollbar;
        bool toggle_layers = false;
        UI(nlohmann::json data,sf::RenderTarget& target);
        void update(sf::RenderWindow& window,Mouse& mouse,EventManager& a_event_manager);
        ImageButton* getButton(const std::string a_id);
};