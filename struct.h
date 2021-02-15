//
// Created by thgir on 11/02/2021.
//

#ifndef MAZE_GENERATOR_STRUCT_H
#define MAZE_GENERATOR_STRUCT_H

#include <stdbool.h>
#define LINE 50
#define COLUMN 50
typedef enum {
    TOP, RIGHT, BOTTOM, LEFT
} boxWall_e;

typedef struct{
    bool isWallSolid[4];
    bool hasBeenVisited;
} box_t;

typedef struct {
    box_t box[LINE][COLUMN];
} maze_t;

typedef struct {
    bool isBoxEmpty[4];
    int nbrOfEmptyBoxes;
} freeBox_t;

typedef struct boxInStack_t boxInStack_t;
struct boxInStack_t{
    box_t * box;
    boxInStack_t * previous;
    int position[2];
};

typedef struct {
    int position[2];
    int distanceFromEntrance;
    boxWall_e exitWall;
}exitBox_t;
#endif //MAZE_GENERATOR_STRUCT_H
