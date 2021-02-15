#ifndef EXIT_CREATION_H
#define EXIT_CREATION_H

#include "struct.h"

int is_adjacent_to_maze_wall(boxInStack_t* box);
void update_exit_box(exitBox_t* exitBox, const int distanceFromEntrance, const int* position, const int direction);

#endif // EXIT_CREATION_H
