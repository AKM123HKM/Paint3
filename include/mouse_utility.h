#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

enum class MouseButtonStates{
	Idle,
	Pressed,
	Held,
	Dragging
};

enum class MouseButtonEvents{
	None,
	Double_Click,
	Click,
	Release_Transition,
	Press_Transition
};

struct MouseButton{
	sf::Clock press_timer;
	sf::Clock click_timer;
	bool was_pressed{false};
	bool is_pressed{false};
	MouseButtonStates state = MouseButtonStates::Idle;
	MouseButtonEvents event = MouseButtonEvents::None;
	sf::Vector2f drag_value = sf::Vector2f(0,0);
};

class Mouse{
private:
	MouseButton left_button;
	MouseButton right_button;
	MouseButton middle_button;
	sf::Vector2f current_mouse_pos;
	sf::Vector2f prev_mouse_pos;
	const double HOLD_THRESHOLD = 0.5;
	const double DRAG_THRESHOLD = 1.0;
	const double DOUBLE_CLICK_THRESHOLD = 1;

	MouseButton& checkButton(sf::Mouse::Button button);

	float magnitude(sf::Vector2f vec);

public:

	sf::Vector2f getMousePosition(sf::RenderWindow& window);
	void updateMousePosition(sf::RenderWindow& window);
	void updateButton(sf::Mouse::Button);
	MouseButtonEvents getButtonEvent(sf::Mouse::Button);
	MouseButtonStates getButtonState(sf::Mouse::Button);
	sf::Vector2f getButtonDragValue(sf::Mouse::Button);
};