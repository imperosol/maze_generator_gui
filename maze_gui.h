//
// Created by thgir on 12/02/2021.
//

#ifndef MAZE_GUI_H
#define MAZE_GUI_H
#include "struct.h"
#include <SDL2/SDL.h>

int display_gui_maze(maze_t maze);

void display_box(box_t box, const int x, const int y, const int box_width, const int box_height, SDL_Renderer* renderer);

#endif // MAZE_GUI_H
