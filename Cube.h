#pragma once

#include "Constants.h"

class Cube
{
    public:
	    const static int DIMENSION = 3;
	    const static int NUM_VERTICES = 4;
        const static int NUM_FACES = 6;
	    float center[DIMENSION] = {};

        bool inRotation = false;
        float* axis = 0;
        float currAngle = 0;
        float targetAngle = 0;
        int rotationType = -1;
        int rotationSide = -1;
        bool rotationCW = true;
        int rotationTimer = -1;

        bool inCooldown = false;
        int cooldownTimer = -1;

        int focusRoom = -1;
        int focusRoomLoc = -1;
        bool awaitFocus = false;

        bool inTranslation = false;
        bool isOpen = false;
        bool parentOpen = false;
        bool openSelf = true;
	    int openDir[DIMENSION] = {0, 0, 0};
	    float targetCenter[DIMENSION] = {0, 0, 0};
        int openAmount = Constants::OPEN_AMOUNT;
        int openTimer = -1;

        Cube();
        Cube(float x, float y, float z);

        float* getCenter();
        virtual void setCenter(float newCenter[DIMENSION]);
        virtual void setRoomNumber(int room, int num);
        virtual void setBlankRoomNumber(int room);
        virtual void setRoomCol(int room, const float* col);
	    virtual void toggleNumberVisibility(bool toggle);
        virtual void setCol(int face, const float col[Constants::COL_SIZE]);
        bool inMotion();
        virtual void draw();
        virtual void update();
        virtual void rotateStart(int type, int side, bool cw, float* newAxis, bool skipAnim);
        virtual void rotateStop(int type, int side, bool cw);
        virtual void rotateEdge(int type, int side);

        virtual bool hasRoomNumber(int room);
		virtual int findRoom(int room);
        virtual void enterFocusRoom(int room);

        virtual void setOpenDir(const int dir[DIMENSION]);
        virtual void toggleOpen(bool toggleSelf, bool toggleParent);
};