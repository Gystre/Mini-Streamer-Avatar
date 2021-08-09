#include "header.hpp"
#include "Config.h"
#include "Input.h"
#include "ImageLoader.h"
#include "Osu.h"
#include "Drawing.h"
#include "KevinMode.h"
#include "Audio.h"

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
    std::cout << "[Main] read in the config" << std::endl;

    //do input
    input = std::make_unique<Input>();
    std::cout << "[Main] created input singleton" << std::endl;

    //do audio
    audio = std::make_unique<Audio>();
    audio->Init();
    std::cout << "[Main] created audio listener" << std::endl;

    //do image loader
    imageLoader = std::make_unique<ImageLoader>();
    std::cout << "[Main] created image loader" << std::endl;

    //do osu
    osu = std::make_unique<Osu>();
    osu->Init();
    std::cout << "[Main] created osu drawing style" << std::endl;

    //do drawing
    drawing = std::make_unique<Drawing>();
    drawing->Init();
    std::cout << "[Main] created drawing drawing style" << std::endl;

    //do kevinMode
    kevinMode = std::make_unique<KevinMode>();
    kevinMode->Init();
    std::cout << "[Main] created kevin mode drawing style" << std::endl;


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
                        //reload the stuff
                        config->ReadConfig();
                        osu->Init();
                        drawing->Init();
                        audio->Init();
                        std::cout << "[Main] reloaded config" << std::endl;
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

        std::array<float, 4> color = config->main.BackgroundColor;
        window.clear(sf::Color(color[0], color[1], color[2], color[3]));
        switch (mode) {
        case 1:
            drawing->Draw(window);
            break;
        case 2:
            kevinMode->Draw(window);
            break;
        case 3:
            osu->Draw(window);
            break;
        }

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