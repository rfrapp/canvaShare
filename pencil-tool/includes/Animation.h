
#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include "Rect.h"
#include "Timer.h"
#include "Texture.h"

std::vector<Rect> analyze_spritesheet(Texture *t, int sprite_start = -1, int sprite_end = -1);

class Animation
{
private:
    Texture spritesheet;
    Timer timer;
    std::vector<Rect> clips;
    int x, y;
    int current_clip, num_clips;
    int max_plays, play_count;
    bool started, paused;
    Uint32 update_ms;

public:
    Animation(Uint32 update_time, int mp = 0) : 
            timer(1), current_clip(0), started(false), paused(false),
            update_ms(update_time), x(0), y(0), max_plays(mp), play_count(0)
    {
    }

    ~Animation() { destroy(); }

    void destroy() { spritesheet.destroy(); }

    bool load_spritesheet(SDL_Renderer *r, std::string path, 
                    int c_r = -1, int c_g = -1, int c_b = -1,
                    SDL_BlendMode b = SDL_BLENDMODE_BLEND, Uint32 alpha = 255);

    // ================ Timer Stuff =======================
    void set_clips(std::vector<Rect> _clips) { clips = _clips; num_clips = clips.size(); }

    int  get_max_plays() const { return max_plays; }
    void set_max_plays(int x)  { max_plays = x; }

    Uint32 get_update_time() const { return update_ms; }
    void set_update_time(Uint32 u) { update_ms = u; }

    void start() { timer.start(); started = true; paused = false; }
    void stop()  { timer.stop(); started = false; paused = false; play_count = 0; current_clip = 0; }
    void reset() { timer.reset(); current_clip = 0; started = false; paused = false; play_count = 0; }
    void pause() { timer.pause(); started = false; paused = true; }

    bool is_started() const { return started; }
    bool is_paused()  const { return paused;  }
    // ============== / Timer Stuff =======================

    // ================ Position Stuff =====================
    void setPos(int _x, int _y) { x = _x; y = _y; }
    int get_x() const { return x; }
    int get_y() const { return y; }
    void move(int dx, int dy)  { x += dx; y += dy; }
    // ============== / Position Stuff =====================

    void update();
    void render(SDL_Renderer *renderer, int _x, int _y);
    void render(SDL_Renderer *renderer);
    int  current_sprite_width() const  { return clips[current_clip].w; } 
    int  current_sprite_height() const { return clips[current_clip].h; }

     Texture* get_spritesheet() { return &spritesheet; }
};

#endif 
