#pragma once
#include "layer.h"
#include "mouse_utility.h"
#include "my_ui.h"
#include "tools.h"
#include <vector>
#include <memory>

const sf::Color BG_COLOR = sf::Color::White;
const sf::Vector2f CANVAS_SIZE = sf::Vector2f(800,566);
const sf::Vector2f CANVAS_POS = sf::Vector2f(0,35);

struct Tools{
    PencilTool pencil;
    EraserTool eraser;
};

class Paint{
    public:
    std::vector<std::unique_ptr<Layer>> layers;
    Layer* current_layer = nullptr;
    Mouse mouse;
    UI ui;
    sf::Font font;
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
        }
    },
        {"add_layers",[this](){
            ui.layers.add_element();
            }
    },
        {"show_layers",[this]()mutable{
            ui.toggle_layers = !(ui.toggle_layers);
        }
    }
    };

    Paint(nlohmann::json data,sf::RenderTarget& target);
    void run(sf::RenderWindow& window);
};