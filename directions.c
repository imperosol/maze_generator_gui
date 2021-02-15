//
// Created by thgir on 11/02/2021.
//

#include "directions.h"
#include <stdlib.h>

void update_position(int* currentPosition, const boxWall_e direction)
{
    if (direction % 2) {
        /* The direction is either LEFT or RIGHT */
        currentPosition[1] -= direction - 2;
    } else {
        /* The direction is either TOP or BOTTOM */
        currentPosition[0] += direction - 1;
    }
}
boxWall_e get_direction(const int nbrOfPotential, const bool* arrayOfPotential)
{
    int randomDirection = rand() % nbrOfPotential;
    for (int i = 0; i < 4; ++i) {
        if (arrayOfPotential[i]) {
            if (!randomDirection) {
                return i;
            } else {
                randomDirection--;
            }
        }
    }
    return 0;
}
int get_potential_directions(freeBox_t* freeBox, const maze_t maze, const int line, const int column)
{
    int arrayInd = 0;
    freeBox->nbrOfEmptyBoxes = 0;
    for (int i = -1; i < 2; i += 2) {
        if (line + i < 0 || line + i >= LINE) {
            /* The box is out of the maze */
            freeBox->isBoxEmpty[arrayInd] = false;
        } else if (maze.box[line+i][column].hasBeenVisited) {
            freeBox->isBoxEmpty[arrayInd] = false;
        } else {
            freeBox->isBoxEmpty[arrayInd] = true;
            freeBox->nbrOfEmptyBoxes++;
        }
        arrayInd++;

        if (column - i < 0 || column - i >= COLUMN) {
            /* The box is out of the maze */
            freeBox->isBoxEmpty[arrayInd] = false;
        } else if (maze.box[line][column-i].hasBeenVisited) {
            freeBox->isBoxEmpty[arrayInd] = false;
        } else {
            freeBox->isBoxEmpty[arrayInd] = true;
            freeBox->nbrOfEmptyBoxes++;
        }
        arrayInd++;
    }
    for (int i = 0; i < 4; ++i) {
        if (freeBox->isBoxEmpty[i]) {
            return 1;
        }
    }
    return 0;
}
