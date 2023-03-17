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
            Special s;
            s.Name = el["name"].get<std::string>();
            s.Icon = el["icon"].get<std::string>();
            s.Time = el["time"].get<std::string>();
            s.Size = el["size"].get<int>();
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

Config ConfigMgr::getConfig()
{
    std::ifstream f("/boot/sign/settings.json");
    json allSettings = json::parse(f);

    Config c;
    c.iconPath = allSettings["iconPath"].get<std::string>();
    c.defaultTheme = allSettings["defaultTheme"].get<std::string>();
    c.useSpecialThemes = allSettings["useSpecialThemes"].get<bool>();
    c.showLongDate = allSettings["features"]["showLongDate"].get<bool>();
    c.showShortDate = allSettings["features"]["showShortDate"].get<bool>();
    c.showSpecials = allSettings["features"]["showSpecials"].get<bool>();
    c.showBirthdays = allSettings["features"]["showBirthdays"].get<bool>();
    c.showWeather = allSettings["features"]["showWeather"].get<bool>();
    c.showTime = allSettings["features"]["showTime"].get<bool>();
    c.rotateInfo = allSettings["features"]["rotateInfo"].get<bool>();
    c.useAnimation = allSettings["features"]["useAnimation"].get<bool>();
    c.maxSpecialsPerDay = allSettings["features"]["maxSpecialsPerDay"].get<int>();
   
    
    std::vector<ThemeCriteria> tcVector;
    for(json::iterator it = allSettings["themeCriteria"].begin(); it != allSettings["themeCriteria"].end(); ++it)
    {
        ThemeCriteria tcItem;
        tcItem.Theme = it.key();
        
        std::map<std::string, std::string> criteria;

        for(json::iterator it2 = it.value().begin(); it2 != it.value().end(); ++it2)
        {
            criteria.insert(std::pair<std::string, std::string>(it2.key(), it2.value()));
        }
        tcItem.Criteria = criteria;
        tcVector.push_back(tcItem);
    }

    c.themeCriteria = tcVector;
    c.specialsCycle = { 
        allSettings["cycle"]["type"].get<std::string>(),
        allSettings["cycle"]["daysInCycle"].get<int>()
    };

    return c;
}


