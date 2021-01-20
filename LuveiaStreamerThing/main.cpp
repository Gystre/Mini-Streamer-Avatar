#include "header.hpp"
#include "Config.h"
#include "Input.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
    AllocConsole();
    freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    SetConsoleTitleA("KYLE TOO COOL YEAHHHH");
#endif

    sf::RenderWindow window;

    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "KYLE GODLY HAHAHAHAHAAH", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(MAX_FRAMERATE);

    //do config
    config = std::make_unique<Config>("config.json");
    config->ReadConfig();

    std::cout << "read in the config" << std::endl;

    //do input
    input = std::make_unique<Input>();

    std::cout << "created input singleton" << std::endl;

    //render loop
    bool isReload = false;
    bool isShowInputDebug = false;

    while (window.isOpen()) 
    {
        sf::Event windowEvent;
        while (window.pollEvent(windowEvent))
        {
            //poll events
            switch (windowEvent.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                // get reload config prompt
                if (windowEvent.key.code == sf::Keyboard::R && windowEvent.key.control) {
                    if (!isReload) {
                        //reload config
                        config->ReadConfig();
                    }
                    isReload = true;
                    break;
                }

                // toggle joystick debug panel
                if (windowEvent.key.code == sf::Keyboard::D && windowEvent.key.control) {
                    isShowInputDebug = !isShowInputDebug;
                    break;
                }

            default:
                isReload = false;
            }
        }

        //draw the cat thing
        //and clear to config color
        int mode = config->main.Mode;

        std::array<float, 4> color = config->decoration.Rgb;
        window.clear(sf::Color(color[0], color[1], color[2], color[3]));
        //switch (mode) {
        //case 1:
        //    osu::draw();
        //    break;
        //case 2:
        //    taiko::draw();
        //    break;
        //case 3:
        //    ctb::draw();
        //    break;
        //case 4:
        //    mania::draw();
        //    break;
        //case 5:
        //    custom::draw();
        //}

        //show input panel
        if (isShowInputDebug) {
            input->drawDebugPanel(window);
        }

        window.display();
    }

#ifdef _DEBUG
    fclose((FILE*)stdin);
    fclose((FILE*)stdout);
    FreeConsole();
#endif

	return 0;
}