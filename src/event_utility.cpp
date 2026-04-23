#include "event_utility.h"

CustomEvent::CustomEvent(std::string a_id,Event a_type,std::vector<int> a_group_indexes,std::string a_group_id){
    id = a_id;
    type = a_type;
    group_indexes = a_group_indexes;
    group_id = a_group_id;
}
CustomEvent::CustomEvent(){
    id = "";
    type = Event::NullEvent;
}

void EventManager::addEvent(Event event,std::string id,std::vector<int> a_group_indexes,std::string a_group_id){
    events.emplace(id,event,a_group_indexes,a_group_id);
}

CustomEvent EventManager::pollEvent(){
    if(!(events.empty())){
        CustomEvent event = events.front();
        events.pop();
        return event;
    }
    return null_event;
}