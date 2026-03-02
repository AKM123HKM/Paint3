#pragma once
#include <iostream>
#include "mouse_utility.h"
#include <SFML/Graphics.hpp>

std::ostream& operator<<(std::ostream& os, MouseButtonStates button_states);

std::ostream& operator<<(std::ostream& os, MouseButtonEvents button_events);

std::ostream& operator<<(std::ostream& os, sf::Vector2f vector);

bool compare_mouse_states(MouseButtonStates a, MouseButtonStates b);

bool compare_mouse_events(MouseButtonEvents a, MouseButtonEvents b);

bool check_point_rect_collision(sf::Vector2f pos,sf::FloatRect rect);