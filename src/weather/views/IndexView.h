#pragma once

#include <Arduino.h>

#include <espix-core.h>

#include "../assets/meteocons-font.h"

#include "./WeatherForecastView.h"

class WeatherIndexView : public View {
public:
  WeatherIndexView() : View() {
    setSelectable(true);
  }

  void render(CanvasContext *context) {
    _drawContent(context);
  }

  void didSelect() {
    Application.pushView(getForecastView());
  }

private:
  WeatherForecastView *getForecastView() {
    if (_forecastView == NULL) {
      _forecastView = new WeatherForecastView();
    }
    return _forecastView;
  }

  void _drawContent(CanvasContext *context) {
    context->setFontSize(FontSize::NORMAL);
    context->setTextAlign(TextAlign::LEFT);
    context->drawString("Mostly Cloudy", 56, 12);

    String temp = "32°C";
    context->setFontSize(FontSize::H1);
    context->drawString(temp, 56, 22);

    context->setFont(Meteocons_Plain_42);
    String weatherIcon = "Q";
    int weatherIconWidth = context->getStringWidth(weatherIcon);
    context->drawString(weatherIcon, 28 - weatherIconWidth / 2, 7);
  }

  WeatherForecastView *_forecastView = NULL;
};
