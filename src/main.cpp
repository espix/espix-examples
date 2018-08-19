#include <Arduino.h>

#include <SH1106.h>

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

unsigned long lastUpdate = 0;
unsigned long lastViewChange = 0;

SH1106Wire display(0x3c, D1, D2);
Application app(&display);
DrawingContext *context;

const uint8_t *animationFrames[] = {ANIMATION_XBM_01, ANIMATION_XBM_02, ANIMATION_XBM_03,
                                    ANIMATION_XBM_04, ANIMATION_XBM_05, ANIMATION_XBM_06,
                                    ANIMATION_XBM_07, ANIMATION_XBM_08, ANIMATION_XBM_09};
int viewIndex = 0;
int viewCount = 7;
View *views[] = {
    new ImageView(APPLE_XBM, APPLE_XBM_WIDTH, APPLE_XBM_HEIGHT),
    new TextView("Hello."),
    new TextView("Think Different.", FONT_SIZE_H2),
    new AnimationView(animationFrames, ANIMATION_XBM_WIDTH, ANIMATION_XBM_HEIGHT, 9, 6),
    new ClockView(),
    new WeatherTodayView(),
    new WeatherForecastView()};

void setView(int index, TransitionOptions options = TRANSITION_OPTIONS_NONE) {
  viewIndex = index;
  app.setRootView(views[viewIndex], options);
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

void setup() {
  Serial.begin(115200);
  Serial.println();

  app.begin();
  // Settings
  app.getScreen()->setOrientation(true);

  setView(0);
}

void loop() {
  if (millis() - lastViewChange > 3000) {
    nextView();
  }
  int timeBudget = app.update();
  if (timeBudget > 0) {
    delay(timeBudget);
  }
}
