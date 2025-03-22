#include <RGBargy.h>

// Check cpu freq when setting resolution!
RGBargy vga0(VGA_MODE_640x480, VGA_PORT_2, VGA_COLOR_6bit_rrggbb);
// RGBargy vga0(VGA_MODE_800x600);
// RGBargy vga0(VGA_MODE_1024x768);

short w = vga0.get_mode_width();
short h = vga0.get_mode_height();
short c = 1 << vga0.get_mode_bitdepth();

void setup() {
  Serial.begin(115200);
  vga0.begin();
}

void loop() {
  Serial.println(1 << vga0.get_mode_bitdepth());
  for (short x=0; x<c; x++) {
    vga0.rect(x*w/c, 0, (x+1)*w/c-2, h-1, x, true);
  }
}
