#include "maze_gui.h"
#include <stdio.h>



int display_gui_maze(maze_t maze)
{
    /*Initialisation SDL*/
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1){
       printf("window initialization failure due to : %s",SDL_GetError());  exit(EXIT_FAILURE);
    }if (SDL_Init(TTF_Init()) == -1){
        printf("window initialization failure due to : %s",SDL_GetError());  exit(EXIT_FAILURE);
    }

    SDL_Window* window = NULL;
    window = SDL_CreateWindow("maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_MAXIMIZED);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    int win_width, win_height;
    SDL_GetWindowSize(window, &win_width, &win_height);

    int box_width = ((win_width-100) / COLUMN) < 40 ? ((win_width-100) / COLUMN) + 1 : 40;
    int box_height = ((win_height-100) / LINE) ? ((win_height-100) / LINE) + 1 : 40;
    if (box_width > box_height){
        box_width = box_height;
    } else {
        box_height = box_width;
    }
    for (int i = 0; i < LINE; ++i){
        for (int j = 0; j < COLUMN; ++j){
            display_box(maze.box[i][j], 50 + box_width*j, 50 + box_height*i, box_width, box_height, renderer);
        }
    }
    SDL_RenderPresent(renderer);

    SDL_Event event;
    SDL_bool run = SDL_TRUE;
    while(run){
        while(SDL_PollEvent(&event)){
            switch(event.type)
            {
            case SDL_WINDOWEVENT:
                if(event.window.event == SDL_WINDOWEVENT_CLOSE){
                    run = SDL_FALSE;
                }
                break;
            }
        }
    }
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}


void display_box(box_t box, const int x, const int y, const int box_width, const int box_height, SDL_Renderer* renderer)
{
    int x1, x2, y1, y2;
    for (int i = 0; i < 4; ++i){
        if (box.isWallSolid[i]){
            switch(i)
            {
            case TOP:
                x1 = x;
                y1 = y;
                x2 = x1 + box_width;
                y2 = y;
                break;
            case RIGHT:
                x1 = x + box_width;
                y1 = y;
                x2 = x1;
                y2 = y + box_height;
                break;
            case BOTTOM:
                x1 = x + box_width;
                y1 = y + box_width;
                x2 = x;
                y2 = y1;
                break;
            case LEFT:
                x1 = x;
                y1 = y + box_width;
                x2 = x;
                y2 = y;
                break;
            }
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }
    }
}
