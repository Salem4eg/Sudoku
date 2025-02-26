#include "Sudoku_generator.h"
#include <QMessageBox>


//Створювач судоку створює готове судоку й на основі нього робить головоломку зі заданою складністю, яку визначає вирішувач судоку.


bool Sudoku_generator::isValid(int row, int col, int number)
{
	for (int c = 0; c < size; c++)
		if (completed_sudoku[row][c] == number)
			return false;

	for (int r = 0; r < size; r++)
		if (completed_sudoku[r][col] == number)
			return false;

	int boxRow = row - row % 3;
	int boxCol = col - col % 3;

	for (int r = 0; r < 3; r++)
		for (int c = 0; c < 3; c++)
			if (completed_sudoku[r + boxRow][c + boxCol] == number)
				return false;

	return true;
}

std::pair<int, int> Sudoku_generator::emptyField()
{
	int row;
	int col;
	for (row = 0; row < size; row++)
		for (col = 0; col < size; col++)
			if (completed_sudoku[row][col] == 0)
				return { row, col };

	return { -1, -1 };
}


void Sudoku_generator::clear()
{
	for (int row = 0; row < size; row++)
		for (int col = 0; col < size; col++)
			completed_sudoku[row][col] = 0;
}

bool Sudoku_generator::generate()
{	
	for (int row = 0; row < size; row++)
	{
		for (int col = 0; col < size; col++)
		{
			if (completed_sudoku[row][col] == 0)
			{
				QList<int> numbers{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
				std::shuffle(numbers.begin(), numbers.end(), number_generator);

				for (int number : numbers)
				{
					
					if (isValid(row, col, number))
					{
						completed_sudoku[row][col] = number;

						if (generate())
							return true;

						completed_sudoku[row][col] = 0;
					}
					
				}

				return false;
			}
		}
	}

	return true;
}


void Sudoku_generator::createPuzzle(int completed_numbers)
{
	uncompleted_sudoku = completed_sudoku;

	int empty_pairs = (81 - completed_numbers) / 2;

	for (int i = 0; i < empty_pairs; i++)
	{
		int row = number_generator() % 9;
		int col = number_generator() % 9;

		if (uncompleted_sudoku[row][col] == 0)
		{
			i--;
			continue;
		}

		uncompleted_sudoku[row][col] = 0;

		//обертовий аналог
		uncompleted_sudoku[size - 1 - row][size - 1 - col] = 0;

	}
}

