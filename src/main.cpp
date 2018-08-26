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

#define OLED_SDA D1
#define OLED_CLK D2
#define KY04_CLK D5
#define KY04_DT D6
#define KY04_SW D7

bool connecting = false;
unsigned long lastUpdate = 0;
unsigned long lastViewChange = 0;
int viewIndex = 0;
int viewCount = 8;

SH1106Wire display(0x3c, OLED_SDA, OLED_CLK);
Application app(&display);

const uint8_t *animationFrames[] = {ANIMATION_XBM_01, ANIMATION_XBM_02, ANIMATION_XBM_03,
                                    ANIMATION_XBM_04, ANIMATION_XBM_05, ANIMATION_XBM_06,
                                    ANIMATION_XBM_07, ANIMATION_XBM_08, ANIMATION_XBM_09};

TextView *ipView = new TextView(FONT_SIZE_H2);
View *views[] = {
    new ClockView(),
    new WeatherTodayView(),
    new WeatherForecastView(),
    new ImageView(APPLE_XBM, APPLE_XBM_WIDTH, APPLE_XBM_HEIGHT),
    new TextView("Hello."),
    new TextView("Think different.", FONT_SIZE_H2),
    new AnimationView(animationFrames, ANIMATION_XBM_WIDTH, ANIMATION_XBM_HEIGHT, 9, 6),
    ipView};

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

void handleKeyPress(KeyCode keyCode) {
  if (connecting) {
    return;
  }

  switch (keyCode) {
  case KEY_LEFT_ARROW:
    previousView();
    break;
  case KEY_ENTER:
  case KEY_RIGHT_ARROW:
    nextView();
    break;
  }
}

void onConnected() {
  connecting = false;
  app.enableOTA();
  ipView->setText(WiFiNetwork.getLocalIP());
  setView(0, TransitionOptions(TRANSITION_TO_BOTTOM));
}

void connect() {
  WiFiConnectionSetting settings[] = {
      WiFiConnectionSetting("Henry's iPhone 6", "13913954971"),
      WiFiConnectionSetting("Henry's Living Room 2.4GHz", "13913954971")};
  WiFiNetwork.connect(settings, 2, true, onConnected);
  connecting = true;
}

void setupDevices() {
  Keyboard.registerKey(KEY_ENTER, KY04_SW);
  Keyboard.begin();
}

void setupApp() {
  app.begin();
  // Settings
  app.getScreen()->setOrientation(true);
  app.getScreen()->setBrightness(1);
  app.onKeyPress(handleKeyPress);
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  setupDevices();
  setupApp();
  connect();
}

void loop() {
  int timeBudget = app.update();
  if (timeBudget > 0) {
    // if (!connecting) {
    //   if (millis() - lastViewChange > 10 * 1000) {
    //     nextView();
    //   }
    // }
    delay(timeBudget);
  }
}
