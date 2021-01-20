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

//need to do here because value init in structs happen after constructor
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
    config->decoration.OffsetX = j["Decoration"]["Offset X"];
    config->decoration.OffsetY = j["Decoration"]["Offset Y"];
    config->decoration.Scalar = j["Decoration"]["Scalar"];

    config->pen.Mouse = j["Pen"]["Mouse"];
    config->pen.Paw = j["Pen"]["Paw"];
    config->pen.PawEdge = j["Pen"]["Paw Edge"];
    config->pen.Key1 = j["Pen"]["Key 1"];
    config->pen.Key2 = j["Pen"]["Key 2"];
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
        j["Decoration"]["Offset X"] = config->decoration.OffsetX;
        j["Decoration"]["Offset Y"] = config->decoration.OffsetY;
        j["Decoration"]["Scalar"] = config->decoration.Scalar;

        j["Pen"]["Mouse"] = config->pen.Mouse;
        j["Pen"]["Paw"] = config->pen.Paw;
        j["Pen"]["Paw Edge"] = config->pen.PawEdge;
        j["Pen"]["Key 1"] = config->pen.Key1;
        j["Pen"]["Key 2"] = config->pen.Key2;

        out << std::setw(2) << j;
    }
}