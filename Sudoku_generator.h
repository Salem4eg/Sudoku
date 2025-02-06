#pragma once

#include <random>
#include <QList>


class Sudoku_generator
{
private:
	size_t size;
	QList<QList<int>> completed_sudoku;
	QList<QList<int>> uncompleted_sudoku;
protected:
	std::mt19937 number_generator;
	bool isValid(int row, int col, int number);
	std::pair<int, int> emptyField();
public:
	Sudoku_generator(size_t sizeOfSudoku = 9) : size(sizeOfSudoku), completed_sudoku(size, QList<int>(size, 0)), number_generator(std::random_device{}()), uncompleted_sudoku(size, QList<int>(size, 0))
	{

	}
	void clear();
	// Створює поле судоку
	bool generate();
	// Приховує частину чисел з поля
	void createPuzzle(int empty_pairs);

	QList<QList<int>>& get_uncompleted_sudoku()
	{
		return uncompleted_sudoku;
	}

	QList<QList<int>>& get_completed_sudoku()
	{
		return completed_sudoku;
	}
};