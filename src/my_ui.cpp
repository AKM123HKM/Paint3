#include "my_ui.h"

UI::UI(nlohmann::json data,sf::RenderTarget& target):sidebar(data["groups"][0]),layers(data["groups"][1]),scrollbar(parse_scrollbar_data(data["groups"][2])){
}

void UI::update(sf::RenderWindow& window,Mouse& mouse,EventManager& event_manager){
    FrameUIContext ui_context;
    ui_context.mouse_pos = mouse.getMousePosition(window);
    sidebar.update(window,mouse,event_manager,ui_context);
    sidebar.draw(window);
    if(toggle_layers){
        layers.update(window,mouse,event_manager,ui_context);
        layers.draw(window);
    }
    scrollbar.update(window,mouse,event_manager,ui_context);
    scrollbar.draw(window);
}

ImageButton* UI::getButton(const std::string a_id){
    ImageButton* button = sidebar.getButton(a_id);
    if(!(button == nullptr)){
        return button;
    }
    button = layers.getButton(a_id);
    return button;
}