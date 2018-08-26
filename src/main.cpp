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
#define ESC_BUTTON D3

bool connecting = false;
unsigned long lastUpdate = 0;
unsigned long lastViewChange = 0;
int viewIndex = 0;
int viewCount = 8;

SH1106Wire display(0x3c, OLED_SDA, OLED_CLK);
Application application;
ProgressView connectionView("Connecting to WiFi...", PROGRESS_INFINITY);

const uint8_t *animationFrames[] = {ANIMATION_XBM_01, ANIMATION_XBM_02, ANIMATION_XBM_03,
                                    ANIMATION_XBM_04, ANIMATION_XBM_05, ANIMATION_XBM_06,
                                    ANIMATION_XBM_07, ANIMATION_XBM_08, ANIMATION_XBM_09};

TextView ipView(FONT_SIZE_H2);
View *views[] = {
    new ClockView(),
    new WeatherTodayView(),
    new WeatherForecastView(),
    new ImageView(APPLE_XBM, APPLE_XBM_WIDTH, APPLE_XBM_HEIGHT),
    new TextView("Hello."),
    new TextView("Think different.", FONT_SIZE_H2),
    new AnimationView(animationFrames, ANIMATION_XBM_WIDTH, ANIMATION_XBM_HEIGHT, 9, 6),
    &ipView};

void setView(int index, TransitionOptions options = TRANSITION_OPTIONS_NONE) {
  viewIndex = index;
  application.setRootView(views[viewIndex], options);
  lastViewChange = millis();
}

void nextView(int duration = 200) {
  viewIndex++;
  if (viewIndex >= viewCount) {
    viewIndex = 0;
  }
  setView(viewIndex, TransitionOptions(TRANSITION_TO_BOTTOM, duration));
}

void previousView(int duration = 200) {
  viewIndex--;
  if (viewIndex < 0) {
    viewIndex = viewCount - 1;
  }
  setView(viewIndex, TransitionOptions(TRANSITION_TO_TOP, duration));
}

void handleKeyPress(KeyCode keyCode) {
  if (connecting) {
    return;
  }
  switch (keyCode) {
  case KEY_ENTER:
    nextView();
    break;
  case KEY_ESC:
    Serial.println("ESC button pressed.");
    break;
  }
}

void handleScroll(int delta) {
  if (connecting) {
    return;
  }
  int duration = 200;
  int value = std::abs(delta);
  if (value >= 4) {
    duration = 100;
  } else if (value >= 6) {
    duration = 50;
  } else if (value >= 8) {
    duration = 10;
  }
  if (delta > 0) {
    previousView(duration);
  } else {
    nextView(duration);
  }
}

void onConnected() {
  connecting = false;
  application.enableOTA();
  ipView.setText(WiFiNetwork.getLocalIP());
  setView(0, TransitionOptions(TRANSITION_TO_BOTTOM));
}

void connect() {
  application.setRootView(&connectionView);
  WiFiConnectionSetting settings[] = {
      WiFiConnectionSetting("Henry's iPhone 6", "13913954971"),
      WiFiConnectionSetting("Henry's Living Room 2.4GHz", "13913954971")};
  WiFiNetwork.connect(settings, 2, onConnected);
  connecting = true;
}

void setupDevices() {
  Screen.begin(&display);
  Screen.setOrientation(true);
  Screen.setBrightness(1);

  Keyboard.registerJogDial(KY04_CLK, KY04_DT, KY04_SW);
  Keyboard.registerKey(KEY_ESC, ESC_BUTTON);
  Keyboard.begin();
}

void setupApp() {
  application.begin();
  // Settings
  application.onKeyPress(handleKeyPress);
  application.onScroll(handleScroll);
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  setupDevices();
  setupApp();
  connect();
}

void loop() {
  int timeBudget = application.update();
  if (timeBudget > 0) {
    // if (!connecting) {
    //   if (millis() - lastViewChange > 10 * 1000) {
    //     nextView();
    //   }
    // }
    delay(timeBudget);
  }
}
