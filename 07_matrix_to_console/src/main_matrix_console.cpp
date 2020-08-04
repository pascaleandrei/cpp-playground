#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>

/**
	Define a simple matrix.
*/
class Matrix
{
private:
	size_t column_count;
	size_t line_count;
	char** matrix;

public:
	Matrix(size_t numColumnsX, size_t numLinesY):
		column_count(numColumnsX),
		line_count(numLinesY) {
		matrix = new char* [line_count];
		for (int index = 0; index < line_count; ++index) {
			matrix[index] = new char[column_count];
		}
	}

	~Matrix()
	{
		for (int index = 0; index < line_count; ++index) {
			delete[] matrix[index];
		}
		delete[] matrix;
	}

	// Set an entire line
	void setLine(size_t line_number, const std::string& data)
	{
		if (data.length() != column_count) {
			return;
		}
		for (int index = 0; index < column_count; ++index) {
			matrix[line_number][index] = data.at(index);
		}
	}

	char getCellXY(size_t x, size_t y)
	{
		if (x < 0 || x >= column_count || y < 0 || y >= line_count) {
			return 0;
		}
		return matrix[y][x];
	}

	/**
		Sets the cell content for a specific cell identified by its coordinates (X and Y)
		@param x The horizontal coordinate of the cell to set. Index starts at zer0.
		@param y The vertical coordinate of the cell to set. Index starts at zer0.
		@param cell_content A cell char to set.

		E.g.
		 X axis: 0 -------- ... -------> Width
		 Y axis
		 0       0,0  1,0                W-1, 0
		 |       0,1  1,1                     
		 |       0,2                      
		 .                             
		 .                             
		 .                             
		 |       0, H-1     ...          W-1, H-1           
		 V

		 Height
	*/
	void setCellXY(size_t x, size_t y, char cell_content)
	{
		if (x < 0 || x >= column_count || y < 0 || y >= line_count) {
			return;
		}
		matrix[y][x] = cell_content;
	}

	void print()
	{
		for (int lineIndex = 0; lineIndex < line_count; ++lineIndex) {
			for (int columnIndex = 0; columnIndex < column_count; ++columnIndex) {
				std::cout << matrix[lineIndex][columnIndex];
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
};

int main()
{
	// Create a matrix with the width of 20 and height of 10.
	Matrix matrix(20, 10);

	matrix.setLine(0, "X-----X----X-----XX-");
	matrix.setLine(1, "--X-----------------");
	matrix.setLine(2, "-----X--------------");
	matrix.setLine(3, "--------X-----------");
	matrix.setLine(4, "-----------X--------");
	matrix.setLine(5, "--------------X-----");
	matrix.setLine(6, "-----------------X--");
	matrix.setLine(7, "-------------------X");
	matrix.setLine(8, "------------------X-");
	matrix.setLine(9, "-----------------X--");

	matrix.print();
	// Would print
/*
X-----X----X-----XX-
--X-----------------
-----X--------------
--------X-----------
-----------X--------
--------------X-----
-----------------X--
-------------------X
------------------X-
-----------------X--
*/
	matrix.setCellXY(2, 1, '-');
	matrix.print();
	// Would print
/*
X-----X----X-----XX-
--------------------
-----X--------------
--------X-----------
-----------X--------
--------------X-----
-----------------X--
-------------------X
------------------X-
-----------------X--
*/

	matrix.setCellXY(3, 7, 'O');
	matrix.print();
/*
X-----X----X-----XX-
--------------------
-----X--------------
--------X-----------
-----------X--------
--------------X-----
-----------------X--
---O---------------X
------------------X-
-----------------X--
*/

	// This should silently fail (not trigger an error): cell Y=11 invalid due to limited height.
	matrix.setCellXY(3, 11, 'O');
	return 0;
}