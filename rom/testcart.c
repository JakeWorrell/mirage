#include "graphics.h"
#include <stdio.h>
#include <string.h>


void blit_text(int x, int y, char *text) {
    int initial_x = x;
    for (int i = 0; i < strlen(text); i++){
        if (text[i]== 10) {
            x = initial_x;
            y+=8;
        } else if (text[i]==9){
            x+=24;
        } else {
            char character = text[i]-32;
            if (character >0) {
                blit_tile(x,y,character);            
            }
            x+=6;
        }
        
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


    blit_text(100,100,"Hello World!\n\tHello Orca!");



} 