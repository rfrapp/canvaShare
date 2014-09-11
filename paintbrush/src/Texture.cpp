
#include "Texture.h"

bool Texture::load_from_file(SDL_Renderer *renderer, std::string path, int c_r, int c_g, int c_b)
{
    destroy();

    SDL_Surface *loaded = IMG_Load(path.c_str());
    SDL_Texture *t = NULL;

    if (loaded == NULL)
    {
        std::cout << "error loading texture image at path: " << path << std::endl;
    }
    else
    {
        if (c_r != -1 && c_g != -1 && c_b != -1)
        {
            // color key the image
            SDL_SetColorKey(loaded, SDL_TRUE, SDL_MapRGB(loaded->format, c_r, c_g, c_b));
        }

        t = SDL_CreateTextureFromSurface(renderer, loaded);

        if (t == NULL)
            std::cout << "error creating texture from image loaded at path: " << path << std::endl;
        else
        {
            width = loaded->w;
            height = loaded->h;

            SDL_QueryTexture(texture, &format, NULL, NULL, NULL);

            pixel_format = *loaded->format;
        }
        
        SDL_FreeSurface(loaded);
    }

    texture = t;
    return texture != NULL;
}

bool Texture::load_from_file_streaming(SDL_Renderer *renderer, 
                                  std::string path, int c_r, int c_g, int c_b)
{
    destroy();

    SDL_Texture *t = NULL;

    SDL_Surface *loaded = IMG_Load(path.c_str());

    if (loaded == NULL)
        std::cout << "Error loading surface at " << path << std::endl;
    else
    {
        SDL_Surface *formatted = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_RGBA8888, NULL);

        if (formatted == NULL)
            std::cout << "Error creating formatted surface" << std::endl;
        else
        {
            t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, formatted->w, formatted->h);

            if (t == NULL)
                std::cout << "Error creating texture" << std::endl;
            else
            {
                width = formatted->w;
                height = formatted->h;

                pixel_format = *formatted->format;

                // Enable blending on texture
                SDL_SetTextureBlendMode( t, SDL_BLENDMODE_BLEND );

                if (c_r != -1 && c_g != -1 && c_b != -1)
                {
                    SDL_LockTexture(t, NULL, &pixels, &pitch);
                    
                    memcpy(pixels, formatted->pixels, formatted->pitch * formatted->h);

                    //Get pixel data in editable format
                    Uint32* _pixels = (Uint32*)pixels;
                    int pixel_count = ( pitch / 4 ) * height;

                    //Map colors                
                    Uint32 colorKey = SDL_MapRGB(formatted->format, c_r, c_g, c_b);
                    Uint32 transparent = SDL_MapRGBA(formatted->format, c_r, c_g, c_b, 0x00 );

                    keycolor = transparent;

                    //Color key pixels
                    for( int i = 0; i < pixel_count; ++i )
                    {
                        if( _pixels[i] == colorKey )
                        {
                            _pixels[i] = transparent;
                        }
                    }

                    SDL_UnlockTexture(t);
                }

                pixels = NULL;
            }

            SDL_FreeSurface(formatted);
        }

        SDL_FreeSurface(loaded);
    }

    texture = t;

    if (texture != NULL)
        SDL_QueryTexture(texture, &format, NULL, NULL, NULL);

    return texture != NULL;
}

void Texture::render(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip)
{
    SDL_Rect render_rect = { x, y, width, height };

    if (clip != NULL)
    {
        render_rect.w = clip->w;
        render_rect.h = clip->h;
    }


    SDL_RenderCopyEx(renderer, texture, clip, &render_rect, angle, NULL, flip_mode);
}

bool Texture::lock()
{
    bool success = true;

    if (pixels != NULL)
    {
        success = false;
        std::cout << "Texture already locked" << std::endl;
    }
    else
    {
        if (SDL_LockTexture(texture, NULL, &pixels, &pitch))
        {
            success = false;
            std::cout << "Error locking texture:" << std::endl;
            std::cout << SDL_GetError() << std::endl;
        }
    }

    return success;
}

bool Texture::unlock()
{
    bool success = true;

    if (pixels == NULL)
    {
        success = false;
        std::cout << "Texture isn't locked!" << std::endl;
    }
    else
    {
        SDL_UnlockTexture(texture);
        pixels = NULL;
        pitch = 0;
    }

    return success;
}
