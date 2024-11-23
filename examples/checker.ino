#include <RGBargy.h>

RGBargy rgbg(RGBG_MODE_640x480);

void setup() {
}

void loop() {
  int x, y, xd, yd;
  int m = millis()/1000;
  for (yd=0; yd<8; yd++) {
    for (y=0; y<60; y++) {
      for (xd=0; xd<8; xd++) {
        for (x=0; x<80; x++) {
          rgbg.pixel(xd*80+x, yd*60+y, (xd+yd+m)%8 );
        }
      }
    }
  }
}
