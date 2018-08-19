#pragma once

#include <Arduino.h>

#include <espix-core.h>

#include "../assets/icon-fonts.h"

class WeatherTodayView : public View {
public:
  void render(DrawingContext *context) {
    _drawContent(context);
  }

private:
  void _drawContent(DrawingContext *context) {
    context->setFontSize(FONT_SIZE_NORMAL);
    context->setTextAlign(TEXT_ALIGN_LEFT);
    context->drawString("Mostly Cloudy", 60, 14);

    String temp = "32°C";
    context->setFontSize(FONT_SIZE_H1);
    context->drawString(temp, 60, 24);

    context->setFont(Meteocons_Plain_42);
    String weatherIcon = "Q";
    int weatherIconWidth = context->getStringWidth(weatherIcon);
    context->drawString(weatherIcon, 32 - weatherIconWidth / 2, 9);
  }
};
