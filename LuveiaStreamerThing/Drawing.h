#pragma once

#include "StreamerType.h"

class Drawing : public StreamerType
{
public:
	Drawing();

	void Init();
	void Draw(sf::RenderWindow& window);
private:
	int offset_x, offset_y;
	sf::Sprite bg, up, left, right, mouse, smoke;

	//smoke related stuff
	bool drawSmoke = false;
	bool previousSmokeKeyState = false;
	bool currentSmokeKeyState = false;
	bool isToggleSmoke; //from config
	int smokeKeyValue; //from config
};

inline std::unique_ptr<Drawing> drawing;