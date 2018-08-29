#include <Arduino.h>

#include <espix-design.h>

#include "../time/views/IndexView.h"
#include "../weather/views/IndexView.h"

class RootView : public CarouselContainer {
public:
  RootView() : CarouselContainer() {
    addSubviews({new TimeIndexView(), new WeatherIndexView()});
  }

  void didMount() {
    if (getCurrentView() == NULL) {
      slideToFirstSubview();
    }
  }
};
