#pragma once
#include <iostream>
#include <vector>
#include <queue>

enum class Event{
    NullEvent,
    ButtonClicked,
    ButtonHover,
    ButtonDragging
};

struct CustomEvent{
    std::string id;
    Event type;
    // Optional event contexts
    /* group index gives the index at which the group lies in the dynamic group. Same buttons have the same id, so it tells which
    ui_element is giving the event.
    */
    std::vector<int> group_indexes;
    std::string group_id;
    CustomEvent(std::string a_id,Event a_type,std::vector<int> a_group_indexes,std::string a_group_id);
    CustomEvent();
};

class EventManager{
    public:
    std::queue<CustomEvent> events;
    CustomEvent null_event;
    void addEvent(Event event,std::string id,std::vector<int> a_group_indexes = {},std::string a_group_id = "");
    CustomEvent pollEvent();
};