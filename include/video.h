#include <SDL2/SDL.h>

#include "../z80/z80.h"

using z80::fast_u8;
using z80::fast_u16;
using z80::least_u8;

class video_chip;

class video_chip
{
    private:
        /* data */

        fast_u8 colour, x, y, mode;
        int width= 320, height=240;
    public:
        SDL_Surface *surface;

        video_chip() {
            surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 8, 0, 0, 0, 0);

            SDL_SetPaletteColors(surface->format->palette, palette, 0, 256);

            memset(surface->pixels, 0, width * height * sizeof(uint8_t));        
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
            int offset = x + y*width;
            unsigned *fp = (unsigned *)(surface->pixels + offset);
            memset(fp, colour, sizeof(uint8_t));        
        }
};
