#include "mainwindow.h"
#include <QApplication>
#include <SDL2/SDL.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindowFrom((void*)w.ptr_gfx->winId());
    SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(render, NULL);
    SDL_RenderPresent(render);

    a.exec();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}
