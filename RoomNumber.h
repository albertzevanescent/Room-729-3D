#pragma once

#include "Constants.h"
#include "Cube.h"

class RoomNumber
{
	public:
		const static int SEGMENT_NUM = 7;
		const static int SEGMENT_VERTICES_NUM = 6;
		const static int SEGMENT_SCALING = 1;
		const static int DIGIT_NUM = 10;
		const static int ROOM_LENGTH = 40;
		const static int PLANE_NUM = 6;

		const static float SEGMENT_WIDTH;
		const static float SEGMENT_HEIGHT;
		const static float DIGIT_SPACING;
		const static float OVERLAP_OFFSET;

		const static float segmentCenters[SEGMENT_NUM * (Cube::DIMENSION - 1)];
		const static float segmentVertices[SEGMENT_VERTICES_NUM * (Cube::DIMENSION - 1)];
		const static bool digitMapping[DIGIT_NUM][SEGMENT_NUM];

		const static int planeMapping[PLANE_NUM][Cube::DIMENSION];
		const static int vertexMapping[4][Cube::DIMENSION - 1];
		float center[Cube::DIMENSION] = {};
		int number = -1;
		int plane = 0;
		bool numVisible = true;

		float col[Constants::COL_SIZE];

		RoomNumber();
		RoomNumber(float x, float y, float z);
		void setCol(const float* colour);
		void setCenter(float newCenter[Cube::DIMENSION]);
		void setCenter(float x, float y, float z);
		void draw();
		void drawRoom();
		void drawNumber(float center[Cube::DIMENSION]);
		void drawDigit(float center[Cube::DIMENSION], int digit);
		void drawSegment(float center[Cube::DIMENSION], bool rotate);
		void update();
};