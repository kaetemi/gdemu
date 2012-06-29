// from http://jlswbs.blogspot.be/2011/09/chua-chaotic-oscillator.html

#include "SPI.h"
#include "GD.h"


        byte i;
        float x = 0.5;
float y = 0.25;
float z = 0.125;
float oldx = 0;
float oldy = 0;
float oldz = 0;
        float h = 0;
        float dt = 0.005;
        float alpha = 15.6;
float beta = 28.58;
float a = -1.14286;
float b = -0.714286;


void setup()
{
       GD.begin();
       GD.ascii();
       GD.wr16(RAM_SPRPAL + (0 * 2), 0x8000);
       GD.wr16(RAM_SPRPAL + (1 * 2), RGB(255, 255, 255));
       GD.fill(RAM_SPRIMG, 0, 256);
       GD.wr(RAM_SPRIMG + 0x78, 1);
       GD.putstr(13, 1, "Chua chaotic oscillator");
}


void loop()
{
        oldx = x;
oldy = y;
oldz = z;
        h = (b * x) + (0.5 * (a - b) * (abs(x+1) - abs(x-1)));
x = oldx + dt * (alpha * (oldy - oldx - h));
y = oldy + dt * (oldx - oldy + oldz);
z = oldz + dt * (-beta * oldy);
       GD.sprite(i++, 200+(80*x), 150+(4*80*y), 0, 0, 0);

       // added delay
       delayMicroseconds(1000);
}
