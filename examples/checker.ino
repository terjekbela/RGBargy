#include <RGBargy.h>

RGBargy rgbg;

void setup() {
  rgbg.init();
}

void loop() {
  int x  = 0;
  int y  = 0;
  int i  = 0;
  int xc = 0;
  int yc = 0;
  for (y=0; y<480; y++) {
    if (yc == 60) {
      yc = 0;
      i  = (i+1) % 8;
    }
    yc++;
    for (x=0; x<640; x++) {
      if (xc == 80) {
        xc = 0;
        i = (i + 1) % 8;
      }
      xc++;
      rgbg.pixel(x, y, i);
    }
  }
}
