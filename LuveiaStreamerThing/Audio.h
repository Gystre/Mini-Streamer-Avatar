#pragma once

class Audio : public sf::SoundRecorder
{
public:
	Audio();
	~Audio();

	//public value that is true whenever mic activity is detected
	bool isTalking;

	//from config
	int minThreshold;
	int negMinThreshold;

	bool onStart();
private:
	bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount);
};

inline std::unique_ptr<Audio> audio;