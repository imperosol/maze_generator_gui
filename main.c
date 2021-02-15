#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


#include "struct.h"
#include "maze_stack.h"
#include "directions.h"
#include "maze_gui.h"
#include "exit_creation.h"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

void initialize_maze(maze_t* maze);
maze_t create_maze();



int main(int argc, char** argv) {

    srand(time(NULL));
    maze_t maze = create_maze();


    //puts("Displaying maze...");
    display_gui_maze(maze);
    //while (getchar() != '\n');
    return 0;
}

maze_t create_maze()
{
    /** Variables declaration **/
    freeBox_t potentialDirections;
    int emptyBoxes = LINE * COLUMN - 1;
    boxWall_e direction;
    int adjacentWall;
    exitBox_t exitBox;
    int stackLength = 0;
    maze_t maze;
    int currentPosition[2] = {0, 0};

    initialize_maze(&maze);

    exitBox.distanceFromEntrance = 0;

    boxInStack_t* maze_stack = NULL;
    maze_stack = stack_push(&(maze.box[0][0]), NULL, currentPosition);
    maze_stack->box->hasBeenVisited = true;

    /** Maze generation **/
    /* The maze is generated using a depth first search algorithm */
    //puts("Generating maze...");
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
            stackLength++;
            /* If necessary, update the exitBox to put it as far as possible from the entrance box */
            if (exitBox.distanceFromEntrance < stackLength) {
                if ((adjacentWall = is_adjacent_to_maze_wall(maze_stack)) > -1) {
                    update_exit_box(&exitBox, stackLength, currentPosition, adjacentWall);
                }
            }
        } else {
            /* If there is no available box next to the current box then the stack is popped
            * until an empty box nearby is found */
            while (!get_potential_directions(&potentialDirections,
                                             maze,
                                             currentPosition[0],
                                             currentPosition[1])) {
                maze_stack = stack_pop(maze_stack);
                stackLength--;
                for (int i = 0; i < 2; ++i) {
                    currentPosition[i] = maze_stack->position[i];
                }
            }
        }
    }
    maze.box[exitBox.position[0]][exitBox.position[1]].isWallSolid[exitBox.exitWall] = false;
    free_stack(maze_stack);
    return maze;
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
    maze->box[0][0].isWallSolid[TOP] = false;

    //puts("Grid initialized");
}

#pragma clang diagnostic pop
