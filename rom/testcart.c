#include "graphics.h"

void main() {
    
    for (int i = 0; i < 255; i++)
    {
        clear(i);



        //wait frame somehow
    }
    
    for (int x = 0; x < 256; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            plot(x,y,x);
        }
    }
} 
