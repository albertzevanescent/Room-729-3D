#include <GL/glew.h>

#include <iostream>
#include "RoomNumber.h"
#include "Constants.h"
#include "Cube.h"

const float RoomNumber::SEGMENT_WIDTH = 1;
const float RoomNumber::SEGMENT_HEIGHT = 3 * SEGMENT_WIDTH;
const float RoomNumber::DIGIT_SPACING = 3 * (SEGMENT_WIDTH + SEGMENT_HEIGHT);
const float RoomNumber::OVERLAP_OFFSET = 0.002f;

const float RoomNumber::segmentCenters[SEGMENT_NUM * (Cube::DIMENSION - 1)] = {
                                        0, 2 * (SEGMENT_WIDTH + SEGMENT_HEIGHT),
                                        (SEGMENT_HEIGHT + SEGMENT_WIDTH), (SEGMENT_WIDTH + SEGMENT_HEIGHT),
                                        (SEGMENT_HEIGHT + SEGMENT_WIDTH), -(SEGMENT_WIDTH + SEGMENT_HEIGHT),
                                        0, -2 * (SEGMENT_WIDTH + SEGMENT_HEIGHT),
                                        -(SEGMENT_HEIGHT + SEGMENT_WIDTH), -(SEGMENT_WIDTH + SEGMENT_HEIGHT),
                                        -(SEGMENT_HEIGHT + SEGMENT_WIDTH), (SEGMENT_WIDTH + SEGMENT_HEIGHT),
                                        0, 0
};

const float RoomNumber::segmentVertices[SEGMENT_VERTICES_NUM * (Cube::DIMENSION - 1)] = {
                                    -SEGMENT_HEIGHT,   SEGMENT_WIDTH,    // left top
                                    SEGMENT_HEIGHT,   SEGMENT_WIDTH,    // right top
                                    SEGMENT_HEIGHT + SEGMENT_WIDTH,   0,    // right
                                    SEGMENT_HEIGHT,   -SEGMENT_WIDTH,    // right bottom
                                    -SEGMENT_HEIGHT,  -SEGMENT_WIDTH,    // left bottom
                                    -(SEGMENT_HEIGHT + SEGMENT_WIDTH),   0     // left
};

const bool RoomNumber::digitMapping[DIGIT_NUM][SEGMENT_NUM] = {
                    {true, true, true, true, true, true, false},
                    {false, true, true, false, false, false, false},
                    {true, true, false, true, true, false, true},
                    {true, true, true, true, false, false, true},
                    {false, true, true, false, false, true, true},
                    {true, false, true, true, false, true, true},
                    {true, false, true, true, true, true, true},
                    {true, true, true, false, false, false, false},
                    {true, true, true, true, true, true, true},
                    {true, true, true, true, false, true, true},
};

const int RoomNumber::planeMapping[PLANE_NUM][Cube::DIMENSION] = {
                    {1, 1, 0},
                    {1, 1, 0},
                    {0, 1, 1},
                    {0, 1, 1},
                    {1, 0, 1},
                    {1, 0, 1},
};

const int RoomNumber::vertexMapping[4][Cube::DIMENSION - 1] = {
                    {-1, 1},
                    {1, 1},
                    {1, -1},
                    {-1, -1}
};

RoomNumber::RoomNumber() : RoomNumber(0, 0, 0)
{
}

RoomNumber::RoomNumber(float x, float y, float z) : center{ x, y, z }
{
    setCol(Constants::colBlack);
}

void RoomNumber::setCol(const float* colour)
{
    memcpy(col, colour, sizeof(col));
}

void RoomNumber::setCenter(float newCenter[Cube::DIMENSION])
{
    memcpy(center, newCenter, sizeof(center));
}

void RoomNumber::setCenter(float x, float y, float z)
{
    center[0] = x;
    center[1] = y;
    center[2] = z;
}

void RoomNumber::draw()
{
    drawNumber(center);
}

void RoomNumber::drawNumber(float center[Cube::DIMENSION])
{
    drawRoom();
    if (number < 0 || !numVisible) {return;}

    int n = number;
    int length = 0;
    float digitCenter[Cube::DIMENSION] = {};
    memcpy(digitCenter, center, sizeof(digitCenter));
    while (n >= 0)
    {
        length++;
        n /= 10;
        if (n == 0) {break;}
    }
    n = number;
    digitCenter[0] = digitCenter[0] + (length - 1) * DIGIT_SPACING / 2;

    for (int i = 0; i < length; i++)
    {
        drawDigit(digitCenter, n % 10);
        digitCenter[0] -= DIGIT_SPACING;
        n /= 10;
    }
}

void RoomNumber::drawRoom()
{
    float vertices[4 * Cube::DIMENSION] = {};
    float colour[4 * Constants::COL_SIZE] = {};
    int vertexMap = 0;
    for (int vertex = 0; vertex < 4; vertex++)
    {
        vertexMap = 0;
        memcpy(&colour[vertex * Constants::COL_SIZE], col, sizeof(col));
        for (int coor = 0; coor < Cube::DIMENSION; coor++)
        {
            if (planeMapping[plane][coor] == 0)
            {
                vertices[vertex * Cube::DIMENSION + coor] = center[coor] + (signbit(center[coor]) ? -1 : 1)*OVERLAP_OFFSET;
            }
            else
            {
                vertices[vertex * Cube::DIMENSION + coor] = center[coor] + (ROOM_LENGTH / 2) * planeMapping[plane][coor] * vertexMapping[vertex][vertexMap++];
            }
        }
    }
    glVertexPointer(Cube::DIMENSION, GL_FLOAT, 0, vertices);
    glColorPointer(Constants::COL_SIZE, GL_FLOAT, 0, colour);
    glDrawArrays(GL_QUADS, 0, 4);
}

void RoomNumber::drawDigit(float center[Cube::DIMENSION], int digit)
{
    float segmentCenter[Cube::DIMENSION] = {};
    int segmentMap = 0;
    for (int i = 0; i < SEGMENT_NUM; i++)
    {
        if (!digitMapping[digit][i]) {continue;}
        segmentMap = 0;
        for (int j = 0; j < Cube::DIMENSION; j++)
        {
            if (planeMapping[plane][j] == 0)
            {
                segmentCenter[j] = center[j] + (signbit(center[j]) ? -1 : 1) * 2 * OVERLAP_OFFSET;
            }
            else
            {
                segmentCenter[j] = center[j] + SEGMENT_SCALING * segmentCenters[i * (Cube::DIMENSION - 1) + segmentMap++];
            }
        }
        drawSegment(segmentCenter, i != 0 && i != 3 && i != 6);
    }
}

void RoomNumber::drawSegment(float center[Cube::DIMENSION], bool rotate)
{
    float background[SEGMENT_VERTICES_NUM * Constants::COL_SIZE] = {};
    for (int i = 0; i < SEGMENT_VERTICES_NUM; i++)
    {
        memcpy(&background[i * Constants::COL_SIZE], &Constants::colBlack, sizeof(&Constants::colBlack));
    }

    float vertices[SEGMENT_VERTICES_NUM * Cube::DIMENSION] = {};
    int segmentMap = 0, swapA = 0, swapB = 0;
    for (int i = 0; i < SEGMENT_VERTICES_NUM; i++)
    {
        segmentMap = 0;
        for (int j = 0; j < Cube::DIMENSION; j++)
        {
            if (planeMapping[plane][j] == 0)
            {
                vertices[i * Cube::DIMENSION + j] = 0;
            }
            else
            {
                vertices[i * Cube::DIMENSION + j] = segmentVertices[i * (Cube::DIMENSION - 1) + segmentMap++] * SEGMENT_SCALING;
            }
        }
        if (rotate)
        {
            swapA = planeMapping[plane][0] == 1 ? 0 : 1;
            swapB = planeMapping[plane][2] == 1 ? 2 : 1;
            std::swap(vertices[i * Cube::DIMENSION + swapA], vertices[i * Cube::DIMENSION + swapB]);
        }
        for (int j = 0; j < Cube::DIMENSION; j++)
        {
            vertices[i * Cube::DIMENSION + j] += center[j];
        }
    }
    glVertexPointer(Cube::DIMENSION, GL_FLOAT, 0, vertices);
    glColorPointer(Constants::COL_SIZE, GL_FLOAT, 0, background);
    glDrawArrays(GL_POLYGON, 0, SEGMENT_VERTICES_NUM);
}

void RoomNumber::update()
{
}