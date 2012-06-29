// from http://jlswbs.blogspot.be/

#include "SPI.h"
#include "GD.h"

       byte i;
       float a = 1;
       float b = 0.9;
       float c = 0.4;
       float d = 6;
       float x = 0;
       float y = 0;
       float z = 0;
       float xn = x;
       float yn_ = y;
       float zn = z;
       float dt = 0.01;

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
       int x = 72 + 16 * ((i >> 4) & 15);
       int y = 22 + 16 * (i & 15);
       int image = i & 63;
       int pal = 3-(i >> 6);
       GD.sprite(i, x, y, image, 0x8 | (pal << 1), 0,0);}
       GD.fill(RAM_SPRIMG, 0, 16384);
       GD.wr16(PALETTE4A + 6, RGB(255,255,255));
       GD.putstr(13, 1, "Ikeda chaotic attractor");
}

void loop()
{
       xn = a+b*(x*cos(z)-y*sin(z));
       yn_ = b*(x*sin(z)+y*cos(z));
       zn = c-d/(1+pow(x,2)+pow(y,2));
       x = x+xn*dt;
       y = y+yn_*dt;
       z = z+zn*dt;
       setpixel (128+(x/5.3), 140+(y/6),255);

       // added delay
       delayMicroseconds(1);
}
