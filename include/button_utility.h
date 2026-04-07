#pragma once
#include "mouse_utility.h"
#include "utility.h"
#include <functional>
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include "json.hpp"


/* Base class used only to be able to store the different UIElements in a single containter.
*/
class UIElement{
    public:
    std::string type;
    virtual void update(sf::Vector2f mouse_pos,MouseButtonEvents left_button_event) = 0;
    virtual sf::Sprite get_sprite() = 0;
    virtual ~UIElement() = default;
};

/* Tried a different way of giving many arguments, this way we don't have to care about the order in which we give the args*/
struct ImageButtonData{
    sf::Vector2f scale;
    sf::Vector2f pos;
    std::string base_file_path;
    std::string highlighted_file_path;
    std::string selected_file_path;
    ImageButtonData();
    ImageButtonData(nlohmann::json data);
};

/* Uses a texture to draw the button. Previously i had one which used a shape, and one which uses a text. The name stayed but 
   the buttons didn't. */
class ImageButton:public UIElement{
    public:
        sf::Texture texture;
        sf::Texture highlighted_texture;
        sf::Texture selected_texture;
        sf::Sprite sprite = sf::Sprite(texture);
        sf::Vector2f original_scale;
        std::function<void()> func = [](){std::cout << "Clicked" << std::endl;}; 
        bool selected = false;

        ImageButton(ImageButtonData data);
        void set_func(const std::function<void()>& Afunc);
        sf::Sprite get_sprite();
        void update(sf::Vector2f mouse_pos,MouseButtonEvents left_button_event);
};

/* Adds button to a list and makes it so that only one button can be active at a time in the given list.*/
class ToggleButtons{
    public:
        std::vector<ImageButton*> buttons;
        ImageButton* current_button = nullptr;

        void add_button(ImageButton* button);
        void toggle_button(sf::RenderWindow& window,Mouse& mouse);
};

/* Adds UI elements inside a list and draws,updates them together*/
class ButtonGroup:public UIElement{
    public:
    std::string id;
    sf::Color color = sf::Color(100,100,100);
    sf::RenderTexture texture;
    sf::Sprite sprite;
    std::vector<std::pair<std::pair<std::string,std::string>,std::unique_ptr<UIElement>>> elements;
    std::vector<std::pair<std::string,ToggleButtons>> toggle_groups;

    ButtonGroup(const nlohmann::json data);
    sf::Sprite get_sprite();
    void update(sf::Vector2f mouse_pos,MouseButtonEvents left_button_event);
    ImageButton* get_button(const std::string id);
};

class DynamicButtonGroup:public ButtonGroup{
    public:
    sf::Vector2f element_offset = sf::Vector2f(0,50);
    sf::Vector2f last_offset = sf::Vector2f(0,0);
    nlohmann::json element_data;
    DynamicButtonGroup(const nlohmann::json data);
    void add_element();
};