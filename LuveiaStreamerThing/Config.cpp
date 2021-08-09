#include "header.hpp"
#include "Config.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

Config::Config(const char* name)
{
    //set the path
    Path = GetCurrentWorkingDirectory();
	Path /= name;

    FileName = name;

    std::cout << "[Config] config path: " << Path << std::endl;
}

std::string Config::GetCurrentWorkingDirectory()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");

    //remove the exe part
    return std::string(buffer).substr(0, pos);
}

//need to do here because value init in structs haposu after constructor
void Config::ReadConfig()
{
    std::ifstream in{ Path };
    if (!in.good())
    {
        //create new config
        Save();
    }
    else 
    {
        //load the current one
        Load(FileName.c_str());
    }
}

void Config::Load(const char* name)
{
    json j;

    //make sure the file exists
    if (std::ifstream in{ Path }; in.good()) {
        j = json::parse(in, nullptr, false);
        if (j.is_discarded())
            return;
    }
    else {
        return;
    }

    //read in the stuff
    config->main.Mode = j["Mode"];
    config->main.BackgroundColor = j["Background Color"];

    config->osu.Mouse = j["Modes"]["Osu"]["Mouse"];
    config->osu.ToggleSmoke = j["Modes"]["Osu"]["Toggle Smoke"];
    config->osu.PawColor = j["Modes"]["Osu"]["Paw Color"];
    config->osu.PawEdgeColor = j["Modes"]["Osu"]["Paw Edge Color"];
    config->osu.Key1 = j["Modes"]["Osu"]["Key 1"];
    config->osu.Key2 = j["Modes"]["Osu"]["Key 2"];
    config->osu.Smoke = j["Modes"]["Osu"]["Smoke Key"];
    config->osu.Wave = j["Modes"]["Osu"]["Wave Key"];
    config->osu.OffsetMouse = j["Modes"]["Osu"]["Offset Mouse"];
    config->osu.OffsetPen = j["Modes"]["Osu"]["Offset Pen"];
    config->osu.MouseScalar = j["Modes"]["Osu"]["Mouse Scalar"];
    config->osu.PenScalar = j["Modes"]["Osu"]["Pen Scalar"];
    config->osu.Comment = j["Modes"]["Osu"]["Comment"];
    config->osu.PawStartingPoint = j["Modes"]["Osu"]["Paw Starting Point"];
    config->osu.PawEndingPoint= j["Modes"]["Osu"]["Paw Ending Point"];
    //resolution stuff
    config->osu.LetterBoxing = j["Modes"]["Osu"]["Letter Boxing"];
    config->osu.OsuWidth = j["Modes"]["Osu"]["Osu Width"];
    config->osu.OsuHeight = j["Modes"]["Osu"]["Osu Height"];
    config->osu.HorizontalPosition = j["Modes"]["Osu"]["Horizontal Position"];
    config->osu.VerticalPosition = j["Modes"]["Osu"]["Vertical Position"];
    config->osu.LeftHanded = j["Modes"]["Osu"]["Left Handed"];

    config->drawing.ToggleSmoke = j["Modes"]["Drawing"]["Toggle Smoke"];
    config->drawing.OffsetPen = j["Modes"]["Drawing"]["Pen Offset"];
    config->drawing.Smoke = j["Modes"]["Drawing"]["Smoke Key"];
    config->drawing.LeftHandPosition = j["Modes"]["Drawing"]["Left Hand Position"];

    config->mouth.MinThreshold = j["Mouth"]["Min Threshold"];
}

void Config::Save()
{
    std::filesystem::path file = Path;
    file.replace_extension(".json");
    std::ofstream out{ file.c_str() };

    if (out.good())
    {
        json j;

        //save the stuff
        j["Mode"] = config->main.Mode;
        j["Background Color"] = config->main.BackgroundColor;

        j["Modes"]["Osu"]["Mouse"] = config->osu.Mouse;
        j["Modes"]["Osu"]["Toggle Smoke"] = config->osu.ToggleSmoke;
        j["Modes"]["Osu"]["Paw Color"] = config->osu.PawColor;
        j["Modes"]["Osu"]["Paw Edge Color"] = config->osu.PawEdgeColor;
        j["Modes"]["Osu"]["Key 1"] = config->osu.Key1;
        j["Modes"]["Osu"]["Key 2"] = config->osu.Key2;
        j["Modes"]["Osu"]["Smoke Key"] = config->osu.Smoke;
        j["Modes"]["Osu"]["Wave Key"] = config->osu.Wave;
        j["Modes"]["Osu"]["Offset Mouse"] = config->osu.OffsetMouse;
        j["Modes"]["Osu"]["Offset Pen"] = config->osu.OffsetPen;
        j["Modes"]["Osu"]["Mouse Scalar"] = config->osu.MouseScalar;
        j["Modes"]["Osu"]["Pen Scalar"] = config->osu.PenScalar;
        j["Modes"]["Osu"]["Comment"] = config->osu.Comment;
        j["Modes"]["Osu"]["Paw Starting Point"] = config->osu.PawStartingPoint;
        j["Modes"]["Osu"]["Paw Ending Point"] = config->osu.PawEndingPoint;
        //resolution stuff
        j["Modes"]["Osu"]["Letter Boxing"] = config->osu.LetterBoxing;
        j["Modes"]["Osu"]["Osu Width"] = config->osu.OsuWidth;
        j["Modes"]["Osu"]["Osu Height"] = config->osu.OsuHeight;
        j["Modes"]["Osu"]["Horizontal Position"] = config->osu.HorizontalPosition;
        j["Modes"]["Osu"]["Vertical Position"] = config->osu.VerticalPosition;
        j["Modes"]["Osu"]["Left Handed"] = config->osu.LeftHanded;

        j["Modes"]["Drawing"]["Toggle Smoke"] = config->drawing.ToggleSmoke;
        j["Modes"]["Drawing"]["Pen Offset"] = config->drawing.OffsetPen;
        j["Modes"]["Drawing"]["Smoke Key"] = config->drawing.Smoke;
        j["Modes"]["Drawing"]["Left Hand Position"] = config->drawing.LeftHandPosition;

        j["Mouth"]["Min Threshold"] = config->mouth.MinThreshold;
        out << std::setw(2) << j;
    }
}