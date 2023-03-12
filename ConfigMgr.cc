#include "ConfigMgr.h"

#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <locale>
#include <ctime>
#include <iterator>

#include "include/nlohmann/json.hpp"
using json = nlohmann::json;

using namespace rgb_matrix;

ConfigMgr::ConfigMgr()
{

}

std::map<std::string, std::vector<Special>> ConfigMgr::getSpecials()
{
    std::ifstream f("/boot/sign/specials.json");
    json allSpecials = json::parse(f);
    std::map<std::string, std::vector<Special>> specials;


    for(json::iterator it = allSpecials.begin(); it != allSpecials.end(); ++it)
    {
        std::vector<Special> oneDay;

        for(auto& el : it.value())
        {
            std::cout << el << std::endl;
            Special s;
            s.Name = el["name"].get<std::string>();
            s.Icon = el["icon"].get<std::string>();
            s.Time = el["time"].get<std::string>();
            oneDay.push_back(s);
        
        }

        specials.insert(std::pair<std::string, std::vector<Special>>(it.key(), oneDay)); 
        
    }

    return specials;
}

std::vector<std::vector<uint8_t>> ConfigMgr::getColors(std::string theme)
{
    std::ifstream f("/boot/sign/themes.json");
    json allColors = json::parse(f);

    std::vector<std::vector<uint8_t>> colors;

    if(theme == "")
    {
        theme = "default";
    }

    if(allColors.contains(theme))
    {
        for(int i=0; i<5;i++)
        {
            std::vector<uint8_t> colorRow;
            for(int j = 0; j<3;j++)
            {

                colorRow.push_back(allColors[theme][i][j].get<uint8_t>());
            }
            colors.push_back(colorRow);
        }
    }
    
    return colors;
}
