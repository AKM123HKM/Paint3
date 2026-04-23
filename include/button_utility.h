#pragma once
#include "mouse_utility.h"
#include "utility.h"
#include <functional>
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include "json.hpp"
#include "event_utility.h"

struct StaticUIContext{
    std::vector<int> group_indexes;
    std::string group_id;
    StaticUIContext();
};

struct FrameUIContext{
    sf::Vector2f mouse_pos;
    FrameUIContext();
};


/* Base class used only to be able to store the different UIElements in a single containter.
*/
class UIElement{
    public:
    std::string type;
    std::string id;
    virtual void update(sf::RenderWindow& window,Mouse& mouse,EventManager& event_manager,FrameUIContext context) = 0;
    virtual void draw(sf::RenderTarget& target) = 0;
    virtual sf::Vector2f getSize() = 0;
    virtual void setPosition(sf::Vector2f pos) = 0;
    virtual ~UIElement() = default;
};

class ImageButton;

class ClickBehaviour{
    public:
        void update(sf::RenderWindow& window,Mouse& mouse,EventManager& event_manager,ImageButton& button,bool hovering);
};

class DraggingBehaviour{
    public:
        sf::Vector2f range = sf::Vector2f(16,116);
        void update(sf::RenderWindow& window,Mouse& mouse,EventManager& event_manager,ImageButton& button,bool hovering);
};

/* Uses a texture to draw the button. Previously i had one which used a shape, and one which uses a text. The name stayed but 
   the buttons didn't. */
class ImageButton:public UIElement{
    public:
        struct ImageButtonData{
            std::string id;
            std::string type;
            bool dragging_behaviour = false;
            sf::Vector2f scale;
            sf::Vector2f pos;
            std::string texture_file_path;
            std::string highlighted_texture_file_path;
            std::string selected_texture_file_path;
            StaticUIContext ui_context;
            bool is_toggle_enabled;
            bool is_drag_horizontal = false;
        };

        sf::Texture texture;
        sf::Texture highlighted_texture;
        sf::Texture selected_texture;
        sf::Sprite sprite = sf::Sprite(texture);
        sf::Vector2f original_scale;
        std::variant<ClickBehaviour,DraggingBehaviour> behaviour;
        bool is_drag_horizontal;
        bool is_toggle_enabled = false; // This tells if the button should have a selected state or not
        bool selected = false; // This keeps tracks of the state if toggle_selected_state is active
        StaticUIContext ui_context;

        ImageButton(ImageButtonData data);
        void draw(sf::RenderTarget& target);
        sf::Vector2f getSize();
        void setPosition(sf::Vector2f pos);
        bool isHovering(sf::Vector2f mouse_pos);
        void handleInteraction(bool hovering,Mouse& mouse,EventManager& event_manager);
        void updateVisualState(bool hovering);
        void update(sf::RenderWindow& window,Mouse& mouse,EventManager& event_manager,FrameUIContext context);
};

class ScrollBar:public UIElement{
    public:
        struct ScrollBarData{
            std::string id;
            std::string type;
            ImageButton::ImageButtonData increment_data;
            ImageButton::ImageButtonData decrement_data;
            ImageButton::ImageButtonData thumb_data;
            sf::Vector2f track_dimensions;
            sf::Color track_color = sf::Color(100,100,100);
            sf::Vector2f track_pos;
            sf::Vector2u scrollbar_dimensions = {156,28};
            sf::Vector2f scrollbar_pos = {0,0};
            sf::Color scrollbar_color = sf::Color(150,150,150);
            sf::Vector2i range = {0,100};
            bool horizontal_bar = true;
            int start_value = 50;
            int step = 1; // Increment and decrement value
            StaticUIContext ui_context;
        };

        sf::RenderTexture texture;
        sf::Sprite sprite;
        sf::Color bg_color;
        sf::Vector2i range;
        int current_value;
        int step;
        bool horizontal_bar = true;
        StaticUIContext ui_context;

        ImageButton increment_button;
        ImageButton decrement_button;
        sf::RectangleShape track;
        ImageButton thumb;
        void draw(sf::RenderTarget& target);
        sf::Vector2f getSize();
        void setPosition(sf::Vector2f pos);
        sf::Vector2f toLocalPos(sf::Vector2f global_mouse_pos);
        void updateElements();
        void finalizeRender();
        void update(sf::RenderWindow& window,Mouse& mouse,EventManager& event_mangaer,FrameUIContext context);
        ScrollBar(ScrollBarData data);

};

/* Adds UI elements inside a list and draws,updates them together*/
class ElementGroup:public UIElement{
    public:
    sf::Color color = sf::Color(100,100,100);
    sf::RenderTexture texture;
    sf::Sprite sprite;
    std::vector<std::unique_ptr<UIElement>> elements;
    StaticUIContext ui_context;

    ElementGroup(const nlohmann::json data,StaticUIContext a_context = {});
    void draw(sf::RenderTarget& target);
    sf::Vector2f getSize();
    void setPosition(sf::Vector2f pos);
    void update(sf::RenderWindow& window,Mouse& mouse,EventManager& event_manager,FrameUIContext context);
    sf::Vector2f toLocalPos(sf::Vector2f global_mouse_pos);
    void finalizeRender();
    ImageButton* getButton(const std::string id);
};

class DynamicElementGroup:public ElementGroup{
    public:
    sf::Vector2f element_offset;
    sf::Vector2f element_size;
    float current_offset;
    int element_index;
    bool horizontal_orientation;
    nlohmann::json element_data;
    StaticUIContext create_child_context();
    void resizeTextureAndSprite();
    sf::Vector2f getNextPosition();
    void advanceOffset();
    DynamicElementGroup(const nlohmann::json data,StaticUIContext a_context = {});
    void addElement();
};


ImageButton::ImageButtonData parse_button_data(nlohmann::json data);
ScrollBar::ScrollBarData parse_scrollbar_data(nlohmann::json data);
