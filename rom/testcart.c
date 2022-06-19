#include "graphics.h"

void main() {
                /*plot(100,100,255);*/


       for (int x = 0; x < 256; x++)
       {
           for (int y = 0; y < 8; y++)
            {
                plot(x,y,x);
            }
       }
} 
