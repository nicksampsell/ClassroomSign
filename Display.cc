#include "led-matrix.h"
#include "graphics.h"
#include "ConfigMgr.h"

#include "Display.h"
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

Display::Display(Canvas *canvas)
{
    std::string shortDateFormat = "%D";
    std::string longDateFormat = "%A, %B %d, %Y";
    _canvas = canvas;

    std::ifstream f("/boot/sign/themes.json");
    json allColors = json::parse(f);

    std::string theme = "default";

    if(allColors.contains(theme))
    {
        for(int i=0; i<5;i++)
        {
            for(int j = 0; j<3;j++)
            {
                _colors[i][j] = allColors[theme][i][j].get<uint8_t>();
            }
        }
    }
}

void Display::DisplayShortDate()
{

    //0 = orange
    //1 = yellow/gold
    //2 = med blue
    //3 = dark blue
    //4 = white

    rgb_matrix::Font dateFont;

    dateFont.LoadFont("./fonts/cherry-10-r.bdf");

    time_t t = time({});
    char timeString[80];
    
    strftime(timeString, sizeof(timeString), "%D", std::localtime(&t));

    int dateFontWidth = dateFont.CharacterWidth((int)'M')*(strlen(timeString));

    Color dateColor(_colors[4][0], _colors[4][1], _colors[4][2]);

    rgb_matrix::DrawText(
            _canvas,
            dateFont,
            (_canvas->width() - dateFontWidth),
            (dateFont.height() / 2) + 6,
            dateColor,
            timeString
            );
}

void Display::DisplayLongDate()
{

    rgb_matrix::Font dateFont;
    rgb_matrix::Font todayIsFont;

    dateFont.LoadFont("./fonts/5x7.bdf");
    todayIsFont.LoadFont("./fonts/4x6.bdf");

    time_t t = time({});
    char timeString[80];
    
    strftime(timeString, sizeof(timeString), "%A, %B %e, %Y", std::localtime(&t));

    int dateFontWidth = dateFont.CharacterWidth((int)'M')*(strlen(timeString));
    int dateFontSmall = todayIsFont.CharacterWidth((int)'M')*(strlen(timeString));

    Color dateColor(_colors[0][0], _colors[0][1], _colors[0][2]);
    Color todayIsColor(_colors[2][0], _colors[2][1], _colors[2][2]);


    //Display the "Today is:" header to display in the top left corner
    //of the screen.
    rgb_matrix::DrawText(
            _canvas,
            todayIsFont,
            8,
            (todayIsFont.height() / 2) + 8,
            todayIsColor,
            "Today is:"
            );



    //Display the date in US Full date format with Day of the Week
    //E.g., Wednesday, March 8, 2023
    //If the width of the date is greater than width of the canvas,
    //display the date with the smaller font used for the "Today is:"
    //header, otherwise use the larger font.
    if(dateFontWidth > _canvas->width() )
    {
        rgb_matrix::DrawText(
                _canvas, 
                todayIsFont, 
                ((_canvas->width() - dateFontSmall) / 2),
                ((todayIsFont.height() / 2) + (todayIsFont.height() + 12)),
                dateColor,
                timeString
                );
    }
    else
    {
         rgb_matrix::DrawText(
                _canvas, 
                dateFont,
                ((_canvas->width() - dateFontWidth) / 2),
                ((dateFont.height() / 2) + (dateFont.height() + 12)),
                dateColor,
                timeString
                );

    }

}

void Display::DisplaySpecials(Special special)
{
    rgb_matrix::Font dayNameFont;
    rgb_matrix::Font specialNameFont;

    dayNameFont.LoadFont("./fonts/cherry-10-r.bdf");
    specialNameFont.LoadFont("./fonts/6x12.bdf");

    std::string dayName = "A Day";

    Color dayNameColor(_colors[1][0], _colors[1][1], _colors[1][2]);
    Color specialNameColor(_colors[4][0], _colors[4][1], _colors[4][2]);


    rgb_matrix::DrawText(
            _canvas,
            dayNameFont,
            (special.Size + 6),
            ((_canvas->height() - (dayNameFont.height() + 4))  - ((specialNameFont.height() / 2) - 4)),
            dayNameColor,
            "A Day");

    rgb_matrix::DrawText(
            _canvas,
            specialNameFont,
            (special.Size + 8),
            (_canvas->height() - (specialNameFont.height() / 2 - 1)),
            specialNameColor,
            special.Name.c_str()
            );
}
