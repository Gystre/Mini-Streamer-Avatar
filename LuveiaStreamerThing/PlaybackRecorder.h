#pragma once

/*
Unused but ya know nice to archive somewhere
Basically just plays back whatever it hears through the mic

*/

 //Useful to hold onto the memory when converting it into a Chunk.
struct Samples {
    Samples(sf::Int16 const* ss, std::size_t count) 
    {
        samples.reserve(count);
        std::copy_n(ss, count, std::back_inserter(samples));
    }

    Samples() {}

    std::vector<sf::Int16> samples;
};

class PlaybackRecorder : private sf::SoundRecorder, private sf::SoundStream
{
public:
	~PlaybackRecorder();

    //init capturing and setup output
    void start();

    //stop both recording and playback
    void stop();

    bool isRunning() { return isRecording; }

protected: //overriden api calls
    bool onProcessSamples(sf::Int16 const* samples, std::size_t sampleCount) override;
    bool onStart() override;
    void onStop() override;
    bool onGetData(Chunk& chunk) override;
    void onSeek(sf::Time) override { /* Not supported, silently does nothing. */ }

private:
    std::atomic<bool> isRecording{ false };
    std::mutex mutex; // protects `data`
    std::condition_variable cv; // notify consumer thread of new samples
    std::queue<Samples> data; // samples come in from the recorder, and popped by the output stream
    Samples playingSamples; // used by the output stream.
};