#include <RGBargy.h>

// Check cpu freq when setting resolution!
RGBargy rgbg(RGBG_MODE_640x480,     125);
// RGBargy rgbg(RGBG_MODE_800x600,  120);
// RGBargy rgbg(RGBG_MODE_1024x768, 175);

short w = rgbg.get_mode_width()  - 1;
short h = rgbg.get_mode_height() - 1;

void setup() {
  randomSeed(analogRead(0));
}

void loop() {
  byte  c = random(15)+1;
  rgbg.rect(0, 0, w-1, h-1, c);
  rgbg.line(0, 0, w-1, h-1, c);
  rgbg.line(w-1, 0, 0, h-1, c);
  rgbg.circle(w/2, h/2, w/2, c);
  rgbg.circle(w/2, h/2, w/4, c);
  rgbg.circle(w/2, h/2, h/2, c);
  rgbg.circle(w/2, h/2, h/6, c);
  delay(10);
}
