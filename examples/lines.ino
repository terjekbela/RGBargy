#include <RGBargy.h>

RGBargy rgbg(RGBG_MODE_640x480);

void setup() {
  randomSeed(analogRead(0));
}

void loop() {
  rgbg.line(
    random(640), random(480),
    random(640), random(480),
    random(7)+1
  );
  delay(1);
}
