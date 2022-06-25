#include "graphics.h"
#include <math.h>
#include <stdlib.h>
void main() {

    clear(24);       
    waitTOF();

    // for (int i = 0; i < 256; i++)
    // {
    //     clear(i);

           
    //     waitTOF();
    // }
    
    // for (int x = 0; x < 256; x++)
    // {
    //     for (int y = 0; y < 8; y++)
    //     {
    //         plot(x,y+200,x);
            
    //     }
    // }

    // dots on corners
    plot(0,0,7);
    plot(319,239,7);
    plot(319,0,7);
    plot(0,239,7);
} 