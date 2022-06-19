#include "graphics.h"

void main() {
    
    for (size_t i = 0; i < 18; i++)
    {
            clear(17);
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
