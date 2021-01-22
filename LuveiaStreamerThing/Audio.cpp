#include "header.hpp"
#include "Audio.h"
#include "Config.h"

Audio::Audio()
{
    isTalking = false;
    minThreshold = config->mouth.MinThreshold;
    negMinThreshold = minThreshold * -1;
}

Audio::~Audio()
{
    this->stop();
}

bool Audio::onStart()
{
    if (!sf::SoundBufferRecorder::isAvailable())
    {
        MessageBoxA(NULL, "Sound buffer isn't available which means no audio will be recorded :P", "Sound buffer recorder not availabe xd roflmao", MB_OK | MB_ICONERROR);
        return false;
    }

    //// get the available sound input device names
    //std::vector<std::string> availableDevices = sf::SoundRecorder::getAvailableDevices();

    //// choose a device
    //std::string inputDevice = availableDevices[0];

    std::string inputDevice = sf::SoundRecorder::getDefaultDevice();

    //see if there's even a mic connected
    if (inputDevice.empty())
    {
        MessageBoxA(NULL, "No mic detected, not going to be ", "No mic detected xd roflmao", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }
    //set recorder to default device
    else if (!this->setDevice(inputDevice))
    {
        MessageBoxA(NULL, ("Can't set device " + inputDevice).c_str(), "Device selection failed", MB_OK | MB_ICONERROR);

        return false;
    }

    std::cout << "[Audio] Set recording device as " << inputDevice << std::endl;

    return true;
}

//gets called after done sampling for the set time interval
bool Audio::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
    int met = 0;
    for (int i = 0; i < sampleCount; i++)
    {
        if (samples[i] > minThreshold || samples[i] < negMinThreshold)
        {
            met++;
        }
    }

    //std::cout << sampleCount << " met: " << met << std::endl;
    if (met >= minThreshold)
    {
        isTalking = true;
    }
    else
    {
        isTalking = false;
    }

    return true;
}