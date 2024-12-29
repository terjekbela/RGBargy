#include <RGBargy.h>

// Check cpu freq when setting resolution!
RGBargy vga0(VGA_MODE_640x480);
// RGBargy vga0(VGA_MODE_800x600);
// RGBargy vga0(VGA_MODE_1024x768);

short w = vga0.get_mode_width()  / 16;
short h = vga0.get_mode_height() / 16;

void setup() {
  vga0.begin();
}

void loop() {
  int x, y, xd, yd, xs, ys;
  byte cs;
  long m = millis()/1000;
  for (yd=0; yd<16; yd++) {
    for (y=0; y<h; y++) {
      ys = yd*h+y;
      for (xd=0; xd<16; xd++) {
        cs = (xd+yd+m)%16;
        xs = xd*w;
        for (x=0; x<w; x++) {
          vga0.pixel(xs+x, ys, cs);
        }
      }
    }
  }
}

// simpler but slower version
// void loop() {
//   int x  = 0;
//   int y  = 0;
//   long m = millis()/1000;
//   for (y=0; y<h; y++) {
//     for (x=0; x<w; x++) {
//       vga0.pixel(x, y, (x / 80 + y / 60 + m) % 8 );
//     }
//   }
// }