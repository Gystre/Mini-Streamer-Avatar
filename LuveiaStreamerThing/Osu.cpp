#include "header.hpp"
#include "Osu.h"
#include "Config.h"
#include "ImageLoader.h"
#include "Input.h"

Osu::Osu() {}

//i would do this in the constructor but like i like inheritiance :P
void Osu::Init()
{
	isMouse = config->osu.Mouse;
	isToggleSmoke = config->osu.ToggleSmoke;

	paw_r = config->osu.PawColor[0];
	paw_g = config->osu.PawColor[1];
	paw_b = config->osu.PawColor[2];
	paw_a = config->osu.PawColor[3];

	paw_edge_r = config->osu.PawEdgeColor[0];
	paw_edge_g = config->osu.PawEdgeColor[1];
	paw_edge_b = config->osu.PawEdgeColor[2];
	paw_edge_a = config->osu.PawEdgeColor[3];

	//read in the hotkeys
	leftKeyValue = config->osu.Key1;
	rightKeyValue = config->osu.Key2;
	smokeKeyValue = config->osu.Smoke;
	waveKeyValue = config->osu.Wave;

	//so unintuitive... the first value in the array corresponds to the mouse while the second
	//value corresponds to the pen
	if (isMouse)
	{
		offset_x = config->decoration.OffsetMouse[0];
		offset_y = config->decoration.OffsetMouse[1];
		scale = config->decoration.MouseScalar;
	}
	else
	{
		offset_x = config->decoration.OffsetPen[0];
		offset_y = config->decoration.OffsetPen[1];
		scale = config->decoration.PenScalar;
	}

	//importing sprites
	up.setTexture(imageLoader->loadTexture("img/osu/up.png"));
	left.setTexture(imageLoader->loadTexture("img/osu/left.png"));
	right.setTexture(imageLoader->loadTexture("img/osu/right.png"));
	wave.setTexture(imageLoader->loadTexture("img/osu/wave.png"));
	if (isMouse) {
		bg.setTexture(imageLoader->loadTexture("img/osu/mousebg.png"));
		device.setTexture(imageLoader->loadTexture("img/osu/mouse.png"), true);
	}
	else {
		bg.setTexture(imageLoader->loadTexture("img/osu/tabletbg.png"));
		device.setTexture(imageLoader->loadTexture("img/osu/tablet.png"), true);
	}
	smoke.setTexture(imageLoader->loadTexture("img/osu/smoke.png"));
	device.setScale(scale, scale);
}

void Osu::Draw(sf::RenderWindow& window)
{
	window.draw(bg);

    // initializing pss and pss2 (kuvster's magic)
    int x_paw_start = config->mousePaw.PawStartingPoint[0];
    int y_paw_start = config->mousePaw.PawStartingPoint[1];
    auto [x, y] = input->getXY();
    int oof = 6;
    std::vector<double> pss = { (float)x_paw_start, (float)y_paw_start };
    double dist = hypot(x_paw_start - x, y_paw_start - y);
    double centreleft0 = x_paw_start - 0.7237 * dist / 2;
    double centreleft1 = y_paw_start + 0.69 * dist / 2;
    for (int i = 1; i < oof; i++) {
        std::vector<double> bez = { (float)x_paw_start, (float)y_paw_start, centreleft0, centreleft1, x, y };
        auto [p0, p1] = input->bezier(1.0 * i / oof, bez, 6);
        pss.push_back(p0);
        pss.push_back(p1);
    }
    pss.push_back(x);
    pss.push_back(y);
    double a = y - centreleft1;
    double b = centreleft0 - x;
    double le = hypot(a, b);
    a = x + a / le * 60;
    b = y + b / le * 60;
    int x_paw_end = config->mousePaw.PawEndingPoint[0];
    int y_paw_end = config->mousePaw.PawEndingPoint[1];
    dist = hypot(x_paw_end - a, y_paw_end - b);
    double centreright0 = x_paw_end - 0.6 * dist / 2;
    double centreright1 = y_paw_end + 0.8 * dist / 2;
    int push = 20;
    double s = x - centreleft0;
    double t = y - centreleft1;
    le = hypot(s, t);
    s *= push / le;
    t *= push / le;
    double s2 = a - centreright0;
    double t2 = b - centreright1;
    le = hypot(s2, t2);
    s2 *= push / le;
    t2 *= push / le;
    for (int i = 1; i < oof; i++) {
        std::vector<double> bez = { x, y, x + s, y + t, a + s2, b + t2, a, b };
        auto [p0, p1] = input->bezier(1.0 * i / oof, bez, 8);
        pss.push_back(p0);
        pss.push_back(p1);
    }
    pss.push_back(a);
    pss.push_back(b);
    for (int i = oof - 1; i > 0; i--) {
        std::vector<double> bez = { 1.0 * x_paw_end, 1.0 * y_paw_end, centreright0, centreright1, a, b };
        auto [p0, p1] = input->bezier(1.0 * i / oof, bez, 6);
        pss.push_back(p0);
        pss.push_back(p1);
    }
    pss.push_back(x_paw_end);
    pss.push_back(y_paw_end);
    double mpos0 = (a + x) / 2 - 52 - 15;
    double mpos1 = (b + y) / 2 - 34 + 5;
    double dx = -38;
    double dy = -50;

    const int iter = 25;

    std::vector<double> pss2 = { pss[0] + dx, pss[1] + dy };
    for (int i = 1; i < iter; i++) {
        auto [p0, p1] = input->bezier(1.0 * i / iter, pss, 38);
        pss2.push_back(p0 + dx);
        pss2.push_back(p1 + dy);
    }
    pss2.push_back(pss[36] + dx);
    pss2.push_back(pss[37] + dy);

    device.setPosition(mpos0 + dx + offset_x, mpos1 + dy + offset_y);

    // drawing mouse
    if (isMouse) {
        window.draw(device);
    }

    // drawing arms
    sf::VertexArray fill(sf::TriangleStrip, 26);
    for (int i = 0; i < 26; i += 2) {
        fill[i].position = sf::Vector2f(pss2[i], pss2[i + 1]);
        fill[i + 1].position = sf::Vector2f(pss2[52 - i - 2], pss2[52 - i - 1]);
        fill[i].color = sf::Color(paw_r, paw_g, paw_b, paw_a);
        fill[i + 1].color = sf::Color(paw_r, paw_g, paw_b, paw_a);
    }
    window.draw(fill);

    // drawing first arm arc
    int shad = paw_edge_a / 3;
    sf::VertexArray edge(sf::TriangleStrip, 52);
    double width = 7;
    sf::CircleShape circ(width / 2);
    circ.setFillColor(sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, shad));
    circ.setPosition(pss2[0] - width / 2, pss2[1] - width / 2);
    window.draw(circ);
    for (int i = 0; i < 50; i += 2) {
        double vec0 = pss2[i] - pss2[i + 2];
        double vec1 = pss2[i + 1] - pss2[i + 3];
        double dist = hypot(vec0, vec1);
        edge[i].position = sf::Vector2f(pss2[i] + vec1 / dist * width / 2, pss2[i + 1] - vec0 / dist * width / 2);
        edge[i + 1].position = sf::Vector2f(pss2[i] - vec1 / dist * width / 2, pss2[i + 1] + vec0 / dist * width / 2);
        edge[i].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, shad);
        edge[i + 1].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, shad);
        width -= 0.08;
    }
    double vec0 = pss2[50] - pss2[48];
    double vec1 = pss2[51] - pss2[49];
    dist = hypot(vec0, vec1);
    edge[51].position = sf::Vector2f(pss2[50] + vec1 / dist * width / 2, pss2[51] - vec0 / dist * width / 2);
    edge[50].position = sf::Vector2f(pss2[50] - vec1 / dist * width / 2, pss2[51] + vec0 / dist * width / 2);
    edge[50].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, shad);
    edge[51].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, shad);
    window.draw(edge);
    circ.setRadius(width / 2);
    circ.setPosition(pss2[50] - width / 2, pss2[51] - width / 2);
    window.draw(circ);

    // drawing second arm arc
    sf::VertexArray edge2(sf::TriangleStrip, 52);
    width = 6;
    sf::CircleShape circ2(width / 2);
    circ2.setFillColor(sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a));
    circ2.setPosition(pss2[0] - width / 2, pss2[1] - width / 2);
    window.draw(circ2);
    for (int i = 0; i < 50; i += 2) {
        vec0 = pss2[i] - pss2[i + 2];
        vec1 = pss2[i + 1] - pss2[i + 3];
        double dist = hypot(vec0, vec1);
        edge2[i].position = sf::Vector2f(pss2[i] + vec1 / dist * width / 2, pss2[i + 1] - vec0 / dist * width / 2);
        edge2[i + 1].position = sf::Vector2f(pss2[i] - vec1 / dist * width / 2, pss2[i + 1] + vec0 / dist * width / 2);
        edge2[i].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a);
        edge2[i + 1].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a);
        width -= 0.08;
    }
    vec0 = pss2[50] - pss2[48];
    vec1 = pss2[51] - pss2[49];
    dist = hypot(vec0, vec1);
    edge2[51].position = sf::Vector2f(pss2[50] + vec1 / dist * width / 2, pss2[51] - vec0 / dist * width / 2);
    edge2[50].position = sf::Vector2f(pss2[50] - vec1 / dist * width / 2, pss2[51] + vec0 / dist * width / 2);
    edge2[50].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a);
    edge2[51].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a);
    window.draw(edge2);
    circ2.setRadius(width / 2);
    circ2.setPosition(pss2[50] - width / 2, pss2[51] - width / 2);
    window.draw(circ2);

    // drawing keypresses
    bool leftKey = input->isPressed(leftKeyValue);

    if (leftKey) {
        if (!leftKeyState) {
            keyState = 1;
            leftKeyState = true;
        }
    }
    else {
        leftKeyState = false;
    }

    bool rightKey = input->isPressed(rightKeyValue);
    if (rightKey) {
        if (!rightKeyState) {
            keyState = 2;
            rightKeyState = true;
        }
    }
    else {
        rightKeyState = false;
    }

    bool waveKey = input->isPressed(waveKeyValue);
    if (waveKey) {
        if (!waveKeyState) {
            keyState = 3;
            waveKeyState = true;
        }
    }
    else {
        waveKeyState = false;
    }

    if (!leftKeyState && !rightKeyState && !waveKeyState) 
    {
        keyState = 0;
        window.draw(up);
    }


    if (keyState == 1) {
        if ((clock() - std::max(timerRightKey, timerWaveKey)) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
            if (!isLeftHanded) {
                window.draw(left);
            }
            else {
                window.draw(right);
            }
            timerLeftKey = clock();
        }
        else {
            window.draw(up);
        }
    }
    else if (keyState == 2) {
        if ((clock() - std::max(timerLeftKey, timerWaveKey)) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
            if (!isLeftHanded) {
                window.draw(right);
            }
            else {
                window.draw(left);
            }
            timerRightKey = clock();
        }
        else {
            window.draw(up);
        }
    }
    else if (keyState == 3) {
        if ((clock() - std::max(timerLeftKey, timerRightKey)) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
            window.draw(wave);
            timerWaveKey = clock();
        }
        else {
            window.draw(up);
        }
    }

    // drawing tablet
    if (!isMouse) {
        window.draw(device);
    }

    // draw smoke
    bool isSmokeKeyPressed = input->isPressed(smokeKeyValue);
    if (isToggleSmoke) {
        previousSmokeKeyState = currentSmokeKeyState;
        currentSmokeKeyState = isSmokeKeyPressed;

        bool isSmokeKeyDown = currentSmokeKeyState && (currentSmokeKeyState != previousSmokeKeyState);

        if (isSmokeKeyDown) {
            drawSmoke = !drawSmoke;
        }
    }
    else {
        drawSmoke = isSmokeKeyPressed;
    }

    if (drawSmoke) {
        window.draw(smoke);
    }
}