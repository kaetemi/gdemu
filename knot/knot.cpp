// from http://jlswbs.blogspot.be/

#include "SPI.h"
#include "GD.h"

    byte i;
    float x = 0.1;
    float y = 0;

void setpixel(byte x, byte y, byte color)
{
    unsigned int addr = RAM_SPRIMG|(x & 0xf)|(y << 4)|((x & 0x30)<<8);
    byte mask = 0xc0 >> ((x >> 5) & 6);
    GD.wr(addr, (GD.rd(addr) & ~mask) | (color & mask));
}

void setup()
{
    int i;
    GD.begin();
    GD.ascii();
    for (i = 0; i < 256; i++) {
    int x =     72 + 16 * ((i >> 4) & 15);
    int y =     22 + 16 * (i & 15);
    int image = i & 63;
    int pal =   3-(i >> 6);
    GD.sprite(i, x, y, image, 0x8 | (pal << 1), 0,0);}
    GD.fill(RAM_SPRIMG, 0, 16384);
    GD.wr16(PALETTE4A + 6, RGB(255,255,255));
    GD.putstr(17, 1, "Knot fractal map");
}

void loop()
{
    double nx = x;
    x = -y;
    y = pow(nx,3)-nx-y;
    setpixel (128+(85*x), 128+(85*y),255);

       // added delay
       delayMicroseconds(1);
}
