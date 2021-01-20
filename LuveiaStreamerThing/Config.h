#pragma once

class Config
{
public:
	Config(const char* name);
	void ReadConfig();
	std::string GetCurrentWorkingDirectory();

	struct Main 
	{
		int Mode = 1;
	}main;

	struct Resolution
	{
		bool LetterBoxing = true;
		int Width = 1920;
		int Height = 1080;
		float HorizontalPosition = 0;
		float VerticalPosition = 0;
	}resolution;

	struct Decoration
	{
		bool LeftHanded = false;
		std::array<float, 4> Rgb = { 255, 255, 255, 255 };
		std::array<float, 2> OffsetX = { 0, 11 };
		std::array<float, 2> OffsetY = { 0, -65 };
		std::array<float, 2> Scalar = { 1, 1 };
	}decoration;

	//config stuff for the pen type draw
	struct Pen
	{
		bool Mouse = true;
		std::array<float, 3> Paw = { 0, 0, 0 };
		std::array<float, 3> PawEdge = { 0,0,0 };
		int Key1 = 90;
		int Key2 = 88;
		//add more keys here
	}pen;

private:
	std::filesystem::path Path;
	std::string FileName;

	void Load(const char* name);
	void Save();
};

inline std::unique_ptr<Config> config;