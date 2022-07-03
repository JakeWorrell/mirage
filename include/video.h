#include <map>
#include <array>

#include <SDL2/SDL.h>

#include "../z80/z80.h"
#include "constants.h"

#define ONE_MEBIBYTE 1048576

using z80::fast_u8;
using z80::fast_u16;
using z80::least_u8;

class video_chip;

class video_chip
{
    private:
        /* data */

        fast_u8 colour, tile_id, mode, state, statePosition; 
        fast_u16 x, y, width = 640, height = 480;

        std::map<fast_u8, std::array<fast_u8, 10>> stateMap = {
            {VIDEO_MODE_CLEAR, {VIDEO_STATE_RCV_COLOR, VIDEO_STATE_BUSY, VIDEO_STATE_READY}},
            {VIDEO_MODE_PLOT, {VIDEO_STATE_RCV_X_UPPER, VIDEO_STATE_RCV_X_LOWER,VIDEO_STATE_RCV_Y_UPPER, VIDEO_STATE_RCV_Y_LOWER, VIDEO_STATE_RCV_COLOR, VIDEO_STATE_BUSY, VIDEO_STATE_READY}},
            {VIDEO_MODE_TILESHEET_BLIT_TILE, {VIDEO_STATE_RCV_X_UPPER, VIDEO_STATE_RCV_X_LOWER,VIDEO_STATE_RCV_Y_UPPER, VIDEO_STATE_RCV_Y_LOWER, VIDEO_STATE_RCV_TILE_ID, VIDEO_STATE_BUSY, VIDEO_STATE_READY}}
        };

    void execute() {
        switch (mode)
        {
            case VIDEO_MODE_CLEAR: {
                //printf("clear colour:%d x%d y%d\n", colour, x, y);
                memset(surface->pixels, colour, width * height * sizeof(uint8_t));   
                break;
            }
            case VIDEO_MODE_PLOT: {
                //printf("pset colour:%d x%d y%d\n", colour, x, y);
                int offset = x + y*width;
                unsigned *fp = (unsigned *)(surface->pixels + offset);
                memset(fp, colour, sizeof(uint8_t));       
                break;
            }
            case VIDEO_MODE_TILESHEET_BLIT_TILE: {
                tilesheet_blit_tile();  
                break;
            }
            default:
                break;
        } 
    }
    
    public:
        fast_u8 tof;
        SDL_Surface *surface;
        SDL_Surface *tile_memory[8] = {};


        video_chip() {
            surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 8, 0, 0, 0, 0);

            SDL_SetPaletteColors(surface->format->palette, palette, 0, 256);
            load_graphics_data();
                 
        }

        ~video_chip();




        void set_register(fast_u16 reg, fast_u8 value) {
            reg = reg&0xf;
            //printf("REG: %04x\n",reg&0xf);
            switch (reg)
            {
                case VIDEO_ADDR_MODE:
                    mode = value;
                    statePosition = 0;
                    state = stateMap[mode][statePosition];
                    break;
                case VIDEO_ADDR_IN:
                    receiveData(value);
                    break;
                default:
                    break;
            }
        }

        fast_u8 get_register(fast_u16 reg) {
            switch (reg)
            {
                case VIDEO_ADDR_TOF:{
                    return tof;
                }
                default:
                    break;
            }
            return 0x00;
        }

        void receiveData(fast_u8 data) {

            switch (state)
            {
                case VIDEO_STATE_RCV_X_UPPER:
                    x = data;

                    break;

                case VIDEO_STATE_RCV_X_LOWER:
                    x = x | data << 8;
                    break;

                case VIDEO_STATE_RCV_Y_UPPER:
                    y = data;

                    break;

                case VIDEO_STATE_RCV_Y_LOWER:
                    y = data;
                    break;

                case VIDEO_STATE_RCV_COLOR:
                    colour = data;
                    break;
                
                case VIDEO_STATE_RCV_TILE_ID:
                    tile_id = data;
                    break;

                default:
                    return;
            }
            //printf("video:%d\n", state);
            statePosition++;
            state = stateMap[mode][statePosition];

        }

        void update() {
            
            if (state == VIDEO_STATE_BUSY){
                execute();
            }
        }

        void tilesheet_blit_tile(void) {
            SDL_Rect srcrect;

            srcrect.x = 6*tile_id;
            srcrect.y = 0;
            srcrect.w = 6;
            srcrect.h = 8;

            SDL_Rect destrect;

            destrect.x = x;
            destrect.y = y;
            destrect.w = 6;
            destrect.h = 8;

            SDL_BlitSurface(tile_memory[0],
                     &srcrect,
                    surface,
                    &destrect);
        }

        void load_graphics_data() {
            SDL_RWops *rwop;
            rwop=SDL_RWFromFile("assets/font.pcx", "rb");
            tile_memory[0]=IMG_LoadPCX_RW(rwop);
            if(!tile_memory[0]) {
                printf("IMG_LoadPCX_RW: %s\n", IMG_GetError());
                // handle error
            }
        }
};
