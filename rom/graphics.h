

void plot(unsigned x, unsigned y, unsigned colour);


void plot(unsigned x, unsigned y, unsigned colour) 
{
    __asm
    ld hl,3
    add hl,sp              ; skip over return address on stack
    inc hl
    ld a,(hl) ;a=x
    out (0x01), a
    add hl,2
    ld a,(hl) ;a=y
    out (0x02), a
    add hl,2

    ld a, (hl)
    out (0x00), a

    ld a, 1
    out (0x03), a
    __endasm;
}