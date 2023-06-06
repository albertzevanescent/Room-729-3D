#include "Constants.h"

const float Constants::colRed[COL_SIZE] = { 1, 0, 0, 1 };
const float Constants::colGreen[COL_SIZE] = { 0, 1, 0, 1 };
const float Constants::colBlue[COL_SIZE] = { 0, 0, 1, 1 };
const float Constants::colYellow[COL_SIZE] = { 1, 1, 0, 1 };
const float Constants::colWhite[COL_SIZE] = { 1, 1, 1, 1 };
const float Constants::colOrange[COL_SIZE] = { 1, 0.5, 0, 1 };
const float Constants::colBlack[COL_SIZE] = { 0, 0, 0, 1 };
const float* Constants::defaultCol[COL_NUM] = { Constants::colWhite, Constants::colYellow, Constants::colBlue,
                                        Constants::colGreen, Constants::colRed, Constants::colOrange };