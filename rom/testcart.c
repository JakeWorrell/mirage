#include "graphics.h"
#include <math.h>
#include <stdlib.h>

void main() {
    
    clear(24);       
    
    for (int x = 0; x < 256; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            plot(x+31,y+10,x);
            
        }
    }

    // corners
    plot(0,0,7);
    plot(319,239,7);
    plot(319,0,7);
    plot(0,239,7);

    blit_tile(100,100,1);

    waitTOF();

} 