#pragma once
#include <QColor>
#include <QPair>
#include <QSet>


enum Difficulties
{
	easy, normal, hard, ultra_hard
};

enum Categories
{
	only_hints_included, only_errors_included, everything_included, nothing_included
};

struct Record
{
	int time;
	int errors;
	int hints;
	Difficulties difficulty;

	Record(int time = 0, int errors = 0, int hints = 0, Difficulties difficulty = Difficulties::easy) : time(time), errors(errors), hints(hints), difficulty(difficulty)
	{
	}

	bool operator<(Record other)
	{
		return time < other.time;
	}
};

struct Theme
{
	QString name;
	QColor regular_text;
	QColor hovered_text;
	QColor field_border;
	QColor field;
	QColor background;
	QString main_menu_background;
};

enum Text_size
{
	small = 16, medium = 20, big = 24
};


// структура для зберігання координат двох ліній, якщо дві лінії лежать на різних рядках та стовпах, то обидві змінні заповнені, а якщо якась лінія
	// на однаковому рядку/стовпі, то координати -1,-1, тобто дві лінії лежать на одному рядку, то рядкові координати -1, -1
struct two_lines_coordinates
{
	QPair<int, int> row_coords;
	QPair<int, int> col_coords;

	two_lines_coordinates(int row1 = -1, int row2 = -1, int col1 = -1, int col2 = -1) : row_coords(row1, row2), col_coords(col1, col2)
	{}

	bool isRowValid()
	{
		return row_coords.first != -1 && row_coords.second != -1;
	}

	bool isColValid()
	{
		return col_coords.first != -1 && col_coords.second != -1;
	}

	bool isValid()
	{
		return isColValid() || isRowValid();
	}
};

struct coords
{
	int row;
	int col;

	coords(int r = -1, int c = -1) : row(r), col(c) {}

	bool operator==(const coords& other) const
	{
		return other.row == row && other.col == col;
	}

	bool operator!=(const coords& other) const
	{
		return other.row != row || other.col != col;
	}
};

struct coords_hash
{
	size_t operator()(const coords& coord) const
	{
		return coord.col ^ coord.row;
	}
};


inline size_t qHash(const coords& key, size_t seed = 0)
{
	return key.row ^ key.col;
}


struct numbers_and_coordinates
{
	QSet<int> numbers;
	coords coord;

	numbers_and_coordinates(QSet<int>& n, int row, int col) : numbers(n), coord(row, col)
	{

	}

	numbers_and_coordinates() : coord(-1, -1)
	{
	}

};

