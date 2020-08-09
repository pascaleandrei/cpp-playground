#include <iostream>

#define DEFAULT_SIZE 20
#define INFERIOR_LIMIT 2
#define SUPERIOR_LIMIT 3

int** matrix;
int size;
enum patterns { BLOCK = 0, BOAT, BLINKER, BEACON, PULSAR, PENTADECATHLON, GLIDER };

void allocateAndInitializeMatrix() {
	std::cin >> size;
	if (size < DEFAULT_SIZE) {
		size = DEFAULT_SIZE;
	}
	matrix = new int* [size];
	for (int index1 = 0; index1 < size; ++index1) {
		matrix[index1] = new int[size];
		for (int index2 = 0; index2 < size; ++index2) {
			matrix[index1][index2] = 0;
		}
	}
}

void drawBlock(int x, int y) {
	if (x == size - 1 || y == size - 1) {
		return;
	}
	matrix[y][x] =
		matrix[y + 1][x] =
		matrix[y][x + 1] =
		matrix[y + 1][x + 1] = 1;
}

void drawBoat(int x, int y) {
	if (x >= size - 2 || y >= size - 2) {
		return;
	}
	matrix[y][x] =
		matrix[y + 1][x] =
		matrix[y][x + 1] =
		matrix[y + 2][x + 1] =
		matrix[y + 1][x + 2] = 1;
}

void drawBlinker(int x, int y) {
	if (x >= size -2) {
		return;
	}
	matrix[x][y] =
		matrix[x + 1][y] =
		matrix[x + 2][y] = 1;
}

void drawBeacon(int x, int y) {
	if (x >= size - 3 || y >= size - 3) {
		return;
	}
	matrix[y][x] =
		matrix[y + 1][x] =
		matrix[y][x + 1] =
		matrix[y + 3][x + 2] =
		matrix[y + 2][x + 3] =
		matrix[y + 3][x + 3] = 1;
}

void drawPulsar(int x, int y) {
	if (x <= 3 || x >= size - 10 || y >= size - 14) {
		return;
	}
	matrix[y][x] =
		matrix[y][x + 6] =
		matrix[y + 1][x] =
		matrix[y + 1][x + 6] =
		matrix[y + 2][x] =
		matrix[y + 2][x + 1] =
		matrix[y + 2][x + 5] =
		matrix[y + 2][x + 6] =
		matrix[y + 4][x - 4] =
		matrix[y + 4][x - 3] =
		matrix[y + 4][x - 2] =
		matrix[y + 4][x + 1] =
		matrix[y + 4][x + 2] =
		matrix[y + 4][x + 4] =
		matrix[y + 4][x + 5] =
		matrix[y + 4][x + 8] =
		matrix[y + 4][x + 9] =
		matrix[y + 4][x + 10] =
		matrix[y + 5][x - 2] =
		matrix[y + 5][x] =
		matrix[y + 5][x + 2] =
		matrix[y + 5][x + 4] =
		matrix[y + 5][x + 6] =
		matrix[y + 5][x + 8] =
		matrix[y + 6][x] =
		matrix[y + 6][x + 1] =
		matrix[y + 6][x + 5] =
		matrix[y + 6][x + 6] =
		matrix[y + 8][x] =
		matrix[y + 8][x + 1] =
		matrix[y + 8][x + 5] =
		matrix[y + 8][x + 6] =
		matrix[y + 9][x - 2] =
		matrix[y + 9][x] =
		matrix[y + 9][x + 2] =
		matrix[y + 9][x + 4] =
		matrix[y + 9][x + 6] =
		matrix[y + 9][x + 8] =
		matrix[y + 10][x - 4] =
		matrix[y + 10][x - 3] =
		matrix[y + 10][x - 2] =
		matrix[y + 10][x + 1] =
		matrix[y + 10][x + 2] =
		matrix[y + 10][x + 4] =
		matrix[y + 10][x + 5] =
		matrix[y + 10][x + 8] =
		matrix[y + 10][x + 9] =
		matrix[y + 10][x + 10] =
		matrix[y + 12][x] =
		matrix[y + 12][x + 1] =
		matrix[y + 12][x + 5] =
		matrix[y + 12][x + 6] =
		matrix[y + 13][x] =
		matrix[y + 13][x + 6] =
		matrix[y + 14][x] =
		matrix[y + 14][x + 6] = 1;
}

void drawPentadecathlon(int x, int y) {
	if (x >= size - 9 || y == 0 || y == size - 1) {
		return;
	}
	matrix[y][x] =
		matrix[y][x + 1] =
		matrix[y - 1][x + 2] =
		matrix[y + 1][x + 2] =
		matrix[y][x + 3] =
		matrix[y][x + 4] =
		matrix[y][x + 5] =
		matrix[y][x + 6] =
		matrix[y - 1][x + 7] =
		matrix[y + 1][x + 7] =
		matrix[y][x + 8] =
		matrix[y][x + 9] = 1;
}

void drawGlider(int x, int y) {
	if (x == 0 || y >= size - 2) {
		return;
	}
	matrix[y][x] =
		matrix[y + 1][x + 1] =
		matrix[y + 2][x - 1] =
		matrix[y + 2][x] =
		matrix[y + 2][x + 1] = 1;
}

void drawShape(patterns pattern, int x, int y) {
	if (x < 0 || y < 0 || x >= size || y >= size) {
		return;
	}
	switch (pattern) {
	case BLOCK:
		drawBlock(x, y);
		break;
	case BOAT:
		drawBoat(x, y);
		break;
	case BLINKER:
		drawBlinker(x, y);
		break;
	case BEACON:
		drawBeacon(x, y);
		break;
	case PULSAR:
		drawPulsar(x, y);
		break;
	case PENTADECATHLON:
		drawPentadecathlon(x, y);
		break;
	case GLIDER:
		drawGlider(x, y);
		break;
	default:
		break;
	}
}

int getNeighboursCount(int x, int y) {
	int count = 0;
	if (x > 0) {
		if (y > 0) {
			count += matrix[y - 1][x - 1];
		}
		count += matrix[y][x - 1];
		if (y < size - 1) {
			count += matrix[y + 1][x - 1];
		}
	}
	if (x < size - 1) {
		if (y > 0) {
			count += matrix[y - 1][x + 1];
		}
		count += matrix[y][x + 1];
		if (y < size - 1) {
			count += matrix[y + 1][x + 1];
		}
	}
	if (y > 0) {
		count += matrix[y - 1][x];
	}
	if (y < size - 1) {
		count += matrix[y + 1][x];
	}
	return count;
}

int nextGeneration() {
	int** temp, newValue, empty = 1;
	temp = new int* [size];
	for (int index1 = 0; index1 < size; ++index1) {
		temp[index1] = new int[size];
		for (int index2 = 0; index2 < size; ++index2) {
			temp[index1][index2] = getNeighboursCount(index2, index1);
		}
	}
	for (int index1 = 0; index1 < size; ++index1) {
		for (int index2 = 0; index2 < size; ++index2) {
			if (matrix[index1][index2] == 1) {
				newValue = temp[index1][index2] >= INFERIOR_LIMIT && temp[index1][index2] <= SUPERIOR_LIMIT;
			}
			else {
				newValue = temp[index1][index2] == SUPERIOR_LIMIT;
			}
			matrix[index1][index2] = newValue;
			if (newValue == 1) {
				empty = 0;
			}
		}
		delete[] temp[index1];
	}
	delete[] temp;
	return !empty;
}

void printMatrix() {
	for (int index1 = 0; index1 < size; ++index1) {
		for (int index2 = 0; index2 < size; ++index2) {
			std::cout << matrix[index1][index2] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void setShapes() {
	int pattern, x, y;
	while(true) {
		std::cout << "Please choose a pattern to draw:" << std::endl;
		std::cout << BLOCK + 1 << " -> Block" << std::endl;
		std::cout << BOAT + 1 << " -> Boat" << std::endl;
		std::cout << BLINKER + 1 << " -> Blinker" << std::endl;
		std::cout << BEACON + 1 << " -> Beacon" << std::endl;
		std::cout << PULSAR + 1 << " -> Pulsar" << std::endl;
		std::cout << PENTADECATHLON + 1 << " -> Pentadecathlon" << std::endl;
		std::cout << GLIDER + 1 << " -> Glider" << std::endl;
		std::cout << 0 << " -> Exit" << std::endl;
		std::cin >> pattern;
		if (pattern == 0) {
			return;
		}
		std::cout << "Please enter the coordinates: ";
		std::cin >> x >> y;
		drawShape(static_cast<patterns>(pattern - 1), x, y);
	}
}

void gameOfLife() {
	int ticks;
	std::cin >> ticks;
	setShapes();
	for (int index = 0; index < ticks; ++index) {
		if (!nextGeneration()) {
			index = ticks;
		}
		printMatrix();
	}
}

void deallocateMatrix() {
	for (int index = 0; index < size; ++index) {
		delete[] matrix[index];
	}
	delete[] matrix;
}

int main()
{
	allocateAndInitializeMatrix();
	gameOfLife();
	deallocateMatrix();
	return 0;
}
