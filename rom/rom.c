#include "graphics.h"

void main() {
__asm
ld a, 0xff
out (0x00), a
__endasm;
       
       for (int x = 0; x < 320; x++)
       {
           plot(x,34);
       }
       

}
