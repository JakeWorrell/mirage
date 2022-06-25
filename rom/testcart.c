#include "graphics.h"
#include <math.h>
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

    int x=0,y=120;
    for (x = 40; x < 315; x+=2) {
		clear(0);
		plot(x,y,255);
		//y=sin(x);
        y=x;
        waitTOF();
	}
} 