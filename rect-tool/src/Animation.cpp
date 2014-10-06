
#include "Animation.h"

std::vector<Rect> analyze_spritesheet(Texture *t, int sprite_start, int sprite_end)
{
    std::vector<Rect> rects;
    
    // texture info
    int x = 0, y = 0;
    Uint32 **pixel_2d = new Uint32*[t->get_height()];
    Uint32 keycolor = t->get_colorkey();

    // Algorithm:
    // -- Before:
    //  - Set the RenderTarget to the texture to be scanned
    //  - Read the pixels of the texture
    //  
    // 1. Look at pixels from left to right
    //    While looking left to right, look down
    // 2. If you find a pixel other than colorkey,
    //    then call that the start x of a rect
    // 3. Now search from that x on vertically until
    //    you find a complete line of just the colorkey.
    //    The line before that will be the end x
    // 4. Now search from top to bottom and left to right
    //    within the x and end x. The first line that has a 
    //    color other than the colorkey is the start y
    // 5. Search from left to right and start y to bottom within
    //    x and start x. Look for a line that contains only the colorkey.
    //    The line before that is the end y.
    // 6. Repeat steps 1-5 until the entire x and y of the document
    //    has been scanned.

    // Lock the texture to get the pitch
    if (!t->lock())
    {
        std::cout << "Couldn't lock pixels" << std::endl;
    }
    else
    {
        //std::cout << "converted pixels to Uint32" << std::endl;

        try 
        {
            for (int i = 0; i < t->get_height(); i++)
                pixel_2d[i] = new Uint32[t->get_width()];

            std::cout << "allocated memory for pixel_2d" << std::endl;
            int pixel_count = 0;

            Uint32 *pixels = (Uint32*) t->get_pixels();

            // put pixels from 1d array into 2d array
            for (int i = 0; i < t->get_height(); i++)
            {
                for (int j = 0; j < t->get_width(); j++)
                {
                    pixel_2d[i][j] = pixels[pixel_count];
                    //std::cout << i << " * " << w << " + " << j << " = " << i * w + j << std::endl;
                    pixel_count++;
                }
            }

            std::cout << "moved pixels over to 2d array" << std::endl;

            std::vector<Rect> row;

            while (y < t->get_height())
            {
                int start_x = 0, end_x = 0;
                int start_y = 0, end_y = 0;

                // scan pixels left to right and top to bottom
                // to get start x
                for (int i = x; i < t->get_width(); i++)
                {
                    bool found = false;
                    for (int j = y; j < t->get_height(); j++)
                    {
                        if (pixel_2d[j][i] != keycolor)
                        {
                            //std::cout << "keycolor: " << keycolor << ' ' << pixel_2d[j][i] << std::endl;
                            start_x = i;
                            found = true;
                            break;
                        }
                    }

                    if (found) break;
                }

                // scan from start_x to w look for line of all keycolor
                for (int i = start_x; i < t->get_width(); i++)
                {
                    bool key_line_found = true;

                    for (int j = y; j < t->get_height(); j++)
                    {
                        if (pixel_2d[j][i] != keycolor)
                            key_line_found = false;
                    }

                    if (key_line_found || i == t->get_width() - 1)
                    {
                        end_x = i;
                        break;
                    }
                }

                // scan top to bottom and left to right, look for first pixel
                // that isn't keycolor
                for (int i = y; i < t->get_height(); i++)
                {
                    bool found = false; 

                    for (int j = start_x; j < end_x; j++)
                    {
                        if (pixel_2d[i][j] != keycolor)
                        {
                            found = true; 
                            start_y = i;
                            break;
                        }
                    }

                    if (found) break;
                }

                // scan top to bottom and left and right, look for a line of
                // all keycolor pixels
                for (int i = start_y; i < t->get_height(); i++)
                {
                    bool key_line_found = true;

                    for (int j = start_x; j < end_x; j++)
                    {
                        if (pixel_2d[i][j] != keycolor)
                        {
                            key_line_found = false;
                            break;
                        }
                    }

                    if (key_line_found || i == t->get_height() - 1)
                    {
                        end_y = i;
                        break;
                    }
                }

                //std::cout << "start_x: " << start_x << " end_x: " << end_x << std::endl;
                //std::cout << "start_y: " << start_y << " end_y: " << end_y << std::endl;

                if (end_x - start_x < 0)
                {
                    std::cout << x << ' ' << y << std::endl;
                    std::cout << "current x: " << x << " current y: " << y << std::endl;

                    std::cout << "start_x: " << start_x << " end_x: " << end_x << std::endl;
                    std::cout << "start_y: " << start_y << " end_y: " << end_y << std::endl;

                    break;
                }

                // no sprite was found
                if ((start_x == 0 && start_y == 0 && end_x == 0 && end_x == 0))
                {

                    if (row.size() == 0)
                        break;
                    else
                    {
                        int max = 0;

                        for (int i = 0; i < row.size(); i++)
                        {
                            if (max < row[i].y + row[i].h) max = row[i].y + row[i].h;
                        }

                        for (int i = 0; i < row.size(); i++)
                            rects.push_back(row[i]);

                        row.clear();

                        y = max + 1;
                        x = 0;
                    }
                }
                else 
                {
                    Rect r(start_x, start_y, end_x - start_x, end_y - start_y);
                    row.push_back(r);

                    if (end_x >= t->get_width() - 1)
                    {
                        int max = 0;

                        for (int i = 0; i < row.size(); i++)
                        {
                            if (max < row[i].y + row[i].h) max = row[i].y + row[i].h;
                        }

                        for (int i = 0; i < row.size(); i++)
                            rects.push_back(row[i]);

                        row.clear();

                        y = max + 1;
                        x = 0;
                    }
                    
                    else if (end_x + 1 < t->get_width() && end_x != 0)
                    {
                        x = end_x + 1;
                    }

                    std::cout << r << std::endl;
                    std::cout << "current x: " << x << " current y: " << y << std::endl;
                }

            }

            //std::cout << "pixel_count: " << pixel_count << std::endl;
            std::cout << "calculated sprites" << std::endl;
        }
        catch (std::bad_alloc &e)
        {
            std::cout << e.what() << std::endl;
        }

        //free(pixels);
    }

    t->unlock();

    for (int i = 0; i < t->get_height(); i++)
        delete [] pixel_2d[i];

    delete [] pixel_2d;

    if (sprite_start >= 0 && sprite_end >= 0)
    {
        // erase before start
        for (int i = 0; i < sprite_start; i++)
        {
            rects.erase(rects.begin() + i);
        }

        // erase after end
        for (int i = rects.size() - 1; i > sprite_end; i--)
        {
            //std::cout << "erassing " << i << ", ";
            rects.erase(rects.begin() + i);
        }
        std::cout << std::endl;
    }
    else if (sprite_start >= 0 && sprite_end <= -1)
    {
        // erase before start
        for (int i = 0; i < sprite_start; i++)
        {
            rects.erase(rects.begin() + i);
        }
    }
    else if (sprite_start <= -1 && sprite_end >= 0)
    {
        // erase after end
        for (int i = rects.size() - 1; i > sprite_end; i--)
        {
            //std::cout << "erassing " << i << ", ";
            rects.erase(rects.begin() + i);
        }
        std::cout << std::endl;
    }

    std::cout << rects.size() << std::endl;

    for (int i = 0; i < rects.size(); i++)
        std::cout << rects[i].x << ' ' << rects[i].y << ' '
                  << rects[i].w << ' ' << rects[i].h << std::endl;

    return rects;
}

bool Animation::load_spritesheet(SDL_Renderer *r, std::string path, 
                    int c_r, int c_g, int c_b,
                    SDL_BlendMode b, Uint32 alpha)
{
    bool success = true;
    if (!spritesheet.load_from_file_streaming(r, path, c_r, c_g, c_b))
    {
        std::cout << "Error loading file at " << path << "\n";
        success = false;
    }
    else
    {
        spritesheet.set_blend_mode(b);
        spritesheet.set_alpha(alpha);
    }

    return success;
}

void Animation::update()
{
    if (timer.isStarted())
    {
        if (timer.getTicks() > update_ms)
        {
            timer.reset();
            current_clip++;

            if (current_clip >= num_clips)
            {
                play_count++;
                current_clip = 0;

                if (play_count == max_plays)
                    stop();
            }
        }
    }
}

void Animation::render(SDL_Renderer *renderer, int _x, int _y)
{
    SDL_Rect r = clips[current_clip].getSDL_Rect();
    spritesheet.render(renderer, _x, _y, &r);
}

void Animation::render(SDL_Renderer *renderer)
{
    SDL_Rect r = clips[current_clip].getSDL_Rect();
    spritesheet.render(renderer, x, y, &r);
}
