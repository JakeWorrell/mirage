#include "../include/constants.h"

void clear(unsigned colour);
void plot(unsigned x, unsigned y, unsigned colour);
void waitTOF();

void clear(unsigned colour) 
{
    __asm
    ld hl,3
    add hl,sp              ; skip over return address on stack
    inc hl

    ld a, VIDEO_MODE_CLEAR
    out (VIDEO_ADDR_MODE), a

    ld a,(hl) ;a=colour (upper)
    out (VIDEO_ADDR_IN), a

    __endasm;
}

/**
 * @brief 
 * 
 * @todo optomise INCs to to single ADD
 * 
 * @param x 
 * @param y 
 * @param colour 
 */
void plot(unsigned x, unsigned y, unsigned colour) 
{
    __asm
    ld hl,3
    add hl,sp              ; skip over return address on stack
    inc hl

    ld a, VIDEO_MODE_PLOT
    out (VIDEO_ADDR_MODE), a

    ld a,(hl) ;a=x (upper)
    out (VIDEO_ADDR_IN), a
    ;inc hl

    ld a,(hl) ;a=x (lower)
    out (VIDEO_ADDR_IN), a
    inc hl
    inc hl

    ld a,(hl) ;a=y (upper)
    out (VIDEO_ADDR_IN), a
    ;inc hl

    ld a,(hl) ;a=y (lower)
    out (VIDEO_ADDR_IN), a
    inc hl
    inc hl

    ld a,(hl) ;a=colour
    out (VIDEO_ADDR_IN), a

    __endasm;
}

void waitTOF() 
{ //this comparison stuff doesn't work
    __asm
    waitTOF:
            in a, (VIDEO_ADDR_TOF)
            cp 0x01
            jp nz, doneWaitTOF
            jp waitTOF
    doneWaitTOF:
    __endasm;

}