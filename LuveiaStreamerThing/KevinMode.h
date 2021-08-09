#pragma once

#include "StreamerType.h"

class KevinMode : public StreamerType
{
public:
	KevinMode();

	void Init();
	void Draw(sf::RenderWindow& window);
private:
	int offset_x, offset_y;
	sf::Sprite bg, mouth, up, down, smoke;
	sf::Sprite mouse, mouseL, mouseR, mouseLR;

	//smoke related stuff
	bool drawSmoke = false;
	bool previousSmokeKeyState = false;
	bool currentSmokeKeyState = false;
	bool isToggleSmoke; //from config
	int smokeKeyValue; //from config
};

inline std::unique_ptr<KevinMode> kevinMode;