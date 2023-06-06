#include "Constants.h"
#include "Cube.h"
#include <iostream>

Cube::Cube() : Cube(0, 0, 0)
{
}

Cube::Cube(float x, float y, float z) : center{ x, y, z }
{
}

float* Cube::getCenter()
{
    return center;
}

void Cube::setCenter(float newCenter[DIMENSION])
{
    memcpy(center, newCenter, sizeof(center));
}

void Cube::setRoomNumber(int room, int num) {}

void Cube::setBlankRoomNumber(int room)
{
    setRoomNumber(room, -1);
}

void Cube::setRoomCol(int room, const float* col) {}

void Cube::toggleNumberVisibility(bool toggle) {}

void Cube::setCol(int face, const float col[Constants::COL_SIZE]) {}

bool Cube::inMotion()
{
    return inRotation || inTranslation || inCooldown;
}

void Cube::draw() {}

void Cube::update()
{
    if (inCooldown)
    {
        if (cooldownTimer > 0) {cooldownTimer--;}
        if (cooldownTimer == 0)
        {
            cooldownTimer = -1;
            inCooldown = false;
        }
        return;
    }

    if (inTranslation)
    {
        if (openTimer > 0)
        {
            openTimer--;
            if (openSelf)
            {
                for (int i = 0; i < DIMENSION; i++)
                {
                    targetCenter[i] += (parentOpen ? 1 : -1) * openDir[i] * (openAmount / Constants::OPEN_TIME);
                }
                setCenter(targetCenter);
            }
        }
        if (openTimer == 0)
        {
            openTimer = -1;
            inTranslation = false;
            inCooldown = true;
            cooldownTimer = Constants::COOLDOWN_TIME;
        }
        return;
    }

    if (inRotation)
    {
        if (!rotationCW && currAngle < targetAngle) {
            currAngle += Constants::ROTATION_AMOUNT / Constants::ROTATION_TIME;
        }
        else if (rotationCW && currAngle > targetAngle) {
            currAngle -= Constants::ROTATION_AMOUNT / Constants::ROTATION_TIME;
        }

        if (rotationTimer > 0) {rotationTimer--;}
        if (rotationTimer == 0)
        {
            rotateStop(rotationType, rotationSide, rotationCW);
        }
    }
}

void Cube::rotateStart(int type, int side, bool cw, float* newAxis, bool skipAnim)
{
    inRotation = true;
    targetAngle = (cw ? -1.0f : 1.0f) * Constants::ROTATION_AMOUNT;
    rotationType = type;
    rotationSide = side;
    rotationCW = cw;
    axis = newAxis;
    rotationTimer = Constants::ROTATION_TIME;
    toggleNumberVisibility(false);
    if (skipAnim) {rotateStop(type, side, cw);}
}

void Cube::rotateStop(int type, int side, bool cw)
{
    inRotation = false;
    inCooldown = true;
    rotationTimer = -1;
    cooldownTimer = Constants::COOLDOWN_TIME;
    currAngle = 0;
    targetAngle = 0;
    rotationType = -1;
    rotationSide = -1;

    int rotationNum = cw ? 1 : 3;
    for (int i = 0; i < rotationNum; i++)
    {
        rotateEdge(type, side);
    }
    toggleNumberVisibility(true);
}

void Cube::rotateEdge(int type, int side) {}

bool Cube::hasRoomNumber(int room) { return false; }

int Cube::findRoom(int room)
{
    return -1;
}

void Cube::enterFocusRoom(int room)
{
    focusRoom = room;
    focusRoomLoc = findRoom(room);
    if (!isOpen) {toggleOpen(false, parentOpen);}
}

void Cube::setOpenDir(const int dir[DIMENSION])
{
    memcpy(openDir, dir, sizeof(openDir));
}

void Cube::toggleOpen(bool toggleSelf, bool toggleParent)
{
    openSelf = toggleSelf;
    parentOpen = toggleParent;
    if (!toggleSelf)
    {
        isOpen = !isOpen;
    }
    openTimer = Constants::OPEN_TIME;
    inTranslation = true;
    memcpy(targetCenter, center, sizeof(center));
}
