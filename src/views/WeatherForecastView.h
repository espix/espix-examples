#pragma once

#include <Arduino.h>

#include <espix-core.h>

#include "../assets/icon-fonts.h"

String DAYS[3] = {"THU", "FRI", "SAT"};
String WEATHERS[3] = {"Q", "H", "B"};
String TEMPERATURES[3] = {"26|33", "26|31", "26|32"};

class WeatherForecastView : public View {
public:
  void render(DrawingContext *context) {
    _drawContent(context);
  }

private:
  void _drawContent(DrawingContext *context) {
    _drawForecastDetails(context, 0, 0);
    _drawForecastDetails(context, 44, 1);
    _drawForecastDetails(context, 88, 2);
  }

  void _drawForecastDetails(DrawingContext *context, int x, int dayIndex) {
    context->setTextAlign(TEXT_ALIGN_CENTER);
    context->setFontSize(FONT_SIZE_NORMAL);
    String day = DAYS[dayIndex];
    context->drawString(day, 20 + x, 4);

    context->setFont(Meteocons_Plain_21);
    context->drawString(WEATHERS[dayIndex], x + 20, 16);

    context->setFontSize(FONT_SIZE_NORMAL);
    context->drawString(TEMPERATURES[dayIndex], x + 20, 40);
  }
};
