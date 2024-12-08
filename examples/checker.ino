#include <RGBargy.h>

// Check cpu freq when setting resolution!
RGBargy rgbg(RGBG_MODE_640x480,     125);
// RGBargy rgbg(RGBG_MODE_800x600,  120);
// RGBargy rgbg(RGBG_MODE_1024x768, 175);

int mw = rgbg.get_mode_width()  / 8;
int mh = rgbg.get_mode_height() / 8;

void setup() {}

void loop() {
  int x, y, xd, yd, xs, ys;
  byte cs;
  long m = millis()/1000;
  for (yd=0; yd<8; yd++) {
    for (y=0; y<mh; y++) {
      ys = yd*mh+y;
      for (xd=0; xd<8; xd++) {
        cs = (xd+yd+m)%8;
        xs = xd*mw;
        for (x=0; x<mw; x++) {
          rgbg.pixel(xs+x, ys, cs);
        }
      }
    }
  }
}

// original simpler but slower version
// void loop() {
//   int x  = 0;
//   int y  = 0;
//   long m = millis()/1000;
//   for (y=0; y<mh; y++) {
//     for (x=0; x<mw; x++) {
//       rgbg.pixel(x, y, (x / 80 + y / 60 + m) % 8 );
//     }
//   }
// }