#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <Windows.h>
#include <iostream>
#include <memory>
#include <filesystem>
#include <array>
#include <fstream>
#include <sstream>
#include <map>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <assert.h>

#define WINDOW_WIDTH 612
#define WINDOW_HEIGHT 383
#define MAX_FRAMERATE 60