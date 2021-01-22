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
		int OsuWidth = 1920;
		int OsuHeight = 1080;
		float HorizontalPosition = 0;
		float VerticalPosition = 0;
	}resolution;

	struct Decoration
	{
		bool LeftHanded = false;
		std::array<float, 4> BackgroundColor = { 255, 255, 255, 255 };

	}decoration;

	//config stuff for the pen type draw
	struct Osu
	{
		bool Mouse = true;
		bool ToggleSmoke = false;
		std::array<float, 4> PawColor = { 255, 255, 255, 255 };
		std::array<float, 4> PawEdgeColor = { 0,0,0, 255 };
		int Key1 = 90;
		int Key2 = 88;
		int Smoke = 67;
		int Wave = 86;

		std::array<float, 2> OffsetMouse = { 0, 0 };
		float MouseScalar = 1;
		std::array<float, 2> OffsetPen = { 11, -65 };
		float PenScalar = 1;

		std::string Comment = "coordinates start in the top left of the window";
		std::array<float, 2> PawStartingPoint = { 211, 159 };
		std::array<float, 2> PawEndingPoint = { 258, 228 };
	}osu;

	struct Drawing
	{
		std::array<float, 2> OffsetPen = { 12, 0 };
		int Smoke = 67;
		bool ToggleSmoke = false;

		std::string Comment = "coordinates start in the top left of the window";
		std::array<float, 2> LeftHandPosition = { 40, 3 };
	}drawing;

	struct Mouth
	{
		int MinThreshold = 40;
	}mouth;

private:
	std::filesystem::path Path;
	std::string FileName;

	void Load(const char* name);
	void Save();
};

inline std::unique_ptr<Config> config;