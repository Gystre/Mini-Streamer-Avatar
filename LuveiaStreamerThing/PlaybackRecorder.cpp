#include "header.hpp"
#include "PlaybackRecorder.h"

PlaybackRecorder::~PlaybackRecorder()
{
    this->stop();
}

// Initialise capturing input & setup output
void PlaybackRecorder::start()
{
    if (!sf::SoundBufferRecorder::isAvailable())
    {
        MessageBoxA(NULL, "Sound buffer isn't available which means no audio will be recorded :P", "Sound buffer recorder not availabe xd roflmao", MB_OK | MB_ICONERROR);
        return;
    }

    sf::SoundRecorder::start();

    sf::SoundStream::initialize(sf::SoundRecorder::getChannelCount(), sf::SoundRecorder::getSampleRate());
    sf::SoundStream::play();
    //sf::SoundStream::setVolume(0);
}

// Stop both recording & playback
void PlaybackRecorder::stop()
{
    sf::SoundRecorder::stop();
    sf::SoundStream::stop();
}

bool PlaybackRecorder::onProcessSamples(sf::Int16 const* samples, std::size_t sampleCount)
{
    for (int i = 0; i < sampleCount; i++)
    {
        std::cout << samples[i] << std::endl;
    }

    {
        std::lock_guard<std::mutex> lock(mutex);
        data.emplace(samples, sampleCount);
    }
    cv.notify_one();
    return true; // continue capture
}

bool PlaybackRecorder::onStart()
{
    isRecording = true;
    return true;
}

void PlaybackRecorder::onStop()
{
    isRecording = false;
    cv.notify_one();
}

bool PlaybackRecorder::onGetData(Chunk& chunk)
{
    // Wait until either:
    //  a) the recording was stopped
    //  b) new data is available
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [this] { return !isRecording || !data.empty(); });

    // Lock was acquired, examine which case we're into:
    if (!isRecording) return false; // stop playing.
    else {
        assert(!data.empty());

        playingSamples.samples = std::move(data.front().samples);
        data.pop();
        chunk.sampleCount = playingSamples.samples.size();
        chunk.samples = playingSamples.samples.data();

        return true;
    }
}