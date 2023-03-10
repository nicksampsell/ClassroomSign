#include "led-matrix.h"
#include <string>
#include <fstream>
#include <vector>

#include "include/nlohmann/json.hpp"
using json = nlohmann::json;
using namespace std;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

class Display {
    public:
        Display(Canvas *canvas);
        void DisplayShortDate();
        void DisplayLongDate();
        void DisplaySpecials();
    private:
        std::string shortDateFormat;
        std::string longDateFormat;
        Canvas* _canvas;
        uint8_t _colors[5][3];
        
};
