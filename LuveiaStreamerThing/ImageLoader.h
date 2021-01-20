#pragma once

class ImageLoader
{
private:
	std::map<std::string, sf::Texture> imgHolder;

public:
	sf::Texture& loadTexture(std::string path);
};

inline std::unique_ptr<ImageLoader> imageLoader;