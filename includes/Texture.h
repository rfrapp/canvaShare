
#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "Rect.h"
#include <cmath>
#include <string>
#include <iostream>

class Texture
{
private:
    SDL_Texture *texture;
    SDL_PixelFormat pixel_format;
    SDL_RendererFlip flip_mode;
    SDL_Point *center;
    Uint32 format;
    Uint32 keycolor;
    int width, height;
    int pitch;
    double angle;
    void *pixels;

public:
    Texture() : texture(NULL), width(0), height(0), angle(0.0)
    { 
    }

    ~Texture() { destroy(); }

    void destroy() 
    {
        if (texture != NULL)
        { 
            SDL_DestroyTexture(texture); 
            texture = NULL;
            width = 0; 
            height = 0; 
        }
    }   

    bool load_from_file(SDL_Renderer *renderer, std::string path, int c_r = -1, int c_g = -1, int c_b = -1);
    bool load_from_file_streaming(SDL_Renderer *renderer, 
                                  std::string path, int c_r = -1, int c_g = -1, int c_b = -1);

    void render(SDL_Renderer *renderer, int x = 0, int y = 0, SDL_Rect *clip = NULL);

    void set_color_mod(Uint8 r, Uint8 g, Uint8 b)
    {
        SDL_SetTextureColorMod(texture, r, g, b);
    }

    void set_blend_mode(SDL_BlendMode mode)
    {
        SDL_SetTextureBlendMode(texture, mode);
    }

    void set_alpha(Uint8 alpha)
    {
        SDL_SetTextureAlphaMod(texture, alpha);
    }

    void set_as_target(SDL_Renderer *r)
    {
        SDL_SetRenderTarget(r, texture);
    }

    int get_width()             const   { return width;     }
    int get_height()            const   { return height;    }
    SDL_Texture * get_texture() const   { return texture;   }
    void * get_pixels()         const   { return pixels;    }
    int get_pitch()             const   { return pitch;     }
    double get_angle()          const   { return angle;     }
    SDL_RendererFlip get_flip() const   { return flip_mode; }

    void set_angle(double a) { angle = a; }
    void flip(SDL_RendererFlip f) { flip_mode = f; }

    bool lock();
    bool unlock();

    SDL_PixelFormat get_pixel_format();
    Uint32 get_format() { return format; }
    Uint32 get_colorkey() const { return keycolor; }
};

#endif 
