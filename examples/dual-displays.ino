#include <RGBargy.h>

// Check cpu freq when setting resolution!
RGBargy vga0(VGA_MODE_640x480, VGA_PORT_0);
RGBargy vga1(VGA_MODE_640x480, VGA_PORT_1);
// RGBargy vga0(VGA_MODE_800x600,  VGA_PORT_0);
// RGBargy vga0(VGA_MODE_1024x768. VGA_PORT_0);

void setup() {
  randomSeed(analogRead(0));
  vga0.begin(VGA_LARGE_8);
  vga1.begin(VGA_LARGE_8);
  vga0.grid(8);
  vga1.grid(8);
}

void loop() {
  short w = vga0.get_mode_width();
  short h = vga0.get_mode_height();
  for(int i=0; i<1000; i++) {
    vga0.pixel(r(w), r(h), r(15)+1);
    vga1.pixel(r(w), r(h), r(15)+1);
  }
  vga0.circle(w/2, h/2, 20, 8, true);  // drawing "0"
  vga0.ellipse(w/2, h/2, 6, 10, 15);
  vga1.circle(w/2, h/2, 20, 8, true);  // drawing "1"
  vga1.vline(w/2+2, h/2-10, 20, 15);
  vga1.line(w/2+2, h/2-10, w/2-5, h/2-5, 15);
  delay(1);
}
int r(int r) {return random(r);}
