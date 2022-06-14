#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "z80/z80.h"

using z80::fast_u8;
using z80::fast_u16;
using z80::least_u8;

#define V_WIDTH 320
#define V_HEIGHT 240
#define V_SCALE 2

#define ROM_MAX_SIZE 100

class my_emulator : public z80::z80_cpu<my_emulator> {
public:
    typedef z80::z80_cpu<my_emulator> base;

    my_emulator() {
        load_rom();
        init_video();
    }

    void on_set_pc(z80::fast_u16 pc) {
        std::printf("pc = 0x%04x\n", static_cast<unsigned>(pc));
        base::on_set_pc(pc);
    }

    fast_u8 on_read(fast_u16 addr) {
        assert(addr < z80::address_space_size);
        fast_u8 n = memory[addr];
        std::printf("read 0x%02x at 0x%04x\n", static_cast<unsigned>(n),
                    static_cast<unsigned>(addr));
        return n;
    }

    void on_write(fast_u16 addr, fast_u8 n) {
        assert(addr < z80::address_space_size);
        std::printf("write 0x%02x at 0x%04x\n", static_cast<unsigned>(n),
                    static_cast<unsigned>(addr));
        memory[addr] = static_cast<least_u8>(n);
    }

    void on_halt() {
        std::printf("HALT\n");
        base::on_halt();
    }


    private:

    least_u8 memory[z80::address_space_size] = {};
    SDL_Window* win;

    void load_rom() {
        FILE * pFile;
        size_t lSize;
        size_t result;

        pFile = fopen ( "rom.bin" , "rb" );
        if (pFile==NULL) {fputs ("Error finding rom.bin",stderr); exit (1);}

        // obtain file size:
        fseek (pFile , 0 , SEEK_END);
        lSize = ftell (pFile);
        rewind (pFile);

        // copy the file into the memory:
        result = fread (memory,1,lSize,pFile);
        if (result != lSize) {fputs ("Error reading rom.bin",stderr); exit (3);}

        fclose (pFile);
    }

    void init_video() {
        // returns zero on success else non-zero
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            printf("error initializing SDL: %s\n", SDL_GetError());
        }
        win = SDL_CreateWindow("GAME",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            V_WIDTH * V_SCALE, V_HEIGHT * V_SCALE, 0);
 
    }
};
 
int main(int argc, char *argv[])
{
    int quit = 0;
    my_emulator e;
    
    SDL_Event event;

    /* Loop until an SDL_QUIT event is found */
    while( !quit ){

        /* Poll for events */
        while( SDL_PollEvent( &event ) ){
            
            switch( event.type ){
                /* Keyboard event */
                /* Pass the event data onto PrintKeyInfo() */
                case SDL_KEYDOWN:
                    switch( event.key.keysym.sym ){
                        case SDLK_ESCAPE:
                            quit = 1;
                            break;
                        default:
                            break;
                    }
                case SDL_KEYUP:
                    break;

                /* SDL_QUIT event (window close) */
                case SDL_QUIT:
                    quit = 1;
                    break;

                default:
                    break;
            }

        }
        if (!e.on_is_halted()) {
            e.on_step();
        }

    }
    return 0;
}