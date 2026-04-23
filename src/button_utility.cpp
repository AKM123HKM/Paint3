#include "button_utility.h"
#include <iostream>

StaticUIContext::StaticUIContext(){
    group_indexes = {};
    group_id = "";
}

FrameUIContext::FrameUIContext(){
    mouse_pos = sf::Vector2f{0,0};
}

// ---------------------------------------------------------- BEHAVIOUR -----------------------------------------------------------

void ClickBehaviour::update(sf::RenderWindow& window,Mouse& mouse,EventManager& event_manager,ImageButton& button,bool hovering){
    MouseButtonEvents left_button_event = mouse.getButtonEvent(sf::Mouse::Button::Left);
    if(hovering){
        event_manager.addEvent(Event::ButtonHover,button.id,button.ui_context.group_indexes);
        if(compareMouseEvents(left_button_event,MouseButtonEvents::Click)){
            if(button.is_toggle_enabled){
                button.selected = !(button.selected);
            }
            event_manager.addEvent(Event::ButtonClicked,button.id,button.ui_context.group_indexes);
        }
    }
}

void DraggingBehaviour::update(sf::RenderWindow& window,Mouse& mouse,EventManager& event_manager,ImageButton& button,bool hovering){
    MouseButtonStates left_button_state = mouse.getButtonState(sf::Mouse::Button::Left);
    MouseButtonEvents left_button_event = mouse.getButtonEvent(sf::Mouse::Button::Left);

    //TODO:Add the events for Press_Transition and Release_Transition

    if(hovering){
        if(compareMouseEvents(left_button_event,MouseButtonEvents::Press_Transition)){
            button.selected = true;
        }
        event_manager.addEvent(Event::ButtonHover,button.id,button.ui_context.group_indexes);
    }

    if(compareMouseEvents(left_button_event,MouseButtonEvents::Release_Transition)){
            button.selected = false;
    }

    if(button.selected && compareMouseStates(left_button_state,MouseButtonStates::Dragging)){
        event_manager.addEvent(Event::ButtonDragging,button.id,button.ui_context.group_indexes,button.ui_context.group_id);
        sf::Vector2f drag_value = mouse.getButtonDragValue(sf::Mouse::Button::Left);

        if(button.is_drag_horizontal){
                sf::Vector2f pos = sf::Vector2f(button.sprite.getPosition().x + drag_value.x,button.sprite.getPosition().y);
                bool out_of_range = pos.x < range.x || pos.x > range.y;
                if(!out_of_range){
                    button.sprite.setPosition(pos);
                }
        }
        else{
            sf::Vector2f pos = sf::Vector2f(button.sprite.getPosition().x,button.sprite.getPosition().y + drag_value.y);
            bool out_of_range = pos.y < range.x || pos.y > range.y;
            if(!out_of_range){
                button.sprite.setPosition(pos);
            }
        }
    }
}

// ----------------------------------------------------------- IMAGEBUTTON ---------------------------------------------------------

ImageButton::ImageButton(ImageButtonData data):
                           texture(data.texture_file_path),
                           sprite(texture),
                           highlighted_texture(data.highlighted_texture_file_path),
                           selected_texture(data.selected_texture_file_path){
    id = data.id;
    type = data.type;
    ui_context = data.ui_context;
    is_toggle_enabled = data.is_toggle_enabled;
    texture.setSmooth(true);
    highlighted_texture.setSmooth(true);
    selected_texture.setSmooth(true);
    sprite.setTexture(texture);
    sprite.setPosition(data.pos);
    original_scale = data.scale;
    sprite.setScale(original_scale);
    is_drag_horizontal = data.is_drag_horizontal;
    if(data.dragging_behaviour){
        behaviour = DraggingBehaviour();
    }
    else{
        behaviour = ClickBehaviour();
    }
}   

void ImageButton::draw(sf::RenderTarget& target){
    target.draw(sprite);
}

bool ImageButton::isHovering(sf::Vector2f mouse_pos){
    return(checkPointRectCollision(mouse_pos,sprite.getGlobalBounds()));
}

void ImageButton::updateVisualState(bool hovering){
    if(hovering){
        sprite.setScale(original_scale + sf::Vector2f(0.1,0.1));
        sprite.setTexture(highlighted_texture);
    }
    else{
        sprite.setScale(original_scale);
        sprite.setTexture(texture);
    }

    if(selected){
        sprite.setTexture(selected_texture);
    }
}

sf::Vector2f ImageButton::getSize(){
    return sprite.getGlobalBounds().size;
}

void ImageButton::setPosition(sf::Vector2f pos){
    sprite.setPosition(pos);
}

void ImageButton::update(sf::RenderWindow& window,Mouse& mouse,EventManager& event_manager,FrameUIContext context){
    bool hovering = isHovering(context.mouse_pos);

    // calls the CickBehaviour or DraggingBehaviour update function based on the type of button.
    std::visit([this,&window,&mouse,&event_manager,hovering](auto& b){
        b.update(window,mouse,event_manager,*this,hovering);
    },behaviour);

    updateVisualState(isHovering(context.mouse_pos));
}


// ----------------------------------------------------- SCROLLBAR --------------------------------------------------------------

ScrollBar::ScrollBar(ScrollBarData data):thumb(data.thumb_data),
                                   track(data.track_dimensions),
                                   increment_button(data.increment_data),
                                   decrement_button(data.decrement_data),
                                   texture(data.scrollbar_dimensions),
                                   sprite(texture.getTexture()){
    sprite.setPosition(data.scrollbar_pos);
    bg_color = data.scrollbar_color;
    horizontal_bar = data.horizontal_bar;
    track.setFillColor(data.track_color);
    track.setPosition(data.track_pos);
    current_value = data.start_value;
    step = data.step;
    range = data.range;
    ui_context = data.ui_context;
}

void ScrollBar::finalizeRender(){
    texture.display();
    sprite.setTexture(texture.getTexture());
}

sf::Vector2f ScrollBar::toLocalPos(sf::Vector2f global_mouse_pos){
    sf::Vector2f transformed_pos = sprite.getInverseTransform().transformPoint(global_mouse_pos);
    return toLocalView(transformed_pos,&texture);
}

void ScrollBar::update(sf::RenderWindow& window,Mouse& mouse,EventManager& event_mangaer,FrameUIContext context){
    FrameUIContext temp_context;
    temp_context.mouse_pos = toLocalPos(context.mouse_pos);

    increment_button.update(window,mouse,event_mangaer,temp_context);
    decrement_button.update(window,mouse,event_mangaer,temp_context);
    thumb.update(window,mouse,event_mangaer,temp_context);
}

void ScrollBar::draw(sf::RenderTarget& target){
    texture.clear(bg_color);

    texture.draw(track);
    increment_button.draw(texture);
    decrement_button.draw(texture);
    thumb.draw(texture);

    finalizeRender();

    target.draw(sprite);
}

sf::Vector2f ScrollBar::getSize(){
    return sprite.getGlobalBounds().size;
}

void ScrollBar::setPosition(sf::Vector2f pos){
    sprite.setPosition(pos);
}

// ------------------------------------------------------ ELEMENT_GROUP ----------------------------------------------------------

ElementGroup::ElementGroup(const nlohmann::json data,StaticUIContext a_context):texture(sf::Vector2u(100,100)),
                                                                                sprite(texture.getTexture()){
    id = data["id"];
    type = data["type"];
    ui_context = a_context;

    sf::Vector2f offset = sf::Vector2f((data.at("offset"))[0],(data.at("offset"))[1]);
    sf::Vector2u size = sf::Vector2u(0,0);
    sf::Vector2f element_pos = offset;
    bool horizontal_orientation = data.at("horizontal_orientation");
    int i = 0;
    for(auto element:data.at("elements")){
        if (element["type"] == "button"){
            ImageButton::ImageButtonData button_data = parse_button_data(element);
            button_data.ui_context.group_indexes = ui_context.group_indexes;
            button_data.ui_context.group_indexes.push_back(i);
            button_data.ui_context.group_id = id;
            elements.emplace_back(std::make_unique<ImageButton>(button_data));
        }
        else if (element["type"] == "group"){
            StaticUIContext temp_context;
            temp_context.group_id = id;
            temp_context.group_indexes = ui_context.group_indexes;
            temp_context.group_indexes.push_back(i);
            elements.emplace_back(std::make_unique<ElementGroup>(element,temp_context));
        }

        // Setting up elements position using their size and offset.
        elements[i].get()->setPosition(element_pos);
        sf::Vector2u element_size = sf::Vector2u(elements[i].get()->getSize());
        if(horizontal_orientation){
            size = sf::Vector2u(element_size.x + size.x + offset.x,std::max(element_size.y,size.y));
            element_pos = sf::Vector2f(element_pos.x + element_size.x + offset.x,offset.y);
        }
        else{
            size = sf::Vector2u(std::max(element_size.x,size.x),element_size.y + size.y + offset.y);
            element_pos = sf::Vector2f(offset.x,element_pos.y + element_size.y + offset.y);
        }
        i++;
    }

    size += sf::Vector2u(offset);

    if(!(texture.resize(size))){
        std::cout << "Failed to resize the texture" << std::endl;
    }
    sprite.setTexture(texture.getTexture(),true);
    sf::Vector2f pos = sf::Vector2f(data["pos"][0],data["pos"][1]);
    sprite.setPosition(pos);
    color = sf::Color(data["color"][0],data["color"][1],data["color"][2]);
    
}

void ElementGroup::update(sf::RenderWindow& window,Mouse& mouse,EventManager& event_manager,FrameUIContext context){
    // change the global mouse coordinates to the texture's view coordinates
    FrameUIContext temp_context = context;
    temp_context.mouse_pos = toLocalPos(context.mouse_pos);

    for(std::unique_ptr<UIElement>& element:elements){
        element->update(window,mouse,event_manager,temp_context);
    }
}

void ElementGroup::draw(sf::RenderTarget& target){
    texture.clear(sf::Color(150,150,150));
    
    for(std::unique_ptr<UIElement>& element:elements){
        element->draw(texture);
    }

    finalizeRender();
    target.draw(sprite);
}

sf::Vector2f ElementGroup::getSize(){
    return sprite.getGlobalBounds().size;
}

sf::Vector2f ElementGroup::toLocalPos(sf::Vector2f global_mouse_pos){
    sf::Vector2f transformed_pos = sprite.getInverseTransform().transformPoint(global_mouse_pos);
    return toLocalView(transformed_pos,&texture);
}

void ElementGroup::finalizeRender(){
    texture.display();
    sprite.setTexture(texture.getTexture());
}

void ElementGroup::setPosition(sf::Vector2f pos){
    sprite.setPosition(pos);
}

ImageButton* ElementGroup::getButton(const std::string a_id){
    for(int i = 0;i < elements.size();i++){
        if(elements[i]->type == "button"){
            if(elements[i]->id == a_id){
                return static_cast<ImageButton*>(elements[i].get());
            }
        }
        else if(elements[i]->type == "group"){
            ElementGroup* group_ptr = static_cast<ElementGroup*>(elements[i].get());
            ImageButton* button_ptr = group_ptr->getButton(a_id);
            if(!(button_ptr == nullptr)){
                return button_ptr;
            }
        }
    }
    return nullptr;
}

// --------------------------------------------------- DYNAMIC_ELEMENT_GROUP ---------------------------------------------------

DynamicElementGroup::DynamicElementGroup(const nlohmann::json data,StaticUIContext a_context):
                                        ElementGroup(data){
    element_size = sprite.getGlobalBounds().size;
    element_offset = sf::Vector2f(data.at("offset")[0],data.at("offset")[1]);
    element_index = 1;
    horizontal_orientation = data.at("horizontal_orientation");
    element_data = data["elements"][0];
}

StaticUIContext DynamicElementGroup::create_child_context(){
    StaticUIContext temp_context;
    temp_context.group_id = id;
    temp_context.group_indexes = ui_context.group_indexes;
    temp_context.group_indexes.push_back(element_index);

    return temp_context;
}

void DynamicElementGroup::resizeTextureAndSprite(){
    sf::Vector2u size;
    if(horizontal_orientation){
        float width = element_size.x * element_index + (element_index + 1)*element_offset.x;
        size = sf::Vector2u(width,element_size.y);
    }
    else{
        float height = element_size.y * element_index + (element_index + 1) * element_offset.y;
        size = sf::Vector2u(element_size.x , height);
    }
    if(!(texture.resize(size))){
        std::cout << "Failed to resize texture!" << std::endl;
    }

    sprite.setTexture(texture.getTexture(),true);
}

sf::Vector2f DynamicElementGroup::getNextPosition(){
    if(horizontal_orientation){
        return sf::Vector2f(element_size.x + element_offset.x + current_offset,element_offset.y);
    }
    else{
        return sf::Vector2f(element_offset.x, element_size.y + element_offset.y + current_offset);
    }
}

void DynamicElementGroup::advanceOffset(){
    if(horizontal_orientation){
        current_offset += element_size.x + element_offset.x;
    }
    else{
        current_offset += element_size.y + element_offset.y;
    }
}

void DynamicElementGroup::addElement(){
    nlohmann::json element_data_instance = element_data;

    sf::Vector2f newPos = getNextPosition();
    element_data_instance["pos"] = nlohmann::json::array({newPos.x, newPos.y});
    advanceOffset();

    if(element_data_instance.at("type") == "group"){
        StaticUIContext temp_context = create_child_context();
        elements.emplace_back(std::make_unique<ElementGroup>(element_data_instance,temp_context));
    }
    else if(element_data_instance.at("type") == "button"){
        ImageButton::ImageButtonData button_data = parse_button_data(element_data_instance);
        button_data.ui_context = create_child_context();
        elements.emplace_back(std::make_unique<ImageButton>(button_data));
    }

    element_index++;
    resizeTextureAndSprite();
}

ImageButton::ImageButtonData parse_button_data(nlohmann::json data){
    ImageButton::ImageButtonData button_data;
    button_data.id = data.at("id");
    button_data.type = data.at("type");
    button_data.dragging_behaviour = data.at("dragging_behaviour");
    if(data.contains("pos")){
        button_data.pos = sf::Vector2f(data.at("pos")[0],data.at("pos")[1]);
    }
    button_data.scale = sf::Vector2f(data.at("scale")[0],data.at("scale")[1]);
    button_data.texture_file_path = data.at("texture");
    button_data.highlighted_texture_file_path = data.at("highlighted_texture");
    button_data.selected_texture_file_path = data.at("selected_texture");
    button_data.is_toggle_enabled = data.at("toggle_selected_state");
    return button_data;
}

ScrollBar::ScrollBarData parse_scrollbar_data(nlohmann::json data){
    ScrollBar::ScrollBarData scrollbar_data;
    sf::Vector2f offset;
    sf::Vector2f assumed_images_dimensions = sf::Vector2f(24,24);

    // Optional values
    if(data.contains("sidebar_color")){
        scrollbar_data.scrollbar_color = sf::Color(data["sidebar_color"][0],data["sidebar_color"][1],data["sidebar_color"][1]);
    }
    else{
        scrollbar_data.scrollbar_color = sf::Color(100,100,100);
    }

    if(data.contains("track_color")){
        scrollbar_data.track_color = sf::Color(data["track_color"][0],data["track_color"][1],data["track_color"][2]);
    }
    else{
        scrollbar_data.track_color = sf::Color(150,150,150);
    }

    if(data.contains("offset")){
        offset = sf::Vector2f(data["offset"][0],data["offset"][1]);
    }
    else{
        offset = sf::Vector2f(2,2);
    }

    if(data.contains("start_value")){
        scrollbar_data.start_value = data["start_value"];
    }
    else{
        scrollbar_data.start_value = 0;
    }

    if(data.contains("step")){
        scrollbar_data.step = data["step"];
    }
    else{
        scrollbar_data.step = 1;
    }

    // Must have values
    scrollbar_data.increment_data = parse_button_data(data.at("elements").at("increment_data"));
    scrollbar_data.decrement_data = parse_button_data(data.at("elements").at("decrement_data"));
    scrollbar_data.thumb_data = parse_button_data(data.at("elements").at("thumb_data"));
    scrollbar_data.horizontal_bar = data.at("horizontal_bar");
    scrollbar_data.thumb_data.is_drag_horizontal = scrollbar_data.horizontal_bar;

    scrollbar_data.increment_data.pos = offset;
    scrollbar_data.track_pos = sf::Vector2f(2*offset.x + assumed_images_dimensions.x,offset.y);
    scrollbar_data.track_dimensions = sf::Vector2f(data.at("track_length"),assumed_images_dimensions.y);
    scrollbar_data.range = sf::Vector2i(data.at("range")[0],data.at("range")[1]);

    //warning
    if(data.at("range")[0] >= data.at("range")[1]){
        std::cout << "Invalid range" << std::endl;
    }

    if(scrollbar_data.start_value == 0){
       scrollbar_data.thumb_data.pos = sf::Vector2f(data.at("range")[0],offset.y);
    }
    else{
       scrollbar_data.thumb_data.pos = sf::Vector2f(scrollbar_data.track_pos.x + ((data.at("range")[1].get<int>() - data.at("range")[0].get<int>())/scrollbar_data.start_value),offset.y);
    }
    scrollbar_data.decrement_data.pos = sf::Vector2f(scrollbar_data.track_pos.x + scrollbar_data.track_dimensions.x,offset.y);
    scrollbar_data.scrollbar_dimensions = sf::Vector2u(4*offset.x + 2*assumed_images_dimensions.x + scrollbar_data.track_dimensions.x ,assumed_images_dimensions.y + 2*offset.y);
    scrollbar_data.scrollbar_pos = sf::Vector2f(data.at("pos")[0],data.at("pos")[1]);

    return scrollbar_data;
}
