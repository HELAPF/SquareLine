#include "mapInit.h"
#include <stdlib.h>
#include <random>

USING_NS_CC;

void mapInit::Initialize(int mapType) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 15);

	int tile, number, numberCount, count;
	number = 1;
	numberCount = 0;
	count = 0;

	int area, line;
	area = 0;
	line = 0;

	for (int i = 0; i < 9; i++) {
		while (count < 16) {
			tile = dist(gen);
			if (block[i][tile] == 0) {
				block[i][tile] = number;
				numberCount++;
				count++;
				if (numberCount == 2) {
					number++;
					numberCount = 0;
				}
			}
		}
		number = 1;
		count = 0;
	}

	for (int i = 0; i < 12; i++) {
		if (i == 4 || i == 8) {
			area++;
			line = 0;
		}
		for (int j = 0; j < 12; j++) {
			if (j >= 0 && j < 4) { data_squareMap[i][j] = block[area * 3][line * 4 + j]; }
			else if (j >= 4 && j < 8) { data_squareMap[i][j] = block[area * 3 + 1][line * 4 + j - 4]; }
			else if (j >= 8 && j < 12) { data_squareMap[i][j] = block[area * 3 + 2][line * 4 + j - 8]; }
		}
		line++;
	}

	switch (mapType) { //Blank making progress
	case SQUARE:
		break;
	case ACORN:
		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 12; j++) {
				switch (i) {
				case 0:
					if (j >= 0 && j < 5) {
						data_squareMap[i][j] = 9;
					}
					else if (j == 9 || j == 11) {
						data_squareMap[i][j] = 9;
					}
					break;
				case 1:
					if (j >= 0 && j < 5) {
						data_squareMap[i][j] = 9;
					}
					break;
				case 2:
					if (j >= 0 && j < 4) {
						data_squareMap[i][j] = 9;
					}
					else if (j == 11) {
						data_squareMap[i][j] = 9;
					}
					break;
				case 3:
					if (j >= 0 && j < 3) {
						data_squareMap[i][j] = 9;
					}
					break;
				case 4:
					if (j >= 0 && j < 2) {
						data_squareMap[i][j] = 9;
					}
					break;
				case 5:
					data_squareMap[i][0] = 9;
					break;
				case 6:
					break;
				default:
					if (j > 9 + (7 - i)) {
						data_squareMap[i][j] = 9;
					}
					break;
				}
			}
		}
		break;
	case CIRCLE:
		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 12; j++) {
				if (i == 0 || i == 11) {
					if (j >= 4 && j <= 7) {}
					else {
						data_squareMap[i][j] = 9;
					}
				}
				else if (i == 1 || i == 10) {
					if (j >= 2 && j <= 9) {}
					else {
						data_squareMap[i][j] = 9;
					}
				}
				else if (i >= 4 && i <= 7) {}
				else {
					if (j >= 1 && j <= 10) {}
					else {
						data_squareMap[i][j] = 9;
					}
				}
			}
		}
		break;
	case HEART:
		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 12; j++) {
				if (i == 0) {
					if (j == 0 || j == 1 || j == 10 || j == 11) {
						data_squareMap[i][j] = 9;
					}
					if (j >= 4 && j <= 7) {
						data_squareMap[i][j] = 9;
					}

				}
				if (i == 1) {
					if (j == 0 || j == 5 || j == 6 || j == 11) {
						data_squareMap[i][j] = 9;
					}
				}
				if (i >= 7) {
					if (j >= (i - 6) && j <= 11 - (i - 6)) {}
					else { data_squareMap[i][j] = 9; }
				}
			}
		}
		break;
	case HARD:
		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 12; j++) {
				if (i == 0 || i == 11) {
					if (j == 0 || j == 11) {
						data_squareMap[i][j] = 9;
					}
				}
				if (i == 1 || i == 10) {
					if (j >= 4 && j <= 7) {
						data_squareMap[i][j] = 9;
					}
				}
				if (i == 2 || i == 9) {
					switch (i) {
					case 2:
						if (j >= 5 && j <= 7) {
							data_squareMap[i][j] = 9;
						}
						break;
					case 9:
						if (j >= 4 && j <= 6) {
							data_squareMap[i][j] = 9;
						}
						break;
					}
				}
				if (i == 3 || i == 8) {
					switch (i) {
					case 3:
						data_squareMap[i][5] = 9;
						break;
					case 8:
						data_squareMap[i][6] = 9;
						break;
					}
				}
				if (i == 4 || i == 7) {
					data_squareMap[i][1] = 9;
					data_squareMap[i][10] = 9;
				}
				if (i == 5 || i == 6) {
					switch (i) {
					case 5:
						if ((j >= 1 && j <= 2) || (j >= 8 && j <= 10)) {
							data_squareMap[i][j] = 9;
						}
						break;
					case 6:
						if ((j >= 1 && j <= 3) || (j >= 9 && j <= 10)) {
							data_squareMap[i][j] = 9;
						}
						break;
					}
				}
			}
		}
		break;
	default:
		break;
	}
}

int mapInit::GetTile(int x, int y) {
	return data_squareMap[x][y];
}

void mapInit::ClearData() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 16; j++) {
			block[i][j] = 0;
		}
	}
}