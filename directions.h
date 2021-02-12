//
// Created by thgir on 11/02/2021.
//

#ifndef MAZE_GENERATOR_DIRECTIONS_H
#define MAZE_GENERATOR_DIRECTIONS_H

#include "struct.h"

int get_potential_directions(freeBox_t* freeBox, const maze_t maze, const int line, const int column);
void update_position(int* currentPosition, const boxWall_e direction);
boxWall_e get_direction(const int nbrOfPotential, const bool* arrayOfPotential);

#endif //MAZE_GENERATOR_DIRECTIONS_H
