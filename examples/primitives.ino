#include <RGBargy.h>

// Check cpu freq when setting resolution!
RGBargy vga0(VGA_MODE_640x480);
// RGBargy vga0(VGA_MODE_800x600);
// RGBargy vga0(VGA_MODE_1024x768);

void setup() {
  randomSeed(analogRead(0));
  vga0.begin();
}

void loop() {
  int i;
  short w = vga0.get_mode_width();
  short h = vga0.get_mode_height();
  for(i=0; i<1000000; i++) vga0.pixel(r(w), r(h), r(15)+1);
  vga0.clear();
  vga0.grid(8);
  for(i=0; i<20000; i++) vga0.line(r(w), r(h), r(w), r(h), r(15)+1);
  vga0.clear();
  vga0.grid(8);
  for(i=0; i<10000; i++) vga0.circle(r(w), r(h), r(w/2), r(15)+1);
  vga0.clear();
  vga0.grid(8);
  for(i=0; i<10000; i++) vga0.ellipse(r(w), r(h), r(w/2), r(h/2), r(15)+1);
  vga0.clear();
  vga0.grid(8);
}
int r(int r) {return random(r);}
