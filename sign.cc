#include "led-matrix.h"
#include "Display.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <string>
#include <ctime>

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

    if(canvas == NULL)
        return 1;

    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    while(!interrupt_received)
    {
        Display display(canvas);
        display.DisplayShortDate();
        display.DisplayLongDate();
    }
    
    canvas->Clear();
    delete canvas;

    return 0;
}

/**
int main(int argc, char **argv) {
  RGBMatrix::Options options;
  options.hardware_mapping = "regular";
  options.rows = 64;
  options.cols = 64;
  options.chain_length = 2;
  options.parallel = 1;
  options.brightness = 60;
  options.led_rgb_sequence = "rbg";
  options.multiplexing = 0;
  options.show_refresh_rate = true;
  rgb_matrix::RuntimeOptions runtime_defaults;
  runtime_defaults.drop_privileges = 0;

  Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &options);
  if(canvas == NULL)
      return 1;

  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);


  time_t now = std::time(0);
  std::tm* ltm = std::localtime(&now);
  currentDate dd(ltm, "Hi");
  dd.setItemPos(100, 30);

  //  DrawOnCanvas(canvas);

  canvas->Clear();
  delete canvas;

}**/


