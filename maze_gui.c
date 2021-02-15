#include "maze_gui.h"
#include "directions.h"
#include <stdio.h>
#include <windows.h>

void resolve_maze(maze_t maze, const int* exitPosition, SDL_Renderer* renderer, const int topPosition, const int leftPosition, int boxWidth, int boxHeight);
int* find_exit(const maze_t maze);
int get_potential_directions_resolve(freeBox_t* freeBox, const maze_t maze, const int line, const int column);

int display_gui_maze(maze_t maze)
{
    /*Initialisation SDL*/
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1){
       printf("window initialization failure due to : %s",SDL_GetError());  exit(EXIT_FAILURE);
    }

    SDL_Window* window = NULL;
    window = SDL_CreateWindow("maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    int win_width, win_height;
    int topPosition, leftPosition;
    SDL_GetWindowSize(window, &win_width, &win_height);

    int box_width = ((win_width-100) / COLUMN) < 40 ? ((win_width-100) / COLUMN) + 1 : 40;
    int box_height = ((win_height-100) / LINE) ? ((win_height-100) / LINE) + 1 : 40;
    if (box_width > box_height){
        box_width = box_height;
    } else {
        box_height = box_width;
    }
    topPosition = win_height/2 - (LINE*box_height)/2;
    leftPosition = win_width/2 - (COLUMN*box_width)/2;
    for (int i = 0; i < LINE; ++i){
        for (int j = 0; j < COLUMN; ++j){
            display_box(maze.box[i][j], leftPosition + box_width*j, topPosition + box_height*i, box_width, box_height, renderer);
        }
    }
    SDL_RenderPresent(renderer);
    int* exit_position = find_exit(maze);

    //Sleep(2000);
    SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
    resolve_maze(maze, exit_position, renderer, topPosition, leftPosition, box_width, box_height);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    free(exit_position);
    //puts("Labyrinthe fini");

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

int* find_exit(const maze_t maze)
{
    int* position = NULL;
    position = (int*) malloc (2*sizeof(int));
    if (!(maze.box[LINE-1][0].isWallSolid[BOTTOM])){
        position[0] = LINE - 1;
        position[1] = 0;
        return position;
    }
    for (int i = 1; i < COLUMN; i++){
        if (!(maze.box[0][i].isWallSolid[TOP])){
            position[0] = 0;
            position[1] = i;
            return position;
        } else if (!(maze.box[LINE-1][i].isWallSolid[BOTTOM])){
            position[0] = LINE - 1;
            position[1] = i;
            return position;
        }
    }
    for (int i = 0; i < LINE; i++){
        if (!(maze.box[i][0].isWallSolid[LEFT])){
            position[0] = i;
            position[1] = 0;
            return position;
        } else if (!(maze.box[i][COLUMN-1].isWallSolid[RIGHT])){
            position[0] = i;
            position[1] = COLUMN - 1;
            return position;
        }
    }
    position[0] = position[1] = 0;
    return position;
}

int get_potential_directions_resolve(freeBox_t* freeBox, const maze_t maze, const int line, const int column)
{
    for (int i = 0; i < 4; i++) {
        freeBox->isBoxEmpty[i] = false;
    }
    freeBox->nbrOfEmptyBoxes = 0;
    if (!(maze.box[line][column].isWallSolid[TOP])){
        if (line != 0){
            if (!maze.box[line-1][column].hasBeenVisited) {
                freeBox->isBoxEmpty[TOP] = true;
                freeBox->nbrOfEmptyBoxes++;
            }
        }
    }
    if (!(maze.box[line][column].isWallSolid[RIGHT])){
        if (!maze.box[line][column+1].hasBeenVisited){
            freeBox->isBoxEmpty[RIGHT] = true;
            freeBox->nbrOfEmptyBoxes++;
        }
    }
    if (!(maze.box[line][column].isWallSolid[BOTTOM])){
        if (!maze.box[line+1][column].hasBeenVisited){
            freeBox->isBoxEmpty[BOTTOM] = true;
            freeBox->nbrOfEmptyBoxes++;
        }
    }
    if (!(maze.box[line][column].isWallSolid[LEFT])){
        if (!maze.box[line][column-1].hasBeenVisited){
            freeBox->isBoxEmpty[LEFT] = true;
            freeBox->nbrOfEmptyBoxes++;
        }
    }
    if (freeBox->nbrOfEmptyBoxes) {
        return 1;
    } else {
        return 0;
    }
}



void resolve_maze(maze_t maze, const int* exitPosition, SDL_Renderer* renderer, const int topPosition, const int leftPosition, int boxWidth, int boxHeight)
{
    for (int i = 0; i < LINE; ++i) {
        for (int j = 0; j < COLUMN; ++j) {
           maze.box[i][j].hasBeenVisited = false;
        }
    }
    const int distanceFromWall = boxHeight - 20 > 0 ? 10 : 4;
    SDL_Rect rect;
    rect.h = boxHeight - 2 * distanceFromWall;
    rect.w = rect.h;
    rect.x = leftPosition + distanceFromWall + 1;
    rect.y = topPosition + distanceFromWall + 1;
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);

    boxWall_e direction;
    int currentPosition[2] = {0, 0};
    boxInStack_t* resolve_stack = NULL;
    resolve_stack = stack_push(resolve_stack, NULL, currentPosition);
    maze.box[0][0].hasBeenVisited = true;

    freeBox_t freeBox;

    while (!(currentPosition[0] == exitPosition[0] && currentPosition[1] == exitPosition[1])) {
        if(get_potential_directions_resolve(&freeBox,
                                            maze,
                                            currentPosition[0],
                                            currentPosition[1])) {
            direction = get_direction(freeBox.nbrOfEmptyBoxes, freeBox.isBoxEmpty);
            update_position(currentPosition, direction);

            resolve_stack = stack_push(&(maze.box[currentPosition[0]][currentPosition[1]]), resolve_stack, currentPosition);
            resolve_stack->box->hasBeenVisited = true; //Set the new box as visited
            switch(direction) {
            case TOP:
                rect.y -= boxHeight;
                break;
            case RIGHT:
                rect.x += boxWidth;
                break;
            case BOTTOM:
                rect.y += boxHeight;
                break;
            case LEFT:
                rect.x -= boxWidth;
                break;
            }
            SDL_RenderFillRect(renderer, &rect);
            SDL_RenderPresent(renderer);
            //Sleep(10);
        } else {
            /* If there is no available box next to the current box then the stack is popped
            * until an empty box nearby is found */
            SDL_SetRenderDrawColor(renderer, 50, 50, 200, 255);
            while (!get_potential_directions_resolve(&freeBox,
                                             maze,
                                             currentPosition[0],
                                             currentPosition[1])) {
                resolve_stack = stack_pop(resolve_stack);
                SDL_RenderFillRect(renderer, &rect);
                if (resolve_stack->position[0] > currentPosition[0]) {
                    rect.y += boxHeight;
                } else if (resolve_stack->position[0] < currentPosition[0]) {
                    rect.y -= boxHeight;
                } else if (resolve_stack->position[1] > currentPosition[1]) {
                    rect.x += boxWidth;
                } else if (resolve_stack->position[1] < currentPosition[1]) {
                    rect.x -= boxWidth;
                }
                for (int i = 0; i < 2; ++i) {
                    currentPosition[i] = resolve_stack->position[i];
                }
            }
            SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
        }
    }
    //puts("Maze finished");
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
