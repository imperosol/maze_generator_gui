#include "exit_creation.h"

void update_exit_box(exitBox_t* exitBox, const int distanceFromEntrance, const int* position, const int direction)
{
    exitBox->distanceFromEntrance = distanceFromEntrance;
    for (int i = 0; i < 2; ++i) {
        exitBox->position[i] = position[i];
    }
    exitBox->exitWall = direction;
}
int is_adjacent_to_maze_wall(boxInStack_t* box)
{
    if (box->position[0] == 0) {
        return 0;
    } else if (box->position[1] == LINE - 1){
        return 1;
    } else if (box->position[0] == COLUMN - 1) {
        return 2;
    } else if (box->position[1] == 0) {
        return 3;
    } else {
        return -1;
    }
}
