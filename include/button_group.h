#pragma once
#include "mouse_utility.h"
#include "button_utility.h"
#include "utility.h"
#include <vector>
#include <memory>
#include <fstream>
#include "json.hpp"

class ToggleButtons{
    public:
        std::vector<ImageButton*> buttons;
        ImageButton* current_button = nullptr;

        void add_button(ImageButton* button);
        void toggle_button(sf::RenderWindow& window,Mouse& mouse);
};

class ButtonGroup{
    public:
    std::string id;
    ButtonGroup(const nlohmann::json data);
    void update(sf::RenderWindow& window,Mouse& mouse);
    ImageButton* get_button(const std::string id);
    protected:
    std::vector<std::pair<std::string,std::unique_ptr<ImageButton>>> buttons;
    std::vector<std::pair<std::string,ToggleButtons>> toggle_groups;
};