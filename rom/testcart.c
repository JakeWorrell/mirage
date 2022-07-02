#include "graphics.h"
#include <stdio.h>
#include <string.h>


void blit_text(int x, int y, char *text) {
    for (int i = 0; i < strlen(text); i++){
        char character = text[i]-30;
        if (character != 2) {
            blit_tile(x,y,character);            
        }
        x+=8;
    }
}

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


    blit_text(100,100,"hello world!");

    waitTOF();

} 