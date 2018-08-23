#include <Arduino.h>

#include <SH1106Wire.h>

#include <espix-core.h>
#include <espix-design.h>

#include "views/ClockView.h"
#include "views/WeatherForecastView.h"
#include "views/WeatherTodayView.h"

#include "assets/animation-01.xbm"
#include "assets/animation-02.xbm"
#include "assets/animation-03.xbm"
#include "assets/animation-04.xbm"
#include "assets/animation-05.xbm"
#include "assets/animation-06.xbm"
#include "assets/animation-07.xbm"
#include "assets/animation-08.xbm"
#include "assets/animation-09.xbm"
#include "assets/apple.xbm"

bool connecting = false;
unsigned long lastUpdate = 0;
unsigned long lastViewChange = 0;
int viewIndex = 0;
int viewCount = 8;

SH1106Wire *display = new SH1106Wire(0x3c, D1, D2);
Application *app = new Application(display);
DrawingContext *context;

const uint8_t *animationFrames[] = {ANIMATION_XBM_01, ANIMATION_XBM_02, ANIMATION_XBM_03,
                                    ANIMATION_XBM_04, ANIMATION_XBM_05, ANIMATION_XBM_06,
                                    ANIMATION_XBM_07, ANIMATION_XBM_08, ANIMATION_XBM_09};

TextView *ipView = new TextView(FONT_SIZE_H2);
View *views[] = {
    new ImageView(APPLE_XBM, APPLE_XBM_WIDTH, APPLE_XBM_HEIGHT),
    new TextView("Hello."),
    new TextView("Think different.", FONT_SIZE_H2),
    new ClockView(),
    new WeatherTodayView(),
    new WeatherForecastView(),
    new AnimationView(animationFrames, ANIMATION_XBM_WIDTH, ANIMATION_XBM_HEIGHT, 9, 6),
    ipView};

void setView(int index, TransitionOptions options = TRANSITION_OPTIONS_NONE) {
  viewIndex = index;
  app->setRootView(views[viewIndex], options);
  lastViewChange = millis();
}

void nextView() {
  viewIndex++;
  if (viewIndex >= viewCount) {
    viewIndex = 0;
  }
  setView(viewIndex, TransitionOptions(TRANSITION_TO_LEFT));
}

void previousView() {
  viewIndex--;
  if (viewIndex < 0) {
    viewIndex = viewCount - 1;
  }
  setView(viewIndex, TransitionOptions(TRANSITION_TO_RIGHT));
}

void handleKeyPress(KeyCode keyCode) {
  if (connecting) {
    return;
  }

  switch (keyCode) {
  case KEY_LEFT_ARROW:
    previousView();
    break;
  case KEY_RIGHT_ARROW:
    nextView();
    break;
  }
}

void onConnected() {
  connecting = false;
  ipView->setText(app->getNetwork()->getLocalIP());
  setView(0, TransitionOptions(TRANSITION_TO_LEFT));
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  app->begin();
  // Settings
  app->getScreen()->setOrientation(false);
  app->getKeyboard()->registerKey(KEY_LEFT_ARROW, D5);
  app->getKeyboard()->registerKey(KEY_RIGHT_ARROW, D6);
  app->onKeyPress(handleKeyPress);

  // WiFi
  connecting = true;
  WiFiConnectionSetting settings[] = {
      WiFiConnectionSetting("Henry's iPhone 6", "13913954971"),
      WiFiConnectionSetting("Henry's Living Room 2.4GHz", "13913954971")};
  app->getNetwork()->connect(settings, 2, true, onConnected);
}

void loop() {
  int timeBudget = app->update();
  if (timeBudget > 0) {
    // if (!connecting) {
    //   if (millis() - lastViewChange > 10 * 1000) {
    //     nextView();
    //   }
    // }
    delay(timeBudget);
  }
}
