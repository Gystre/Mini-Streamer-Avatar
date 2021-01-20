#pragma once

#include "StreamerType.h"

#define BONGO_KEYPRESS_THRESHOLD 0

class Osu : public StreamerType
{
public:
	Osu();

	void Init();
	void Draw(sf::RenderWindow& window);

private:
	int offset_x, offset_y;
	int paw_r, paw_g, paw_b, paw_a;
	int paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a;
	double scale;
	bool isMouse, isLeftHanded, isEnableToggleSmoke;
	sf::Sprite bg, up, left, right, device, smoke, wave;
	bool drawSmoke = false;

	int keyState = 0;

	bool leftKeyState = false;
	bool rightKeyState = false;
	bool waveKeyState = false;
	bool previousSmokeKeyState = false;
	bool currentSmokeKeyState = false;
	bool isToggleSmoke = false;
	double timerLeftKey = -1;
	double timerRightKey = -1;
	double timerWaveKey = -1;

	int leftKeyValue, rightKeyValue, smokeKeyValue, waveKeyValue;
};

inline std::unique_ptr<Osu> osu;