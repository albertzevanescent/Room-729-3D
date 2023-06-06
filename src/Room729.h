#pragma once

#include "Room27.h"
#include "Constants.h"


class Room729 : public Room27
{
	public:
		Room729();
		Room729(float x, float y, float z);
		void draw() override;
		void update() override;
		void rotateStart(int type, int side, bool cw, float* newAxis, bool skipAnim) override;
		void rotateStart(int num, int type, int side, bool cw, bool skipAnim);
		void rotateStartFragment(int num, int type, int side, bool cw, bool skipAnim);
};

