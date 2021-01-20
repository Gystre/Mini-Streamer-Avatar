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

    std::cout << "config path: " << Path << std::endl;

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
    config->main.Mode = j["Main"]["Mode"];

    config->resolution.LetterBoxing = j["Resolution"]["Letter Boxing"];
    config->resolution.Width = j["Resolution"]["Width"];
    config->resolution.Height = j["Resolution"]["Height"];
    config->resolution.HorizontalPosition = j["Resolution"]["Horizontal Position"];
    config->resolution.VerticalPosition = j["Resolution"]["Vertical Position"];

    config->decoration.LeftHanded = j["Decoration"]["Left Handed"];
    config->decoration.Rgb = j["Decoration"]["Rgb"];
    config->decoration.OffsetMouse = j["Decoration"]["Offset Mouse"];
    config->decoration.OffsetPen = j["Decoration"]["Offset Pen"];
    config->decoration.MouseScalar = j["Decoration"]["Mouse Scalar"];
    config->decoration.PenScalar = j["Decoration"]["Pen Scalar"];

    config->osu.Mouse = j["Osu"]["Mouse"];
    config->osu.ToggleSmoke = j["Osu"]["Toggle Smoke"];
    config->osu.PawColor = j["Osu"]["Paw Color"];
    config->osu.PawEdgeColor = j["Osu"]["Paw Edge Color"];
    config->osu.Key1 = j["Osu"]["Key 1"];
    config->osu.Key2 = j["Osu"]["Key 2"];
    config->osu.Smoke = j["Osu"]["Smoke Key"];
    config->osu.Wave = j["Osu"]["Wave Key"];

    config->mousePaw.Comment = j["Mouse Paw"]["Comment"];
    config->mousePaw.PawStartingPoint = j["Mouse Paw"]["Paw Starting Point"];
    config->mousePaw.PawEndingPoint = j["Mouse Paw"]["Paw Ending Point"];
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
        j["Main"]["Mode"] = config->main.Mode;

        j["Resolution"]["Letter Boxing"] = config->resolution.LetterBoxing;
        j["Resolution"]["Width"] = config->resolution.Width;
        j["Resolution"]["Height"] = config->resolution.Height;
        j["Resolution"]["Horizontal Position"] = config->resolution.HorizontalPosition;
        j["Resolution"]["Vertical Position"] = config->resolution.VerticalPosition;

        j["Decoration"]["Left Handed"] = config->decoration.LeftHanded;
        j["Decoration"]["Rgb"] = config->decoration.Rgb;
        j["Decoration"]["Offset Mouse"] = config->decoration.OffsetMouse;
        j["Decoration"]["Offset Pen"] = config->decoration.OffsetPen;
        j["Decoration"]["Mouse Scalar"] = config->decoration.MouseScalar;
        j["Decoration"]["Pen Scalar"] = config->decoration.PenScalar;
        /*j["Decoration"]["Scalar"] = config->decoration.Scalar;*/

        j["Osu"]["Mouse"] = config->osu.Mouse;
        j["Osu"]["Toggle Smoke"] = config->osu.ToggleSmoke;
        j["Osu"]["Paw Color"] = config->osu.PawColor;
        j["Osu"]["Paw Edge Color"] = config->osu.PawEdgeColor;
        j["Osu"]["Key 1"] = config->osu.Key1;
        j["Osu"]["Key 2"] = config->osu.Key2;
        j["Osu"]["Smoke Key"] = config->osu.Smoke;
        j["Osu"]["Wave Key"] = config->osu.Wave;

        j["Mouse Paw"]["Comment"] = config->mousePaw.Comment;
        j["Mouse Paw"]["Paw Starting Point"] = config->mousePaw.PawStartingPoint;
        j["Mouse Paw"]["Paw Ending Point"] = config->mousePaw.PawEndingPoint;


        out << std::setw(2) << j;
    }
}