#include <iostream>

#include "Room27.h"
#include "Constants.h"
#include "Room1.h"

const int Room27::cubePattern[CUBE_NUM][DIMENSION] = {//top, mid, bot -> left, mid, right
                                    {-1, 1, -1}, { 0, 1, -1 }, { 1, 1, -1 },
                                    { -1, 1, 0 }, { 0, 1, 0 }, { 1, 1, 0 },
                                    { -1, 1, 1 }, { 0, 1, 1 }, { 1, 1, 1 },
                                    { -1, 0, -1 }, { 0, 0, -1 }, { 1, 0, -1 },
                                    { -1, 0, 0 }, {0, 0, 0}, { 1, 0, 0 },
                                    { -1, 0, 1 }, { 0, 0, 1 }, { 1, 0, 1 },
                                    { -1, -1, -1 }, { 0, -1, -1 }, { 1, -1, -1 },
                                    { -1, -1, 0 }, { 0, -1, 0 }, { 1, -1, 0 },
                                    { -1, -1, 1 }, { 0, -1, 1 }, { 1, -1, 1 },
};

const int Room27::sideOffsetRef[DIMENSION] = { 1, 9, 3 };

const int Room27::faceRef[DIMENSION][DIMENSION * DIMENSION] = {
                            { 0, 3, 6, 9, 12, 15, 18, 21, 24 },
                            { 0, 1, 2, 3, 4, 5, 6, 7, 8 },
                            { 0, 1, 2, 9, 10, 11, 18, 19, 20 }
};

const int Room27::faceCycleRef[DIMENSION][FACE_CYCLE_LENGTH][FACE_CYCLE_LENGTH / 2] = {
                { {0, 3}, {6, 15}, {24, 21}, {18, 9}},
                { {0, 1}, {6, 3}, {8, 7}, {2, 5} },
                { {0, 1}, {18, 9}, {20, 19}, {2, 11} }
};

const int Room27::focusCycleRef[CUBE_NUM][DIMENSION] = {//next position, type, side
                                    { 6, 0, 0 }, // 0
                                    { 7, 0, 1 },
                                    { 8, 0, 2 },
                                    { 15, 0, 0 }, //3
                                    { 16, 0, 1 },
                                    { 17, 0, 2 },
                                    { 24, 0, 0 }, //6
                                    { 15, 2, 2 },
                                    { 6, 2, 2 },
                                    { 3, 0, 0}, //9
                                    { 4, 0, 1},
                                    { 5, 0, 2},
                                    { 16, 1, 1}, //12
                                    { -1, -1, -1},
                                    { 10, 1, 1},
                                    { 25, 2, 2}, //15
                                    { -1, -1, -1},
                                    { 7, 2, 2},
                                    { 0, 0, 0 }, //18
                                    { 1, 0, 1 },
                                    { 2, 0, 2 },
                                    { 9, 0, 0 }, //21
                                    { 10, 0, 1 },
                                    { 11, 0, 2 },
                                    { -1, -1, -1}, //24
                                    { -1, -1, -1},
                                    { 8, 2, 2}
};

Room27::Room27() : Room27(0, 0, 0)
{
}

Room27::Room27(float x, float y, float z) : Room27(x, y, z, 1)
{
}

Room27::Room27(float x, float y, float z, int roomNum) : Cube(x, y, z)
{
    openAmount *= 2;
    for (int i = 0; i < CUBE_NUM; i++)
    {
        cubes[i] = new Room1(Constants::CUBE_SIDE_LENGTH * cubePattern[i][0] + center[0], Constants::CUBE_SIDE_LENGTH * cubePattern[i][1] + center[1],
            Constants::CUBE_SIDE_LENGTH * cubePattern[i][2] + center[2], i);
        cubes[i]->setOpenDir(cubePattern[i]);
    }
    setRoomNumber(-1, roomNum);

    int type = -1, offset = -1;
    for (int i = 0; i < Constants::COL_NUM; i++)
    {
        switch (i)
        {
        case 0:
            type = 2;
            offset = 2 * sideOffsetRef[type];
            break;
        case 1:
            type = 2;
            offset = 0 * sideOffsetRef[type];
            break;
        case 2:
            type = 0;
            offset = 0 * sideOffsetRef[type];
            break;
        case 3:
            type = 0;
            offset = 2 * sideOffsetRef[type];
            break;
        case 4:
            type = 1;
            offset = 0 * sideOffsetRef[type];
            break;
        case 5:
            type = 1;
            offset = 2 * sideOffsetRef[type];
            break;
        default:
            break;
        }
        for (int j = 0; j < DIMENSION * DIMENSION; j++)
        {
            cubes[faceRef[type][j] + offset]->setCol(i, Constants::colWhite);
        }
    }
}

void Room27::setCenter(float newCenter[DIMENSION])
{
    Cube::setCenter(newCenter);
    float newCubeCenter[DIMENSION] = {};
    for (int i = 0; i < CUBE_NUM; i++)
    {
        newCubeCenter[0] = Constants::CUBE_SIDE_LENGTH * cubePattern[i][0] + center[0];
        newCubeCenter[1] = Constants::CUBE_SIDE_LENGTH * cubePattern[i][1] + center[1];
        newCubeCenter[2] = Constants::CUBE_SIDE_LENGTH * cubePattern[i][2] + center[2];
        cubes[i]->setCenter(newCubeCenter);
    }
}

void Room27::setRoomNumber(int room, int num)
{
    for (int i = 0; i < CUBE_NUM; i++)
    {
        if (i == (CUBE_NUM - 1) / 2) {continue;}
        cubes[i]->setRoomNumber(0, num++);
    }
}

void Room27::setBlankRoomNumber(int room)
{
    for (int i = 0; i < CUBE_NUM; i++)
    {
        cubes[i]->setBlankRoomNumber(0);
    }
}

void Room27::setRoomCol(int room, const float* col)
{
    cubes[findRoom(room)]->setRoomCol(room, col);
}

void Room27::draw()
{
    for (int i = 0; i < CUBE_NUM; i++)
    {
        cubes[i]->draw();
    }
}

void Room27::update()
{
    for (int i = 0; i < CUBE_NUM; i++)
    {
        cubes[i]->update();
    }
    Cube::update();
    if (!inRotation && !inCooldown && !inTranslation && isOpen && focusRoomLoc != -1)
    {
        int cycle[DIMENSION] = {};
        memcpy(cycle, focusCycleRef[focusRoomLoc], sizeof(cycle));
        focusRoomLoc = cycle[0];
        if (focusRoomLoc != -1)
        {
            rotateStart(cycle[1], cycle[2], true, center, false);
        }
        else
        {
            focusRoomLoc = -1;
            toggleOpen(false, parentOpen);
        }
    }
}

void Room27::rotateStart(int type, int side, bool cw, float* newAxis, bool skipAnim)
{
    Cube::rotateStart(type, side, cw, newAxis, skipAnim);
    const int* cycle = faceRef[type];
    int start = side == 3 ? 0 : side;
    int end = side == 3? side : start + 1;
    for (int i = 0; i < DIMENSION * DIMENSION; i++)
    {
        for (int j = start; j < end; j++)
        {
            cubes[cycle[i] + j * sideOffsetRef[type]]->rotateStart(type, j, cw, newAxis, skipAnim);
        }
    }
}

void Room27::rotateStart(int num, int type, int side, bool cw, bool skipAnim)
{
    rotateStart(type, side, cw, center, skipAnim);
}

void Room27::rotateEdge(int type, int side)
{
    int cycle[FACE_CYCLE_LENGTH][FACE_CYCLE_LENGTH / 2] = {};
    memcpy(cycle, faceCycleRef[type], sizeof(cycle));
    int start = side == 3 ? 0 : side;
    int end = side == 3? side : start + 1;
    int offset;
    float swap[DIMENSION] = {};
    const int* cycle2 = faceRef[type];
    for (int k = start; k < end; k++)
    {          
        offset = k * sideOffsetRef[type];
        for (int i = 1; i < FACE_CYCLE_LENGTH; i++)
        {
            for (int j = 0; j < FACE_CYCLE_LENGTH / 2; j++)
            {
                std::swap(cubes[cycle[0][j] + offset], cubes[cycle[i][j] + offset]);
                memcpy(swap, cubes[cycle[0][j] + offset]->getCenter(), sizeof(swap));
                cubes[cycle[0][j] + offset]->setCenter(cubes[cycle[i][j] + offset]->getCenter());
                cubes[cycle[i][j] + offset]->setCenter(swap);
            }
        }
        for (int i = 0; i < DIMENSION * DIMENSION; i++)
        {
            cubes[cycle2[i] + offset]->setOpenDir(cubePattern[cycle2[i] + offset]);
        }
    }
}

bool Room27::hasRoomNumber(int room)
{
    return findRoom(room) != -1;
}

int Room27::findRoom(int room)
{
    for (int i = 0; i < CUBE_NUM; i++)
    {
        if (cubes[i]->hasRoomNumber(room))
        {
            return i;
        }
    }
    return -1;
}

void Room27::toggleOpen(bool toggleSelf, bool toggleParent)
{
    Cube::toggleOpen(toggleSelf, toggleParent);
    if (!toggleSelf)
    {
        for (int i = 0; i < CUBE_NUM; i++)
        {
            cubes[i]->toggleOpen(true, isOpen);
        }
    }
}
