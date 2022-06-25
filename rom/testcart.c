#include "graphics.h"
#include <math.h>
#include <stdlib.h>
void main() {

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

    do
    {

        plot(rand() % 320,rand() % 240,rand() % 255);
        /* code */
    } while (1);

} 