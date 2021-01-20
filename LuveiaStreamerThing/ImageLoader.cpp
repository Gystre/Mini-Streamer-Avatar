#include "header.hpp"
#include "ImageLoader.h"

sf::Texture& ImageLoader::loadTexture(std::string path)
{
    if (imgHolder.find(path) == imgHolder.end()) {
        while (!imgHolder[path].loadFromFile(path)) {
            std::cout << "[ERROR] Cannot find file " + path << std::endl;
            MessageBoxA(NULL, ("Cannot find file " + path).c_str(), "Error importing images", MB_ICONERROR | MB_OK);
            exit(-1);
        }
    }
    return imgHolder[path];
}