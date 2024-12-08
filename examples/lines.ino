#include <RGBargy.h>

// Check cpu freq when setting resolution!
RGBargy rgbg(RGBG_MODE_640x480,     125);
// RGBargy rgbg(RGBG_MODE_800x600,  120);
// RGBargy rgbg(RGBG_MODE_1024x768, 175);

short w = rgbg.get_mode_width();
short h = rgbg.get_mode_height();

void setup() {
  randomSeed(analogRead(0));
}

void loop() {
  rgbg.line(r(w), r(h), r(w), r(h), r(15)+1);
  delay(1);
}
int r(int r) {return random(r);}
