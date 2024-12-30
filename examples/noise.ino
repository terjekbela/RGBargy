#include <RGBargy.h>

// Check cpu freq when setting resolution!
RGBargy vga0(VGA_MODE_640x480);
// RGBargy vga0(VGA_MODE_800x600);
// RGBargy vga0(VGA_MODE_1024x768);

short w = vga0.get_mode_width();
short h = vga0.get_mode_height();

void setup() {
  randomSeed(analogRead(0));
  vga0.begin();
}

void loop() {
  int x, y;
  char c;
  for(y=0; y<h/2; y++) {
    for(x=0; x<w/2; x++) {
      c = random(16);
      vga0.pixel(x*2,   y*2,   c);
      vga0.pixel(x*2+1, y*2,   c);
      vga0.pixel(x*2,   y*2+1, c);
      vga0.pixel(x*2+1, y*2+1, c);
    }
  }
}
