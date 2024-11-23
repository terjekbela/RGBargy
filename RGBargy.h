#ifndef RGBargy_h
#define RGBargy_h

#define RGBG_MODE_640x480     1
#define RGBG_MODE_800x600     2
#define RGBG_MODE_1024x768    3

class RGBargy {
  public:
    RGBargy(byte mode);
    void pixel(short x, short y, byte color);
  private:
    int _pin;
};

#endif
