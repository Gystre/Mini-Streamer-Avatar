#pragma once

#define TOTAl_INPUT_TABLE_SIZE 256
#define JOYSTICK_AXIS_DEADZONE 10.0f
#define JOYSTICK_TRIGGER_DEADZONE 3.0f

enum JoystickInputMapRange {
    MinButton = 0,
    MaxButton = 31,
    LS_Left,
    LS_Right,
    LS_Up,
    LS_Down,
    RS_Left,
    RS_Right,
    RS_Up,
    RS_Down,
    DPad_Left,
    DPad_Right,
    DPad_Up,
    DPad_Down,
    LTrigger,
    RTrigger
};

class Input
{
public:
    Input();
    void drawDebugPanel(sf::RenderWindow& window);
    std::pair<double, double> bezier(double ratio, std::vector<double>& points, int length);
    std::pair<double, double> getXY();

    bool isPressed(int key_code);
    bool isPressedFallback(int key_code);

    bool aKeyWasPressed = false; //set in the poll event for KeyPressed
private:
	int horizontal, vertical;
	int osu_x, osu_y, osu_h, osu_v;
	bool isLetterbox, isLeftHanded;

	int lastJoystickKeycode = -1;

	std::string debugMessage;
	std::string debugBitMessage;

	sf::RectangleShape debugBackground;
	sf::Font debugFont;
	sf::Text debugText;

    int INPUT_KEY_TABLE[TOTAl_INPUT_TABLE_SIZE];

    sf::Keyboard::Key asciiToKey(int key_code);
    bool isJoystickConnected();
    bool isJoystickPressed(int key_code);
};

inline std::unique_ptr<Input> input;