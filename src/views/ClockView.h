#pragma once

#include <Arduino.h>

#include <espix-core.h>

class ClockView : public View {
public:
  ClockView() {
  }

  bool shouldUpdate() {
    if (isDirty()) {
      return true;
    }
    if (millis() - getLastUpdate() > 1000) {
      return true;
    }
    return false;
  }

  void update() {
    _text = TimeClient::getInstance()->getFormattedTime();
  }

  void render(DrawingContext *context) {
    context->setFontSize(FONT_SIZE_H1);
    context->setTextAlign(TEXT_ALIGN_CENTER_BOTH);
    context->drawString(_text);
  }

private:
  String _text = "-- : -- : --";
};
