#include "header.hpp"
#include "Drawing.h"
#include "Config.h"
#include "ImageLoader.h"
#include "Input.h"

Drawing::Drawing() {}

void Drawing::Init()
{
	isToggleSmoke = config->drawing.ToggleSmoke;
    smokeKeyValue = config->drawing.Smoke;

	offset_x = config->drawing.OffsetPen[0];
	offset_y = config->drawing.OffsetPen[1];

	//get images
	bg.setTexture(imageLoader->loadTexture("img/drawing/bg.png"));
	up.setTexture(imageLoader->loadTexture("img/drawing/up.png"));
	left.setTexture(imageLoader->loadTexture("img/drawing/left.png")); 
	right.setTexture(imageLoader->loadTexture("img/drawing/right.png")); 
	mouse.setTexture(imageLoader->loadTexture("img/drawing/mouse.png")); 
	smoke.setTexture(imageLoader->loadTexture("img/drawing/smoke.png"));
}

void Drawing::Draw(sf::RenderWindow& window)
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

    mouse.setPosition(mpos0 + dx + offset_x, mpos1 + dy + offset_y);

    // drawing mouse
    window.draw(mouse);

    //any key was pressed
    //bool leftKey = input->isPressed(90); //Z
    if (input->aKeyWasPressed)
    {
        window.draw(left);
    }
    else
    {
        window.draw(up);
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