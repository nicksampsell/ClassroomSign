#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <string>
#include <ctime>

#include "led-matrix.h"
#include "ConfigMgr.h"
#include "Display.h"

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo)
{
    interrupt_received = true;
}

int main(int argc, char **argv) {

    RGBMatrix::Options defaults;
    rgb_matrix::RuntimeOptions runtime;
    defaults.hardware_mapping = "regular";
    defaults.rows = 64;
    defaults.cols = 64;
    defaults.chain_length = 2;
    defaults.parallel = 1;
    defaults.show_refresh_rate = false;
    defaults.led_rgb_sequence = "rbg";
    runtime.drop_privileges = 0;
    defaults.brightness = 60;
    Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults, &runtime);
    std::vector<std::vector<uint8_t>> colors;
    std::map<std::string, std::vector<Special>> specials;

    if(canvas == NULL)
        return 1;

    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    while(!interrupt_received)
    {
        ConfigMgr cfg;
        colors = cfg.getColors("default");
        specials = cfg.getSpecials();


        Display display(canvas);
        display.DisplayShortDate();
        display.DisplayLongDate();
        display.DisplaySpecials(specials["a"][0]);
    }
    
    canvas->Clear();
    delete canvas;

    return 0;
}

