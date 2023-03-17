#ifndef CONFIGMGR_H
#define CONFIGMGR_H

#include "led-matrix.h"
#include "graphics.h"

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

struct Special {
    std::string Name;
    std::string Time;
    std::string Icon;
    int Size;
};

struct ThemeCriteria {
    std::string Theme;
    std::map<std::string, std::string> Criteria;
};

struct Cycle
{
    std::string Type;
    int DaysInCycle;
};

struct Config {
    std::string iconPath;
    std::string defaultTheme;
    bool useSpecialThemes;
    std::vector<ThemeCriteria> themeCriteria;
    bool showLongDate;
    bool showShortDate;
    bool showSpecials;
    bool showBirthdays;
    bool showWeather;
    bool showTime;
    bool rotateInfo;
    bool useAnimation;
    int maxSpecialsPerDay;
    Cycle specialsCycle;
};

class ConfigMgr {
    public:
        ConfigMgr();
        std::vector<std::vector<uint8_t>> getColors(std::string theme);
        std::map<std::string, std::vector<Special>> getSpecials();
        Config getConfig();
};
#endif
