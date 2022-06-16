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
#define V_RAM_SIZE V_WIDTH*V_HEIGHT
#define ROM_MAX_SIZE 100


class video_chip
{
    private:
        /* data */

        fast_u8 colour, x, y, mode;
            SDL_Color colors[256];

    
    public:
        SDL_Surface *surface;

        video_chip() {
            surface = SDL_CreateRGBSurface(0, V_WIDTH, V_HEIGHT, 32,
                                    0x00FF0000,
                                    0x0000FF00,
                                    0x000000FF,
                                    0xFF000000);

int i;

for(i = 0; i < 256; i++)
{
    colors[i].r = colors[i].g = colors[i].b = (Uint8)i;
}


SDL_SetPaletteColors(surface->format->palette, colors, 0, 256);

            memset(surface->pixels, 0, V_WIDTH * V_HEIGHT * sizeof(uint8_t));        
        }

        ~video_chip();


        void set_register(fast_u16 reg, fast_u8 value) {
            reg = reg&0xf;
            //printf("REG: %04x\n",reg&0xf);
            switch (reg)
            {
                case 0x00:
                    colour = value;
                    break;
                case 0x01:
                    x = value;
                    break;
                case 0x02:
                    y = value;
                    break;
                case 0x03:
                    mode = value;
                    execute();
                    break;
                default:
                    break;
            }
        }


        void execute() {
            //printf("pset colour:%d x%d y%d\n", colour, x, y);
            int offset = x + y*V_WIDTH;
            unsigned *fp = (unsigned *)(surface->pixels + offset);
            memset(fp, colour, sizeof(uint8_t));        


        }
};


class my_emulator : public z80::z80_cpu<my_emulator> {
public:
    typedef z80::z80_cpu<my_emulator> base;

    my_emulator() {
        load_rom();
        init_video();
    }

    void on_set_pc(z80::fast_u16 pc) {
        //std::printf("pc = 0x%04x\n", static_cast<unsigned>(pc));
        base::on_set_pc(pc);
    }

    fast_u8 on_read(fast_u16 addr) {
        assert(addr < z80::address_space_size);
        fast_u8 n = memory[addr];
        // std::printf("read 0x%02x at 0x%04x\n", static_cast<unsigned>(n),
        //             static_cast<unsigned>(addr));
        return n;
    }

    void on_write(fast_u16 addr, fast_u8 n) {
        assert(addr < z80::address_space_size);
        // std::printf("write 0x%02x at 0x%04x\n", static_cast<unsigned>(n),
        //             static_cast<unsigned>(addr));
        memory[addr] = static_cast<least_u8>(n);
    }

    void on_halt() {
        std::printf("HALT\n");
        base::on_halt();
    }

    void on_output(fast_u16 port, fast_u8 n) {
        
        //std::printf("output 0x%02x to 0x%04x\n", static_cast<unsigned>(n),
        //            static_cast<unsigned>(port));
        
        video->set_register(port,n);
    }

    void render_display() {
        
        
        SDL_UpdateTexture(texture, NULL, video->surface->pixels, video->surface->pitch);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);   
    }

    private:

    least_u8 memory[z80::address_space_size] = {};
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    video_chip *video;

    void load_rom() {
        FILE * pFile;
        size_t lSize;
        size_t result;

        pFile = fopen ( "testrom.bin" , "rb" );
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
        
        SDL_CreateWindowAndRenderer(V_WIDTH *2, V_HEIGHT*2, SDL_RENDERER_PRESENTVSYNC , &window, &renderer);
    
        texture = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_INDEX8,
            SDL_TEXTUREACCESS_STREAMING, V_WIDTH, V_HEIGHT);
        	

        video = new video_chip();
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

            // 7mhz = 7000000; 60fps (I think) so 7M/60 = 116666. no idea
            for (size_t i = 0; i < 116666; i++)
            {
                e.on_step();
            }
        
            e.render_display();
        }

    }
    return 0;
}