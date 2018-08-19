#pragma once

#include <Arduino.h>

#include <espix-core.h>

class ClockView : public View {
public:
  ClockView() {
    _startTime = millis();
  }

  bool shouldUpdate(ViewUpdateOptions options) {
    if (options.forceUpdate) {
      return true;
    }
    if (options.now - getLastUpdate() > 1000) {
      return true;
    }
    return false;
  }

  void update(ViewUpdateOptions options) {
    _text = _formatTime(millis() - _startTime);
  }

  void render(DrawingContext *context) {
    context->setFontSize(FONT_SIZE_H1);
    context->setTextAlign(TEXT_ALIGN_CENTER_BOTH);
    context->drawString(_text);
  }

  String _formatTime(unsigned long duration) {
    unsigned long durationInSeconds = duration / 1000;
    unsigned short secs = durationInSeconds % 60;
    unsigned short mins = durationInSeconds / 60;
    String result = mins < 10 ? "13:0" : "13:";
    result += String(mins);
    result += secs < 10 ? ":0" : ":";
    result += String(secs);
    return result;
  }

private:
  unsigned long _startTime;
  String _text = "--:--:--";
};
