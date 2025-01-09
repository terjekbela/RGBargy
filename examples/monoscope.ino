#include <RGBargy.h>

// Check cpu freq when setting resolution!
RGBargy vga0(VGA_MODE_640x480);
// RGBargy vga0(VGA_MODE_800x600);
// RGBargy vga0(VGA_MODE_1024x768);

short w = vga0.get_mode_width()  - 1;
short h = vga0.get_mode_height() - 1;

void setup() {
  randomSeed(analogRead(0));
  vga0.begin();
}

void loop() {
  byte  c = random(15)+1;
  vga0.rect(0, 0, w-1, h-1, c);
  vga0.line(0, 0, w-1, h-1, c);
  vga0.line(w-1, 0, 0, h-1, c);
  vga0.circle(w/2, h/2, w/2, c);
  vga0.circle(w/2, h/2, w/4, c);
  vga0.circle(w/2, h/2, h/2, c);
  vga0.circle(w/2, h/2, h/6, c);
  delay(10);
}
