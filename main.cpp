#include <iomanip>
#include <ctime>
#include <string>  
#include <iostream> 
#include <sstream> 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "include/palette.h"
#include "include/video.h"

#include "z80/z80.h"

using z80::fast_u8;
using z80::fast_u16;
using z80::least_u8;

#define V_WIDTH 320
#define V_HEIGHT 240
#define V_SCALE 3
#define V_RAM_SIZE V_WIDTH*V_HEIGHT
#define ROM_MAX_SIZE 100




class my_emulator : public z80::z80_cpu<my_emulator> {
public:
    video_chip *video;

    typedef z80::z80_cpu<my_emulator> base;

    my_emulator() {
        init_video();
    }


    void load_rom(const char* romFileName) {
        FILE * pFile;
        size_t lSize;
        size_t result;

        pFile = fopen ( romFileName , "rb" );
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
        //              static_cast<unsigned>(addr));
        memory[addr] = static_cast<least_u8>(n);
    }

    void on_halt() {
        std::printf("HALT\n");
        base::on_halt();
        //exit(0);
    }

     void on_step() {
        base::on_step();

        video->update();
    }

    void on_output(fast_u16 port, fast_u8 n) {
        
        // std::printf("output 0x%02x to 0x%04x\n", static_cast<unsigned>(n),
        //             static_cast<unsigned>(port));
        
        video->set_register(port,n);
    }

    fast_u8 on_input(fast_u16 port) {
        return video->get_register(port);
    }

    void render_display() {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, video->surface);
        SDL_RenderCopy(renderer, texture, &viewport, NULL);
        SDL_RenderPresent(renderer);   
    }

    private:

    least_u8 memory[z80::address_space_size] = {};
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect viewport{0,0,320,240};


    void init_video() {
        // returns zero on success else non-zero
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            printf("error initializing SDL: %s\n", SDL_GetError());
        }
        
        window = SDL_CreateWindow("Foo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, V_WIDTH*V_SCALE, V_HEIGHT*V_SCALE, 0);
        renderer = SDL_CreateRenderer(window, -1, 0);
        
    
        video = new video_chip();
    }
};


void screenshot(SDL_Surface* surface) {
    std::stringstream buffer;
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    buffer << "screenshot_" << std::put_time(&tm, "%d-%m-%y_%H-%M-%S") << ".png";
    const std::string tmp =  std::string{buffer.str()};
    const char* str = tmp.c_str();
    SDL_SaveBMP(surface, str);
}

 
int main(int argc, char *argv[])
{
    int quit = 0;
    my_emulator e;

    e.load_rom("testcart.bin");

    const int FPS = 60;
    const int DELAY_TIME = 1000 / FPS;
    Uint32 frameStart, frameTime;
    
    SDL_Event event;

    /* Loop until an SDL_QUIT event is found */

    while ( !quit ) {
		frameStart = SDL_GetTicks();
          
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
                        case SDLK_F10: {
                            screenshot(e.video->surface);
                            break;
                        }
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

        size_t i = 0;
        while (!e.on_is_halted())
        {
            e.video->tof = i<5; // TOF for the first few clock cycles

            e.on_step();  

            if (i >=100000) {
                break;
            }
            i++;
        }
       
        frameTime = SDL_GetTicks() - frameStart;

		if (frameTime < DELAY_TIME)
		{
			SDL_Delay(int(DELAY_TIME - frameTime));
		}

        e.render_display();

        
    }

    
    return 0;
}
