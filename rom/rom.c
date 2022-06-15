#include "graphics.h"

void main() {
__asm
ld a, 0xff
out (0x00), a
__endasm;
       
       for (int x = 0; x < 256; x++)
       {
           for (int y = 0; y < 240; y++)
            {
                plot(x,y,x);
            
            }
        
       }
       

} 
