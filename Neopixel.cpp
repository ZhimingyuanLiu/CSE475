#include "Neopixel.h"
#include "Debug.h"

// Initialize fixed list of light gestures.
constexpr void (*Neopixel::LIGHTS[])(uint32_t);

uint8_t Neopixel::_currentIdx = 0;
uint32_t Neopixel::_lastLoop = 0;
Adafruit_NeoPixel_ZeroDMA Neopixel::_strip = Adafruit_NeoPixel_ZeroDMA(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRBW);

void Neopixel::setup() {
  _strip.begin();
  _strip.setBrightness(5);
  _strip.show();
}

void Neopixel::loop() {
  uint32_t thisLoop = millis();
  uint32_t dt;
  if (_lastLoop) {
    dt = thisLoop - _lastLoop;
  } else {
    dt = 0;
  }
  _lastLoop = thisLoop;

  if (LIGHTS[_currentIdx] != nullptr) {
    LIGHTS[_currentIdx](dt);
  }
}

void Neopixel::setLight(uint8_t lightIdx) {
  // lightIdx soundIdx. Anything outside of the bounds of the array is 0.
  if (lightIdx >= sizeof(LIGHTS) / sizeof(void*)) {
    lightIdx = 0;
  }

  if (_currentIdx != lightIdx) {
    _strip.clear();
    _strip.show();

    _currentIdx = lightIdx;
    _lastLoop = 0;
  }
}

uint8_t Neopixel::getLight() {
  return _currentIdx;
}

void Neopixel::rainbow(uint32_t dt) {
  static uint8_t offset = 0;
  static uint32_t rainbowColors[NEOPIXEL_COUNT] = {16711680, 13578240, 10444800, 7311360, 4177920, 1044480, 56865, 44625,
                                                   32385, 20145, 7905, 1179885, 4325565, 7471245, 10616925, 13762605};

  if (dt > 50) {
    for (uint8_t pix = 0; pix < NEOPIXEL_COUNT; pix++) {
      _strip.setPixelColor((pix + offset) % NEOPIXEL_COUNT, rainbowColors[pix]);
    }
    _strip.show();
    offset++;
  }
}

void Neopixel::wind_light() {

  int num = _strip.numPixels();
  for (uint16_t i = 0; i < _strip.numPixels(); i++) {
    int wait = random(20,120);
    _strip.setPixelColor(i%num,Wheel(((i * 256 / _strip.numPixels()) + i) & 255));
    _strip.setPixelColor((i+1)%num,Wheel(((i * 256 / _strip.numPixels()) + i) & 255));
    _strip.setPixelColor((i+2)%num,Wheel(((i * 256 / _strip.numPixels()) + i) & 255));
    _strip.setPixelColor((i+3)%num,Wheel(((i * 256 / _strip.numPixels()) + i) & 255));
    _strip.setPixelColor((i+4)%num,Wheel(((i * 256 / _strip.numPixels()) + i) & 255));
    _strip.show();
    delay(wait);
    _strip.clear();
  }
}


uint32_t Neopixel::Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return _strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return _strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return _strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
