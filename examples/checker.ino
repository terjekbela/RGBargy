#include <RGBargy.h>

RGBargy rgbg;

void setup() {
  rgbg.init();
}

void loop() {
  int x = 0;
  int y = 0;
  for (y=0; y<480; y++) {
    for (x=0; x<640; x++) {
      rgbg.pixel(x, y, (x / 80 + y / 60) % 8 );
    }
  }
}
