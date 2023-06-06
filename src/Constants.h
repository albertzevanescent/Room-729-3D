#pragma once

class Constants
{
	public:
		const static int COL_SIZE = 4;
		const static int COL_NUM = 6;
		const static float colRed[COL_SIZE];
		const static float colGreen[COL_SIZE];
		const static float colBlue[COL_SIZE];
		const static float colYellow[COL_SIZE];
		const static float colWhite[COL_SIZE];
		const static float colOrange[COL_SIZE];
		const static float colBlack[COL_SIZE];
		const static float* defaultCol[COL_NUM];

		const static int CUBE_SIDE_LENGTH = 50;
		const static int ROTATION_TIME = 30;
		const static int ROTATION_AMOUNT = 90;
		const static int COOLDOWN_TIME = 10;
		const static int OPEN_TIME = 10;
		const static int OPEN_AMOUNT = 10;
};
