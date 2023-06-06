#pragma once

#include "Constants.h"
#include "RoomNumber.h"
#include "Cube.h"

class Room1 : public Cube
{
	public:
		const static float faceCenter[NUM_FACES * DIMENSION];
		const static float vertexPattern[NUM_FACES * NUM_VERTICES * DIMENSION];

		const static int EDGE_CYCLE_LENGTH = 4;
		const static int edgeCycle[DIMENSION][EDGE_CYCLE_LENGTH];

		float vertices[NUM_FACES * NUM_VERTICES * DIMENSION] = {};
		RoomNumber roomNumbers[NUM_FACES] = {};

		Room1();
		Room1(float x, float y, float z);
		Room1(float x, float y, float z, int num);
		void setCenter(float newCenter[DIMENSION]) override;
		void setCol(const float* col);
		void setCol(int face, const float col[Constants::COL_SIZE]) override;
		void setVertices();
		void setRoomNumberCenter(int num);
		void setRoomNumber(int room, int num) override;
        void setRoomCol(int room, const float* col) override;
		void toggleNumberVisibility(bool toggle) override;
		void draw() override;
		void rotateShift(bool cw);
		void rotateEdge(int type, int side) override;

        bool hasRoomNumber(int num) override;
};
