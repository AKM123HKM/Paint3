#pragma once
#include "layer.h"
#include "mouse_utility.h"
#include "button_utility.h"
#include "tools.h"
#include "button_group.h"
#include <vector>
#include <memory>

const sf::Color BG_COLOR = sf::Color::White;
const sf::Vector2f CANVAS_SIZE = sf::Vector2f(800,516);
const sf::Vector2f CANVAS_POS = sf::Vector2f(0,84);

struct Tools{
    PencilTool pencil;
    EraserTool eraser;
};

class Paint{
    public:
    std::vector<std::unique_ptr<Layer>> layers;
    Layer* current_layer = nullptr;
    Mouse mouse;
    sf::Font font;
    ButtonGroup sidebar;
    // SideBar sidebar;
    sf::Color current_color = sf::Color::Red;
    sf::Color eraser_color = BG_COLOR;
    Tools tools;
    Tool* current_tool = &(tools.pencil);

    std::map<std::string,std::function<void()>> bindings = {
        {"eraser",[this]()mutable{
            if(current_tool != &tools.eraser){
                current_tool = &tools.eraser;
            }
            else{
                current_tool = &tools.pencil;
            }
        }},
        {"save",[](){std::cout << "Saving...." << std::endl;}},
        {"add_layer",[](){std::cout << "Added Layer :P" << std::endl;}}
    };

    Paint(nlohmann::json data);
    void run(sf::RenderWindow& window);
};