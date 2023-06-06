#pragma once

#include "Cube.h"
#include "Constants.h"

class Room27 : public Cube
{
	public:
		const static int CUBE_NUM = 27;
		const static int FACE_CYCLE_LENGTH = 4;
		const static int cubePattern[CUBE_NUM][DIMENSION];
		const static int sideOffsetRef[DIMENSION];
		const static int faceRef[DIMENSION][DIMENSION * DIMENSION];
		const static int faceCycleRef[DIMENSION][FACE_CYCLE_LENGTH][FACE_CYCLE_LENGTH / 2];
		const static int focusCycleRef[CUBE_NUM][DIMENSION];

		Cube* cubes[CUBE_NUM];

		Room27();
		Room27(float x, float y, float z);
		Room27(float x, float y, float z, int roomNum);
		void setCenter(float newCenter[DIMENSION]) override;
		void setRoomNumber(int room, int num) override;
		void setBlankRoomNumber(int room) override;
        void setRoomCol(int room, const float* col) override;
		void draw() override;
		void update() override;
		void rotateStart(int type, int side, bool cw, float* newAxis, bool skipAnim) override;
		void rotateStart(int num, int type, int side, bool cw, bool skipAnim);
		void rotateEdge(int type, int side) override;

        bool hasRoomNumber(int room) override;
		int findRoom(int room) override;

        void toggleOpen(bool toggleSelf, bool toggleParent) override;
};

