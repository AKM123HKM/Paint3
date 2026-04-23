#pragma once
#include <iostream>
#include "mouse_utility.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include "json.hpp"
#include "button_utility.h"
#include "event_utility.h"


std::ostream& operator<<(std::ostream& os, MouseButtonStates button_states);

std::ostream& operator<<(std::ostream& os, MouseButtonEvents button_events);

std::ostream& operator<<(std::ostream& os, sf::Vector2f vector);

std::ostream& operator<<(std::ostream& os, std::vector<int> vector);

std::ostream& operator<<(std::ostream& os,CustomEvent event);

std::ostream& operator<<(std::ostream& os,Event event);

bool compareMouseStates(MouseButtonStates a, MouseButtonStates b);

bool compareMouseEvents(MouseButtonEvents a, MouseButtonEvents b);

bool checkPointRectCollision(sf::Vector2f pos,sf::FloatRect rect);

sf::Vector2f getPerpendicular(sf::Vector2f x,sf::Vector2f y);

nlohmann::json loadJSON(const std::string file_path);

sf::Vector2f toLocalView(sf::Vector2f mouse_pos,sf::RenderTarget* target);

bool operator==(CustomEvent event1,CustomEvent event2);