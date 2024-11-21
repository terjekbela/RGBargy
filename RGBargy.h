#ifndef RGBargy_h
#define RGBargy_h

class RGBargy {
  public:
    RGBargy();
    void init();
    void pixel(short x, short y, byte color);
  private:
    int _pin;
};

#endif
