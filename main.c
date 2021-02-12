#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


#include "struct.h"
#include "maze_stack.h"
#include "directions.h"
#include "maze_gui.h"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

void display_maze(const maze_t maze);
void initialize_maze(maze_t* maze);


int main(int argc, char** argv) {

    srand(time(NULL));

    /** Variables declaration **/
    freeBox_t potentialDirections;
    int emptyBoxes = LINE * COLUMN - 1;
    boxWall_e direction;
    maze_t maze;
    int currentPosition[2] = {0, 0};

    initialize_maze(&maze);

    boxInStack_t* maze_stack = NULL;
    maze_stack = stack_push(&(maze.box[0][0]), NULL, currentPosition);
    maze_stack->box->hasBeenVisited = true;

    /** Maze generation **/
    /* The maze is generated using a depth first search algorithm */
    puts("Generating maze...");
    while (emptyBoxes){
        if (get_potential_directions(&potentialDirections,
                                     maze,
                                     currentPosition[0],
                                     currentPosition[1])) {
            /* If there is at least one box that has not been previously visited then one direction
            * is randomly chosen. The box corresponding to this direction is then marked as visited
            * and the position of the tracker is updated */
            direction = get_direction(potentialDirections.nbrOfEmptyBoxes, potentialDirections.isBoxEmpty);
            maze_stack->box->isWallSolid[direction] = false; //Open the wall by which the box is exited
            update_position(currentPosition, direction);
            maze_stack = stack_push(&(maze.box[currentPosition[0]][currentPosition[1]]), maze_stack, currentPosition);
            maze_stack->box->hasBeenVisited = true; //Set the new box as visited
            maze_stack->box->isWallSolid[(direction + 2) % 4] = false; //Open the wall by which the box is penetrated
            emptyBoxes--;
        } else {
            /* If there is no available box next to the current box then the stack is popped
            * until an empty box nearby is found */
            while (!get_potential_directions(&potentialDirections,
                                             maze,
                                             currentPosition[0],
                                             currentPosition[1])) {
                maze_stack = stack_pop(maze_stack);
                for (int i = 0; i < 2; ++i) {
                    currentPosition[i] = maze_stack->position[i];
                }
            }
        }
    }
    puts("Displaying maze...");
    display_gui_maze(maze);

    while (getchar() != '\n');
    return 0;
}


void initialize_maze(maze_t* maze)
{
    /*Begin by declaring all the maze boxes as empty from all sides*/
    for (int i = 0; i < LINE; ++i) {
        for (int j = 0; j < COLUMN; ++j) {
            for (int k = 0; k < 4; ++k) {
                maze->box[i][j].isWallSolid[k] = true;
                maze->box[i][j].hasBeenVisited = false;
            }
        }
    }
    //maze->box[0][0].isWallSolid[TOP] = false;

    puts("Grid initialized");
}
void display_maze(const maze_t maze)
{
    printf("\n");
    for (int i = 0; i < LINE; ++i) {
        /* Each box is displayed in three time : the top, the middle part, and then the bottom */

        /* Top */
        for (int j = 0; j < COLUMN; ++j) {
            if (maze.box[i][j].isWallSolid[TOP]) {
                printf("+----+");
            } else {
                printf("+    +");
            }
        }
        printf("\n");

        /* Middle part */
        for (int j = 0; j < COLUMN; ++j) {
            if (maze.box[i][j].isWallSolid[LEFT]) {
                printf("|    ");
            } else {
                printf("     ");
            }
            if (maze.box[i][j].isWallSolid[RIGHT]) {
                printf("|");
            } else {
                printf(" ");
            }
        }
        printf("\n");

        /* Bottom */
        for (int j = 0; j < COLUMN; ++j) {
            if (maze.box[i][j].isWallSolid[BOTTOM]) {
                printf("+----+");
            } else {
                printf("+    +");
            }
        }
        printf("\n");
    }
}

#pragma clang diagnostic pop
