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
};

class ConfigMgr {
    public:
        ConfigMgr();
        std::vector<std::vector<uint8_t>> getColors(std::string theme);
        std::map<std::string, std::vector<Special>> getSpecials();
};
#endif
