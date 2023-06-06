#include <iostream>

#include "Room27.h"
#include "Room729.h"
#include "Constants.h"

Room729::Room729() : Room729(0, 0, 0)
{
}

Room729::Room729(float x, float y, float z)
{
    center[0] = x;
    center[1] = y;
    center[2] = z;
    openSelf = false;
    int roomNum = 1;

    for (int i = 0; i < CUBE_NUM; i++)
    {
        cubes[i] = new Room27((float)Constants::CUBE_SIDE_LENGTH * 3 * cubePattern[i][0], (float)Constants::CUBE_SIDE_LENGTH * 3 * cubePattern[i][1],
            (float)Constants::CUBE_SIDE_LENGTH * 3 * cubePattern[i][2]);
        if (i == (CUBE_NUM - 1) / 2)
        {
            cubes[i]->setBlankRoomNumber(0);
            continue;
        }
        cubes[i]->setRoomNumber(0, roomNum);
        roomNum += (CUBE_NUM - 1);
        cubes[i]->setOpenDir(cubePattern[i]);
    }
}

void Room729::draw()
{
    for (int i = 0; i < CUBE_NUM; i++)
    {
        cubes[i]->draw();
    }
}

void Room729::update()
{
    for (int i = 0; i < CUBE_NUM; i++)
    {
        cubes[i]->update();
    }
    Cube::update();

    if (awaitFocus && focusRoomLoc != -1 && !cubes[focusRoomLoc]->isOpen && !cubes[focusRoomLoc]->inMotion())
    {
        cubes[focusRoomLoc]->setRoomCol(focusRoom, Constants::colGreen);
        focusRoomLoc = -1;
        awaitFocus = false;
        toggleOpen(false, parentOpen);
    }
    else if (!inMotion() && isOpen && focusRoomLoc != -1 && !cubes[focusRoomLoc]->isOpen)
    {
        int cycle[DIMENSION] = {};
        memcpy(cycle, focusCycleRef[focusRoomLoc], sizeof(cycle));
        if (cycle[0] != -1)
        {
            focusRoomLoc = cycle[0];
            rotateStart(cycle[1], cycle[2], true, center, false);
        }
        else if (!awaitFocus)
        {
            awaitFocus = true;
            cubes[focusRoomLoc]->enterFocusRoom(focusRoom);
        }
    }
}

void Room729::rotateStart(int type, int side, bool cw, float* newAxis, bool skipAnim)
{
    const int* cycle = faceRef[type];
    for (int i = 0; i < DIMENSION * DIMENSION; i++)
    {
        cubes[cycle[i] + side * sideOffsetRef[type]]->rotateStart(type, 3, cw, newAxis, skipAnim);
    }
    Cube::rotateStart(type, side, cw, newAxis, skipAnim);
}

void Room729::rotateStart(int num, int type, int side, bool cw, bool skipAnim)
{
    if (num == 0)
    {
        rotateStart(type, side, cw, center, skipAnim);
    }
    else
    {
        num--;
        rotateStartFragment(faceRef[2][num] + 2 * sideOffsetRef[2], type, side, cw, skipAnim);
    }
}

void Room729::rotateStartFragment(int num, int type, int side, bool cw, bool skipAnim)
{
    cubes[num]->rotateStart(type, side, cw, cubes[num]->getCenter(), skipAnim);
}