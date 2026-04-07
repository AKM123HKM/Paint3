#include "mouse_utility.h"
#include <iostream>

MouseButton& Mouse::check_button(sf::Mouse::Button button){
	if (sf::Mouse::Button::Left == button){
		return left_button;
	}
	else if (sf::Mouse::Button::Right == button){
		return right_button;
	}
	return middle_button;
}

float Mouse::magnitude(sf::Vector2f vec){
	return sqrt(vec.x*vec.x + vec.y*vec.y);
}

sf::Vector2f Mouse::get_mouse_position(sf::RenderWindow& window){
	sf::Vector2i mouse_pixel =  sf::Mouse::getPosition(window);
	sf::Vector2f mouse_pos = window.mapPixelToCoords(mouse_pixel);

	return mouse_pos;
}

void Mouse::updateMousePosition(sf::RenderWindow& window){
	prev_mouse_pos = current_mouse_pos;
	sf::Vector2i mouse_pixel = sf::Mouse::getPosition(window);
	current_mouse_pos = window.mapPixelToCoords(mouse_pixel);
}

void Mouse::updateButton(sf::Mouse::Button aButton){
	auto& button = check_button(aButton);
	button.event = MouseButtonEvents::None;
	if (sf::Mouse::isButtonPressed(aButton)){
		if(button.state == MouseButtonStates::Idle){
			button.event = MouseButtonEvents::Press_Transition;
			button.press_timer.restart();
		}
		button.state = MouseButtonStates::Pressed;

		if (button.state == MouseButtonStates::Pressed & button.press_timer.getElapsedTime().asSeconds() > HOLD_THRESHOLD){
			button.state = MouseButtonStates::Held;
		}

		if (button.state == MouseButtonStates::Pressed || button.state == MouseButtonStates::Held){
			if (magnitude(prev_mouse_pos - current_mouse_pos) >= DRAG_THRESHOLD){
				button.state = MouseButtonStates::Dragging;
			}
		}
	}
	else{
		if(button.state != MouseButtonStates::Idle){
			button.event = MouseButtonEvents::Release_Transition;
		}
		if(button.state == MouseButtonStates::Pressed){
			if(button.click_timer.getElapsedTime().asSeconds() <= DOUBLE_CLICK_THRESHOLD){
				button.event = MouseButtonEvents::Double_Click;
				button.click_timer.restart();
			}
			button.event = MouseButtonEvents::Click;
			button.click_timer.restart();
		}
		button.state = MouseButtonStates::Idle;
		button.press_timer.reset();
		button.press_timer.stop();
	}
}

MouseButtonStates Mouse::get_button_state(sf::Mouse::Button aButton){
	auto& button = check_button(aButton);
	return button.state;
}

MouseButtonEvents Mouse::get_button_event(sf::Mouse::Button aButton){
	auto& button = check_button(aButton);
	return button.event;
}