#include <GL/glew.h>
#include <iostream>
#include <cstdlib>

#include "Room1.h"
#include "Constants.h"
#include "RoomNumber.h"
#include "Cube.h"

const float Room1::faceCenter[NUM_FACES * DIMENSION] = {
                                0, 0, 1.001f,
                                0, 0, -1.001f,
                                -1.001f, 0, 0,
                                1.001f, 0, 0,
                                0, 1.001f, 0,
                                0, -1.001f, 0
};

const float Room1::vertexPattern[NUM_FACES * NUM_VERTICES * DIMENSION] = {// Front, back, left, right, top, bot
                                        -1, 1, 1, 1, 1, 1, 1, -1, 1, -1, -1, 1,
                                        -1, 1, -1, 1, 1, -1, 1, -1, -1, -1, -1, -1,
                                        -1, 1, 1, -1, 1, -1, -1, -1, -1, -1, -1, 1,
                                        1, 1, 1, 1, 1, -1, 1, -1, -1, 1, -1, 1,
                                        -1, 1, 1, -1, 1, -1, 1, 1, -1, 1, 1, 1,
                                        -1, -1, 1, -1, -1, -1, 1, -1, -1, 1, -1, 1,
};

const int Room1::edgeCycle[DIMENSION][EDGE_CYCLE_LENGTH] = {
                                            {0, 5, 1, 4},
                                            {0, 3, 1, 2},
                                            {2, 5, 3, 4}
};

Room1::Room1() : Room1(0, 0, 0)
{
}

Room1::Room1(float x, float y, float z) : Cube(x, y, z)
{
    for (int i = 0; i < NUM_FACES; i++)
    {
        roomNumbers[i] = RoomNumber();
        roomNumbers[i].plane = i;
        setRoomNumberCenter(i);
        roomNumbers[i].numVisible = false;
        roomNumbers[i].number = i;
    }
    toggleNumberVisibility(true);
    setVertices();
}

Room1::Room1(float x, float y, float z, int num) : Room1{ x, y, z }
{
    for (int i = 0; i < NUM_FACES; i++)
    {
        setRoomNumber(i, num);
    }
}

void Room1::setCenter(float newCenter[DIMENSION])
{
    Cube::setCenter(newCenter);
    setVertices();
    for (int i = 0; i < NUM_FACES; i++)
    {
        setRoomNumberCenter(i);
    }
}

void Room1::setCol(const float* col)
{
    for (int i = 0; i < NUM_FACES; i++)
    {
        setCol(i, col);
    }
}

void Room1::setCol(int face, const float col[Constants::COL_SIZE])
{
    roomNumbers[face].setCol(col);
}

void Room1::setVertices()
{
    for (int i = 0; i < NUM_FACES * NUM_VERTICES * DIMENSION; i++)
    {
        vertices[i] = center[i % DIMENSION] + vertexPattern[i] * Constants::CUBE_SIDE_LENGTH / 2;
    }
}

void Room1::setRoomNumberCenter(int num)
{
    roomNumbers[num].setCenter(center[0] + faceCenter[num * DIMENSION] * Constants::CUBE_SIDE_LENGTH / 2, 
    center[1] + faceCenter[num * DIMENSION + 1] * Constants::CUBE_SIDE_LENGTH / 2,
    center[2] + faceCenter[num * DIMENSION + 2] * Constants::CUBE_SIDE_LENGTH / 2);
}

void Room1::setRoomNumber(int room, int num)
{
    for (int i = 0; i < NUM_FACES; i++)
    {
        roomNumbers[i].number = num;
    }
}

void Room1::setRoomCol(int room, const float* col)
{
    setCol(col);
}

void Room1::toggleNumberVisibility(bool toggle)
{
    roomNumbers[0].numVisible = toggle;
}

void Room1::draw()
{
    rotateShift(true);
    float background[NUM_FACES * NUM_VERTICES * Constants::COL_SIZE] = {};
    for (int i = 0; i < NUM_FACES * NUM_VERTICES; i++)
    {
        memcpy(&background[i * Constants::COL_SIZE], &Constants::colBlack, sizeof(Constants::colBlack));
    }
    glVertexPointer(DIMENSION, GL_FLOAT, 0, vertices);
    glColorPointer(Constants::COL_SIZE, GL_FLOAT, 0, background);
    glDrawArrays(GL_QUADS, 0, NUM_FACES * NUM_VERTICES);

    for (int face = 0; face < NUM_FACES; face++)
    {
        roomNumbers[face].draw();
    }
    rotateShift(false);
}

void Room1::rotateShift(bool cw)
{
    if (!inRotation) { return; }
    glTranslatef(axis[0], axis[1], axis[2]);
    glRotatef((cw ? -1 : 1) * currAngle, (float)(rotationType == 0 ? 1 : 0),
        (float)(rotationType == 1 ? 1 : 0), (float)(rotationType == 2 ? 1 : 0));
    glTranslatef(-axis[0], -axis[1], -axis[2]);
}

void Room1::rotateEdge(int type, int side)
{
    int cycle[EDGE_CYCLE_LENGTH] = {};
    memcpy(cycle, edgeCycle[type], sizeof(cycle));
    float swapCenter[DIMENSION] = {};
    for (int i = 1; i < EDGE_CYCLE_LENGTH; i++)
    {
        std::swap(roomNumbers[cycle[0]], roomNumbers[cycle[i]]);
        std::swap(roomNumbers[cycle[0]].plane, roomNumbers[cycle[i]].plane);
        memcpy(swapCenter, roomNumbers[cycle[0]].center, sizeof(swapCenter));
        memcpy(roomNumbers[cycle[0]].center, roomNumbers[cycle[i]].center, sizeof(swapCenter));
        memcpy(roomNumbers[cycle[i]].center, swapCenter, sizeof(swapCenter));
    }
}

bool Room1::hasRoomNumber(int num)
{
    return roomNumbers[0].number == num;
}