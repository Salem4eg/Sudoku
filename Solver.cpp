#include "Solver.h"
#include <iostream>


void Solver::solve()
{
	while (single_candidate() || single_position() || candidate_lines() ||
				double_pairs() || naked_pair() || hidden_numbers() || x_wings() || swordfish() || jellyfish() || 
			simple_chains() || xy_wing() || xyz_wing())
		{
		}
}

bool Solver::solve_with_easy_techniques()
{
	bool used_easy_techniques = false;

	bool used_easy_techniques_this_cycle;

	do
	{
		used_easy_techniques_this_cycle = false;

		if (use_easy_techniques())
		{
			used_easy_techniques_this_cycle = true;
			used_easy_techniques = true;
		}

	}while (used_easy_techniques_this_cycle);

	return isSolved() && used_easy_techniques;
}

bool Solver::solve_with_normal_techniques()
{
	bool used_normal_techniques = false;

	bool used_easy_techniques_this_cycle;
	bool used_normal_techniques_this_cycle;

	do
	{
		used_easy_techniques_this_cycle = false;
		used_normal_techniques_this_cycle = false;

		if (use_easy_techniques())
		{
			used_easy_techniques_this_cycle = true;
			continue;
		}

		if (use_normal_techniques())
		{
			used_normal_techniques = true;
			used_normal_techniques_this_cycle = true;
		}

	}while (used_easy_techniques_this_cycle || used_normal_techniques_this_cycle);

	return isSolved() && used_normal_techniques;
}

bool Solver::solve_with_hard_techniques()
{
	bool used_hard_techniques = false;

	bool used_easy_techniques_this_cycle;
	bool used_normal_techniques_this_cycle;
	bool used_hard_techniques_this_cycle;

	do
	{
		used_easy_techniques_this_cycle = false;
		used_normal_techniques_this_cycle = false;
		used_hard_techniques_this_cycle = false;

		if (use_easy_techniques())
		{
			used_easy_techniques_this_cycle = true;
			continue;
		}

		if (use_normal_techniques())
		{
			used_normal_techniques_this_cycle = true;
			continue;
		}

		if (use_hard_techniques())
		{
			used_hard_techniques = true;
			used_hard_techniques_this_cycle = true;
		}

	} while (used_easy_techniques_this_cycle || used_normal_techniques_this_cycle || used_hard_techniques_this_cycle);

				
	return isSolved() && used_hard_techniques;
}

bool Solver::use_easy_techniques()
{
	return single_candidate() || single_position() || candidate_lines();
}

bool Solver::use_normal_techniques()
{
	return single_candidate() || single_position() || candidate_lines() ||
		double_pairs() || naked_pair() || hidden_numbers();
}

bool Solver::use_hard_techniques()
{
	return x_wings() || swordfish() || jellyfish() ||
		simple_chains() || xy_wing() || xyz_wing();
}

bool Solver::isValid(int row, int col, int number, const QList<QList<int>>& field) const
{
	for (int c = 0; c < size; c++)
		if (field[row][c] == number)
			return false;

	for (int r = 0; r < size; r++)
		if (field[r][col] == number)
			return false;

	int boxrow = row - row % 3;
	int boxcol = col - col % 3;

	for (int r = 0; r < 3; r++)
		for (int c = 0; c < 3; c++)
			if (field[r + boxrow][c + boxcol] == number)
				return false;

	return true;
}

bool Solver::isSolved()
{
	for (int row = 0; row < size; row++)
	{
		for (int col = 0; col < size; col++)
			if (field[row][col] == 0)
				return false;
	}

	return true;
}

void Solver::clear_row_and_col(int row, int col, int number)
{
	clear_row(row, number);
	clear_col(col, number);
}

//очищення записок рядка від числа
void Solver::clear_row(int row, int number)
{
	for (int c = 0; c < 9; c++)
		notes[row][c].removeOne(number);
	
}

// очищення записок колонки від числа
void Solver::clear_col(int col, int number)
{
	for (int r = 0; r < 9; r++)
		notes[r][col].removeOne(number);

}

// очищення записок квадрату від числа
void Solver::clear_box(int boxrow, int boxcol, int number)
{
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			for (auto i = notes[boxrow * 3 + row][boxcol * 3 + col].begin(); i != notes[boxrow * 3 + row][boxcol * 3 + col].end(); i++)
			{
				if (*i == number)
				{
					notes[boxrow * 3 + row][boxcol * 3 + col].erase(i);
					break;
				}
			}
		}
	}
}


bool Solver::clear_field(int row, int col, int number)
{
	return notes[row][col].removeOne(number);
}



bool Solver::clear_row_except(int row, int number, int exceptindex)
{
	bool number_erased = false;
	for (int c = 0; c < 9; c++)
	{
		if (c == exceptindex)
		{
			c += 2;
			continue;
		}
		QList<int> temp;
		for (int n : notes[row][c])
		{
			if (n == number)
				number_erased = true;
			else
				temp.push_back(n);

		}
		notes[row][c] = temp;
	}
	return number_erased;
}

bool Solver::clear_col_except(int col, int number, int exceptindex)
{
	bool number_erased = false;
	for (int r = 0; r < 9; r++)
	{
		if (r == exceptindex)
		{
			r += 2;
			continue;
		}
		QList<int> temp;
		for (int n : notes[r][col])
		{
			if (n == number)
				number_erased = true;
			else
				temp.push_back(n);
		}
		notes[r][col] = temp;
	}
	return number_erased;
}

bool Solver::clear_row_in_box(int boxrow, int boxcol, int row, int number)
{

	bool number_erased = false;
	for (int col = 0; col < 3; col++)
	{
		QList<int> temp;
		for (int n : notes[boxrow * 3 + row][boxcol * 3 + col])
		{
			if (n != number)
				temp.push_back(n);
			else
				number_erased = true;
		}

		notes[boxrow * 3 + row][boxcol * 3 + col] = temp;
	}

	return number_erased;

}

bool Solver::clear_col_in_box(int boxrow, int boxcol, int col, int number)
{

	bool number_erased = false;
	for (int row = 0; row < 3; row++)
	{
		QList<int> temp;
		for (int n : notes[boxrow * 3 + row][boxcol * 3 + col])
		{
			if (n != number)
				temp.push_back(n);
			else
				number_erased = true;
		}

		notes[boxrow * 3 + row][boxcol * 3 + col] = temp;
	}

	return number_erased;

}

bool Solver::clear_numbers_in_row_except(int row, QSet<int>& numbers, QSet<coords>&& except_coords)
{
	bool number_cleared = false;

	for (int col = 0; col < 9; col++)
	{
		if (except_coords.find({ row,col }) != except_coords.end())
			continue;

		QList<int> temp;
		for (int n : notes[row][col])
		{
			if (numbers.find(n) == numbers.end())
				temp.push_back(n);
			else
				number_cleared = true;
		}
		notes[row][col] = temp;
	}

	return number_cleared;
}

bool Solver::clear_numbers_in_col_except(int col, QSet<int>& numbers, QSet<coords>&& except_coords)
{
	bool number_cleared = false;
	for (int row = 0; row < 9; row++)
	{
		if (except_coords.find({ row,col }) != except_coords.end())
			continue;

		QList<int> temp;
		for (int n : notes[row][col])
		{
			if (numbers.find(n) == numbers.end())
				temp.push_back(n);
			else
				number_cleared = true;
		}
		notes[row][col] = temp;
	}

	return number_cleared;
}


bool Solver::clear_numbers_in_box_except(int boxrow, int boxcol, QSet<int>& numbers, QSet<coords>&& except_coords)
{
	bool number_cleared = false;
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			if (except_coords.find({ row,col }) != except_coords.end())
				continue;

			QList<int> temp;
			for (int n : notes[boxrow * 3 + row][boxcol * 3 + col])
			{
				if (numbers.find(n) == numbers.end())
					temp.push_back(n);
				else
					number_cleared = true;
			}
			notes[boxrow * 3 + row][boxcol * 3 + col] = temp;

		}
	}
	return number_cleared;
}


bool Solver::single_candidate()
{
	bool method_used = false;
	for (int row = 0; row < field.size(); row++)
	{
		for (int col = 0; col < field.size(); col++)
		{
			if (notes[row][col].size() == 1)
			{
				field[row][col] = notes[row][col][0];
				clear_row_and_col(row, col, field[row][col]);
				clear_box(row / 3, col / 3, field[row][col]);
				method_used = true;
			}
		}
	}
	return method_used;
}


bool Solver::single_position_box(int boxrow, int boxcol)
{
	bool method_used = false;
	// перелічуємо кількість чисел від 1 до 9
	QList<int> count(9, 0);
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			for (int n : notes[boxrow * 3 + row][boxcol * 3 + col])
				count[n - 1]++;
		}
	}

	// знаходимо число, яке було лише один раз і вставляємо його в поле

	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			for (int n : notes[boxrow * 3 + row][boxcol * 3 + col])
			{
				if (count[n - 1] == 1)
				{
					field[boxrow * 3 + row][boxcol * 3 + col] = n;
					notes[boxrow * 3 + row][boxcol * 3 + col].clear();
					clear_row_and_col(boxrow * 3 + row, boxcol * 3 + col, n);
					method_used = true;
					break;
				}
			}
		}
	}
	return method_used;
}

bool Solver::single_position()
{
	bool method_used = false;
	for (int boxrow = 0; boxrow < 3; boxrow++)
	{
		for (int boxcol = 0; boxcol < 3; boxcol++)
		{
			if (single_position_box(boxrow, boxcol))
				method_used = true;
		}
	}

	return method_used;
}



bool Solver::candidate_lines_box(int boxrow, int boxcol)
{
	bool method_used = false;

	QList<int> number_amount(9, 0);

	// перелічуємо кожне число
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			for (int number : notes[boxrow * 3 + row][boxcol * 3 + col])
				number_amount[number - 1]++;
		}
	}


	// перевірка кожного числа, чи всі його появи є на горизонтальній лінії
	for (int row = 0; row < 3; row++)
	{
		QList<int> number_amount_in_horizontal_line(9, 0);
		//перевіряємо горизонтальну лінію
		for (int col = 0; col < 3; col++)
		{
			// записуємо появу кожного числа на горизонтальній лінії
			for (int n : notes[boxrow * 3 + row][boxcol * 3 + col])
				number_amount_in_horizontal_line[n - 1]++;

		}

		int number = 1;
		for (int count : number_amount_in_horizontal_line)
		{
			//якщо число з'являється лише на горизонтальній лінії, то очищуємо цей рядок від цього числа на інших квадратах, окрім цього
			if (number_amount[number - 1] == count && count != 0)
			{
				if (clear_row_except(boxrow * 3 + row, number, boxcol * 3))
					method_used = true;
			}
			number++;
		}

	}

	// перевірка кожного числа на вертикальній лінії
	for (int col = 0; col < 3; col++)
	{
		QList<int> number_amount_in_vertical_line(9, 0);
		//перевіряємо вертикальну лінію
		for (int row = 0; row < 3; row++)
		{
			for (int n : notes[boxrow * 3 + row][boxcol * 3 + col])
				number_amount_in_vertical_line[n - 1]++;

		}

		int number = 1;
		for (int count : number_amount_in_vertical_line)
		{
			//якщо число з'являється лише на вертикальній лінії, то очищуємо цей стовпчик від цього числа на інших квадратах
			if (number_amount[number - 1] == count && count != 0)
			{
				if (clear_col_except(boxcol * 3 + col, number, boxrow * 3))
					method_used = true;
			}
			number++;
		}

	}


	return method_used;
}

bool Solver::candidate_lines()
{
	bool method_used = false;

	for (int boxrow = 0; boxrow < 3; boxrow++)
	{
		for (int boxcol = 0; boxcol < 3; boxcol++)
		{
			if (candidate_lines_box(boxrow, boxcol))
				method_used = true;
		}
	}

	return method_used;
}



two_lines_coordinates Solver::double_pairs_box(int boxRow, int boxCol, int number)
{
	QList<int> rowLines;
	QList<int> colLines;

	

	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			for (int n : notes[boxRow * 3 + row][boxCol * 3 + col])
			{
				if (n == number)
				{

					rowLines.push_back(row);
					colLines.push_back(col);

					if (rowLines.size() > 2 && colLines.size() > 2)
					{
						return { -1,-1,-1,-1 };
					}
					break;
				}
			}
		}
	}

	
	if (rowLines.size() == 2 && rowLines[0] != rowLines[1] &&
		colLines.size() == 2 && colLines[0] != colLines[1])
		return { rowLines[0], rowLines[1], colLines[0], colLines[1] };

	if (rowLines.size() == 2 && rowLines[0] != rowLines[1])
		return { rowLines[0], rowLines[1], -1, -1 };

	if (colLines.size() == 2 && colLines[0] != colLines[1])
		return { -1, -1, colLines[0], colLines[1] };

	return { -1,-1,-1,-1 };
}


bool Solver::double_pairs_number(int number)
{
	bool methodUsed = false;
	QList<two_lines_coordinates> lines;
	
	for (int boxRow = 0; boxRow < 3; boxRow++)
		for (int boxCol = 0; boxCol < 3; boxCol++)
			lines.push_back(double_pairs_box(boxRow, boxCol, number));

	for (int boxRow = 0; boxRow < 3; boxRow++)
	{
		for (int boxCol = 0; boxCol < 3; boxCol++)
		{
			size_t index = boxRow * 3 + boxCol;

			if (!lines[index].isValid())
				continue;

			// якщо число стоїть на двох горизонтальних лініях і число не на крайньому квадраті
			if (lines[index].isRowValid() && boxCol < 2)
			{
				// то перевіряємо, чи число на іншому квадраті стоїть на тих самих лініях
				if (boxCol == 0 && lines[index + 1].row_coords == lines[index].row_coords ||
					boxCol == 0 && lines[index + 2].row_coords == lines[index].row_coords ||
					boxCol == 1 && lines[index + 1].row_coords == lines[index].row_coords)
				{

					int box_col_to_clear;

					if (boxCol == 0 && lines[index + 1].row_coords == lines[index].row_coords)
						box_col_to_clear = 2;
					else if (boxCol == 0 && lines[index + 2].row_coords == lines[index].row_coords)
						box_col_to_clear = 1;
					else
						box_col_to_clear = 0;

					if (clear_row_in_box(boxRow, box_col_to_clear, lines[index].row_coords.first, number))
						methodUsed = true;
					if (clear_row_in_box(boxRow, box_col_to_clear, lines[index].row_coords.second, number))
						methodUsed = true;


				}
			}

			if (lines[index].isColValid() && boxRow < 2)
			{
				if (boxRow == 0 && lines[index + 3].col_coords == lines[index].col_coords ||
					boxRow == 0 && lines[index + 6].col_coords == lines[index].col_coords ||
					boxRow == 1 && lines[index + 3].col_coords == lines[index].col_coords)
				{
					int box_row_to_clear;

					if (boxRow == 0 && lines[index + 3].col_coords == lines[index].col_coords)
						box_row_to_clear = 2;
					else if (boxRow == 0 && lines[index + 6].col_coords == lines[index].col_coords)
						box_row_to_clear = 1;
					else
						box_row_to_clear = 0;


					if (clear_col_in_box(box_row_to_clear, boxCol, lines[index].col_coords.first, number))
						methodUsed = true;
					if (clear_col_in_box(box_row_to_clear, boxCol, lines[index].col_coords.second, number))
						methodUsed = true;

				}

			}
		}


	}

	return methodUsed;
}


bool Solver::double_pairs()
{
	bool methodUsed = false;

	for (int number = 1; number <= 9; number++)
	{
		if (double_pairs_number(number))
			methodUsed = true;

	}
	return methodUsed;
}



bool Solver::naked_pair_rows()
{
	bool method_used = false;

	for (int row = 0; row < 9; row++)
	{
		QList<numbers_and_coordinates> numbers_and_coords;

		for (int col = 0; col < 9; col++)
		{
			if (notes[row][col].size() == 2)
			{
				QSet<int> numbers{ notes[row][col][0], notes[row][col][1] };
				numbers_and_coords.emplace_back(numbers, row, col);
			}
		}

		if (numbers_and_coords.empty())
			continue;

		for (int i = 0; i < numbers_and_coords.size() - 1; i++)
		{
			for (int j = i + 1; j < numbers_and_coords.size(); j++)
			{
				if (numbers_and_coords[i].numbers == numbers_and_coords[j].numbers)
					if (clear_numbers_in_row_except(row, numbers_and_coords[i].numbers,
						{ numbers_and_coords[i].coord, numbers_and_coords[j].coord }))
						method_used = true;
			}
		}

	}
	return method_used;
}

bool Solver::naked_pair_cols()
{
	bool method_used = false;

	for (int col = 0; col < 9; col++)
	{
		QList<numbers_and_coordinates> numbers_and_coords;

		for (int row = 0; row < 9; row++)
		{
			if (notes[row][col].size() == 2)
			{
				QSet<int> numbers{ notes[row][col][0], notes[row][col][1] };
				numbers_and_coords.emplace_back(numbers, row, col);
			}
		}

		if (numbers_and_coords.empty())
			continue;

		for (int i = 0; i < numbers_and_coords.size() - 1; i++)
		{
			for (int j = i + 1; j < numbers_and_coords.size(); j++)
			{
				if (numbers_and_coords[i].numbers == numbers_and_coords[j].numbers)
					if (clear_numbers_in_col_except(col, numbers_and_coords[i].numbers,
						{ numbers_and_coords[i].coord, numbers_and_coords[j].coord }))
						method_used = true;
			}
		}
	}
	return method_used;
}

bool Solver::naked_pair_box()
{
	bool method_used = false;

	for (int boxrow = 0; boxrow < 3; boxrow++)
	{
		for (int boxcol = 0; boxcol < 3; boxcol++)
		{
			QList<numbers_and_coordinates> numbers_and_coords;


			for (int row = 0; row < 3; row++)
			{
				for (int col = 0; col < 3; col++)
				{
					int rowindex = boxrow * 3 + row;
					int colindex = boxcol * 3 + col;
					if (notes[rowindex][colindex].size() == 2)
					{
						QSet<int> numbers{ notes[rowindex][colindex][0], notes[rowindex][colindex][1] };
						numbers_and_coords.emplace_back(numbers, row, col);
					}
				}
			}
			if (numbers_and_coords.empty())
				continue;

			for (int i = 0; i < numbers_and_coords.size() - 1; i++)
			{
				for (int j = i + 1; j < numbers_and_coords.size(); j++)
				{
					if (numbers_and_coords[i].numbers == numbers_and_coords[j].numbers)
						if (clear_numbers_in_box_except(boxrow, boxcol, numbers_and_coords[i].numbers,
							{ numbers_and_coords[i].coord, numbers_and_coords[j].coord }))
							method_used = true;
				}
			}
		}
	}
	return method_used;
}

bool Solver::naked_pair()
{
	bool methodUsed = false;

	if (naked_pair_box())
		methodUsed = true;

	if (naked_pair_cols())
		methodUsed = true;

	if (naked_pair_rows())
		methodUsed = true;

	return methodUsed;
}


void Solver::load_puzzle(const QList<QList<int>>& uncompleted_sudoku)
{
	size = uncompleted_sudoku.size();
	notes = { size, QList<QList<int>>(size) };
	field = uncompleted_sudoku;

	for (int row = 0; row < size; row++)
	{
		for (int col = 0; col < size; col++)
		{
			if (uncompleted_sudoku[row][col] == 0)
			{
				for (int i = 1; i <= 9; i++)
				{
					if (isValid(row, col, i, uncompleted_sudoku))
						notes[row][col].push_back(i);
				}
			}
		}
	}
}

QList<QList<int>> Solver::get_sudoku()
{
	return field;
}

QList<QList<QList<int>>> Solver::get_notes()
{
	return notes;
}

bool Solver::hidden_numbers()
{
	bool method_used = false;
	for (int maximum_numbers = 2; maximum_numbers < 5; maximum_numbers++)
	{
		if (hidden_numbers_in_rows(maximum_numbers))
			method_used = true;
		if (hidden_numbers_in_columns(maximum_numbers))
			method_used = true;
		if (hidden_numbers_in_box(maximum_numbers))
			method_used = true;
	}
	return method_used;
}

bool Solver::hidden_numbers_in_rows(int numbers_to_find)
{
	bool method_used = false;
	for (int row = 0; row < 9; row++)
	{
		if (hidden_numbers_in_row(row, numbers_to_find))
			method_used = true;
	}
	return method_used;
}
bool Solver::hidden_numbers_in_columns(int numbers_to_find)
{
	bool method_used = false;
	for (int col = 0; col < 9; col++)
	{
		if (hidden_numbers_in_column(col, numbers_to_find))
			method_used = true;
	}
	return method_used;
}
bool Solver::hidden_numbers_in_box(int numbers_to_find)
{
	bool method_used = false;
	for (int box = 0; box < 9; box++)
	{
		if (hidden_numbers_in_box(box, numbers_to_find))
			method_used = true;
	}
	return method_used;
}


bool Solver::hidden_numbers_in_row(int row, int numbers_to_find)
{
	QSet<int> numbers_set;
	QHash<int,QList<coords>> occurence;

	for (int col = 0; col < 9; col++)
	{
		for (int number : notes[row][col])
		{
			numbers_set.insert(number);
			occurence[number].push_back({row, col});
		}
	}


	QList<int> numbers(numbers_set.begin(), numbers_set.end());

	auto potential_numbers = get_potential_numbers(numbers, occurence, numbers_to_find);

	if (potential_numbers.size() < numbers_to_find)
		return false;

	auto combinations = get_combinations(potential_numbers, numbers_to_find);


	bool method_used = false;

	for (auto& combination : combinations)
	{
		QSet<coords> overlapings;
		if (is_good_combination(combination, occurence, numbers_to_find, overlapings))
		{
			QList<coords> overlaping_fields(overlapings.begin(), overlapings.end());

			for (auto& field : overlaping_fields)
			{
				if (delete_numbers_in_field_except_numbers(field.row, field.col, combination))
					method_used = true;
			}
		}
	}

	return method_used;
}
bool Solver::hidden_numbers_in_column(int col, int numbers_to_find)
{
	QSet<int> numbers_set;
	QHash<int,QList<coords>> occurence;

	for (int row = 0; row < 9; row++)
	{
		for (int number : notes[row][col])
		{
			numbers_set.insert(number);
			occurence[number].push_back({row, col});
		}
	}

	QList<int> numbers(numbers_set.begin(), numbers_set.end());

	auto potential_numbers = get_potential_numbers(numbers, occurence, numbers_to_find);

	if (potential_numbers.size() < numbers_to_find)
		return false;

	auto combinations = get_combinations(potential_numbers, numbers_to_find);


	bool method_used = false;

	for (auto& combination : combinations)
	{
		QSet<coords> overlapings;
		if (is_good_combination(combination, occurence, numbers_to_find, overlapings))
		{
			QList<coords> overlaping_fields(overlapings.begin(), overlapings.end());

			/*if (delete_numbers_in_col_except_fields(col, overlaping_fields, combination))
				method_used = true;*/

			for (auto& field : overlaping_fields)
			{
				if (delete_numbers_in_field_except_numbers(field.row, field.col, combination))
					method_used = true;
			}
		}
	}

	return method_used;
}
bool Solver::hidden_numbers_in_box(int box, int numbers_to_find)
{
	QSet<int> numbers_set;
	QHash<int,QList<coords>> occurence;

	int box_row = box / 3;
	int box_col = box % 3;

	
	for (int row = box_row * 3; row < 9; row++)
	{
		for (int col = box_col * 3; col < 9; col++)
		{
			for (int number : notes[row][col])
			{
				numbers_set.insert(number);
				occurence[number].push_back({ row, col });
			}
		}
	}

	QList<int> numbers(numbers_set.begin(), numbers_set.end());

	auto potential_numbers = get_potential_numbers(numbers, occurence, numbers_to_find);

	if (potential_numbers.size() < numbers_to_find)
		return false;

	auto combinations = get_combinations(potential_numbers, numbers_to_find);


	bool method_used = false;

	for (auto& combination : combinations)
	{
		QSet<coords> overlapings;
		if (is_good_combination(combination, occurence, numbers_to_find, overlapings))
		{
			QList<coords> overlaping_fields(overlapings.begin(), overlapings.end());

			/*if (delete_numbers_in_box_except_fields(box, overlaping_fields, combination))
				method_used = true;*/

			for (auto& field : overlaping_fields)
			{
				if (delete_numbers_in_field_except_numbers(field.row, field.col, combination))
					method_used = true;
			}
		}
	}

	return method_used;
}


QList<int> Solver::get_potential_numbers(QList<int>& numbers, QHash<int,QList<coords>>& occurence, int max_numbers)
{
	QList<int> potential_numbers;

	for (int number : numbers)
		if (occurence[number].size() <= max_numbers)
			potential_numbers.push_back(number);

	return potential_numbers;
}


QList<QList<int>> Solver::get_combinations(QList<int>& numbers, int size)
{
	QList<int> indexes;

	for (int i = 0; i < size; i++)
		indexes.push_back(i);

	QList<QList<int>> combinations;

	while (true)
	{
		QList<int> combination;

		for (int i = 0; i < size; i++)
			combination.push_back(numbers[indexes[i]]);

		combinations.push_back(combination);

			
		int i;
		for (i = size - 1; i >= 0; i--)
		{
			if (indexes[i] != numbers.size() - size + i)
			{
				indexes[i]++;

				for (int j = i + 1; j < size; ++j) 
					indexes[j] = indexes[j - 1] + 1;

				break;
			}
		}

		if (i < 0)
            break;

	}

	return combinations;
}

bool Solver::is_good_combination(QList<int>& combination, QHash<int, QList<coords>>& number_occurences, int max_numbers, QSet<coords>& overlapings)
{

	// Заповнення накладань (два числа з комбінації, або більше на одному полі) й перевірка чи ці накладання нам підходять
	for (int number : combination)
	{
		if (!is_good_overlaping(number, combination, number_occurences, max_numbers, overlapings))
			return false;
	}


	// Перевірка на те, чи кожна поява кожного числа була лише на тих полях, на яких є інші числа з тієї ж комбінації
	for (int number : combination)
	{
		for (auto& occurence : number_occurences[number])
			if (!overlapings.contains(occurence))
				return false;
	}

	return overlapings.size() == max_numbers;
}

bool Solver::is_good_overlaping(int number, QList<int>& combination, QHash<int, QList<coords>>& number_occurences, int max_numbers, QSet<coords>& overlapings)
{
	QSet<coords> number_overlaping;

	for (coords occurence : number_occurences[number])
	{
		for (int number_to_compare : combination)
		{
			if (number_to_compare != number)
			{
				if (number_occurences[number_to_compare].contains(occurence))
				{
					overlapings.insert(occurence);
					number_overlaping.insert(occurence);
				}
			}
		}
	}

	return (number_overlaping.size() >= 2 && number_overlaping.size() <= max_numbers);
}


bool Solver::delete_numbers_in_field_except_numbers(int row, int col, QList<int>& except_numbers)
{
	bool method_used = false;
	for (int number : notes[row][col])
	{
		if (!except_numbers.contains(number))
		{
			notes[row][col].removeOne(number);
			method_used = true;
		}
	}

	return method_used;
}



QList<QPair<int, QList<int>>> Solver::get_rows_with_number(int number, int maximum_occurence)
{
	QList<QPair<int,QList<int>>> rows;

	for (int row = 0; row < 9; row++)
	{
		QList<int> columns;
		for (int col = 0; col < 9; col++)
		{
			if (notes[row][col].contains(number))
				columns.push_back(col);
		}
		if (columns.size() >= 2 && columns.size() <= maximum_occurence)
			rows.emplace_back(row, columns);
	}

	return rows;
}

QList<QPair<int, QList<int>>> Solver::get_columns_with_number(int number, int maximum_occurence)
{
	QList<QPair<int,QList<int>>> columns;

	for (int col = 0; col < 9; col++)
	{
		QList<int> rows;
		for (int row = 0; row < 9; row++)
		{
			if (notes[row][col].contains(number))
				rows.push_back(row);
		}
		if (rows.size() >= 2 && rows.size() <= maximum_occurence)
			columns.emplace_back(col, rows);
	}

	return columns;
}


QHash<int, int> Solver::get_indexes_occurence(QList<QPair<int, QList<int>>>& lists)
{
	QHash<int,int> index_occurence;

	for (auto& list : lists)
	{
		for (int index : list.second)
			index_occurence[index]++;
	}

	return index_occurence;
}

QList<QList<int>> Solver::get_lists_combinations(QList<QPair<int, QList<int>>>& list, int size)
{
	if (list.size() < size)
		return {};

	QList<int> indexes;

	for (int i = 0; i < size; i++)
		indexes.push_back(i);

	QList<QList<int>> combinations;

	while (true)
	{
		QList<int> combination;

		for (int i = 0; i < size; i++)
			combination.push_back(indexes[i]);

		combinations.push_back(combination);

		
		int i;
		for (i = size - 1; i >= 0; i--)
		{
			if (indexes[i] != list.size() - size + i)
			{
				indexes[i]++;

				for (int j = i + 1; j < size; ++j) 
					indexes[j] = indexes[j - 1] + 1;

				break;
			}
		}

		if (i < 0)
            break;

	}

	return combinations;
}


QSet<int> Solver::get_indexes_from_list(QList<QPair<int, QList<int>>>& lists)
{
	QSet<int> indexes;

	for (auto& list : lists)
	{
		for (int index : list.second)
			indexes.insert(index);
	}

	return indexes;
}

bool Solver::x_wings()
{
	bool method_used = false;

	for (int number = 1; number <= 9; number++)
	{
		if (x_wings_rows(number))
			method_used = true;
		if (x_wings_columns(number))
			method_used = true;
	}

	return method_used;
}

bool Solver::x_wings_rows(int number)
{
	auto rows = get_rows_with_number(number, 2);


	for (auto& combination : get_lists_combinations(rows, 2))
	{
		QSet<int> unique_indexes;

		for (int list_index : combination)
		{
			for (int index : rows[list_index].second)
				unique_indexes.insert(index);
		}

		if (unique_indexes.size() == 2)
		{
			bool notes_erased = false;
			for (int col : unique_indexes)
			{
				if (clear_col_except(col, number, { rows[combination[0]].first, rows[combination[1]].first }))
					notes_erased = true;
			}
			return notes_erased;
		}
	}
	
	return false;
}



bool Solver::x_wings_columns(int number)
{
	auto columns = get_columns_with_number(number, 2);

	for (auto& combination : get_lists_combinations(columns, 2))
	{
		QSet<int> unique_indexes;

		for (int list_index : combination)
		{
			for (int index : columns[list_index].second)
				unique_indexes.insert(index);
		}

		if (unique_indexes.size() == 2)
		{
			bool notes_erased = false;
			for (int row : unique_indexes)
			{
				if (clear_row_except(row, number, { columns[combination[0]].first, columns[combination[1]].first }))
					notes_erased = true;
			}
			return notes_erased;
		}
	}
	
	return false;
}

bool Solver::swordfish()
{
	bool method_used = false;

	for (int number = 1; number <= 9; number++)
	{
		if (swordfish_rows(number))
			method_used = true;
		if (swordfish_columns(number))
			method_used = true;
	}

	return method_used;
}

bool Solver::swordfish_rows(int number)
{
	auto rows = get_rows_with_number(number, 3);

	for (auto& combination : get_lists_combinations(rows, 3))
	{
		QSet<int> unique_indexes;

		for (int list_index : combination)
		{
			for (int index : rows[list_index].second)
				unique_indexes.insert(index);
		}

		if (unique_indexes.size() == 3)
		{
			bool notes_erased = false;
			for (int col : unique_indexes)
			{
				
				if (clear_col_except(col, number, { rows[combination[0]].first, rows[combination[1]].first, rows[combination[2]].first }))
					notes_erased = true;
			}
			return notes_erased;
		}
	}
	
	return false;
}

bool Solver::swordfish_columns(int number)
{
	auto columns = get_columns_with_number(number, 3);

	for (auto& combination : get_lists_combinations(columns, 3))
	{
		QSet<int> unique_indexes;

		for (int list_index : combination)
		{
			for (int index : columns[list_index].second)
				unique_indexes.insert(index);
		}

		if (unique_indexes.size() == 3)
		{
			bool notes_erased = false;
			for (int row : unique_indexes)
			{
				
				if (clear_row_except(row, number, { columns[combination[0]].first, columns[combination[1]].first, columns[combination[2]].first }))
					notes_erased = true;
			}
			return notes_erased;
		}
	}
	
	return false;
}



bool Solver::jellyfish()
{
	bool method_used = false;

	for (int number = 1; number <= 9; number++)
	{
		if (jellyfish_rows(number))
			method_used = true;
		if (jellyfish_columns(number))
			method_used = true;
	}

	return method_used;
}

bool Solver::jellyfish_rows(int number)
{
	auto rows = get_rows_with_number(number, 4);

	for (auto& combination : get_lists_combinations(rows, 4))
	{
		QSet<int> unique_indexes;

		for (int list_index : combination)
		{
			for (int index : rows[list_index].second)
				unique_indexes.insert(index);
		}

		if (unique_indexes.size() == 4)
		{
			bool notes_erased = false;
			for (int col : unique_indexes)
			{
				
				if (clear_col_except(col, number, { rows[combination[0]].first, rows[combination[1]].first, rows[combination[2]].first, rows[combination[3]].first }))
					notes_erased = true;
			}
			return notes_erased;
		}
	}
	
	return false;
}

bool Solver::jellyfish_columns(int number)
{
	auto columns = get_columns_with_number(number, 4);

	for (auto& combination : get_lists_combinations(columns, 4))
	{
		QSet<int> unique_indexes;

		for (int list_index : combination)
		{
			for (int index : columns[list_index].second)
				unique_indexes.insert(index);
		}

		if (unique_indexes.size() == 4)
		{
			bool notes_erased = false;
			for (int row : unique_indexes)
			{
				
				if (clear_row_except(row, number, { columns[combination[0]].first, columns[combination[1]].first, columns[combination[2]].first, columns[combination[3]].first }))
					notes_erased = true;
			}
			return notes_erased;
		}
	}
	
	return false;
}






bool Solver::clear_row_except(int row, int number, QSet<int> except_indexes)
{
	bool method_used = false;

	for (int col = 0; col < 9; col++)
	{
		if (!except_indexes.contains(col))
		{
			if (notes[row][col].removeOne(number))
				method_used = true;
		}
	}

	return method_used;
}


bool Solver::clear_col_except(int col, int number, QSet<int> except_indexes)
{
	bool method_used = false;
	for (int row = 0; row < 9; row++)
	{
		if (!except_indexes.contains(row))
		{
			if (notes[row][col].removeOne(number))
				method_used = true;
		}
	}

	return method_used;
}







bool Solver::simple_chains()
{
	bool candidates_removed = false;

	for (int number = 1; number < 10; number++)
	{
		if (simple_chains_number(number))
			candidates_removed = true;
	}

	return candidates_removed;
}

bool Solver::simple_chains_number(int number)
{
	auto chains = find_bi_location_chains(number);

	bool number_cleared = false;

	for (auto& chain : chains)
	{
		int solution_number = find_wrong_possible_solution(chain);

		if (solution_number == 0) // нічого не знайшло
		{
			// Ця функція використовується виключно, коли не можливо одразу виявити не правильне рішення, тому прибираються кандидати, 
			// які не можуть існувати в обох рішеннях
			if (find_candidates_affected_by_both_solutions(chain, number))
				number_cleared = true;
		}
		else if (solution_number == 1) // chain.first очевидно не правильне рішення
		{
			for (auto& coords : chain.first)
				if (clear_field(coords.row, coords.col, number))
					number_cleared = true;
		}
		else // chain.second очевидно не правильне рішення
		{
			for (auto& coords : chain.second)
				if (clear_field(coords.row, coords.col, number))
					number_cleared = true;
		}
	}

	return number_cleared;
}


bool is_wrong_solution(QList<coords>& solution)
{
	QHash<int,int> row_appearence;
	QHash<int,int> col_appearence;
	QHash<int,int> box_appearence;

	for (auto& coordinates : solution)
		row_appearence[coordinates.row]++;
	
	for (auto& coordinates : solution)
		col_appearence[coordinates.col]++;
	
	for (auto& coordinates : solution)
		box_appearence[coordinates.row / 3 * 3 + coordinates.col / 3]++;


	for (int value : row_appearence.values())
		if (value > 1)
			return true;
	
	for (int value : col_appearence.values())
		if (value > 1)
			return true;
	
	for (int value : box_appearence.values())
		if (value > 1)
			return true;

	return false;
}

int Solver::find_wrong_possible_solution(QPair<QList<coords>, QList<coords>>& chain)
{
	if (is_wrong_solution(chain.first))
		return 1;
	if (is_wrong_solution(chain.second))
		return 2;

	return 0;
}


bool Solver::delete_candidates_affected_by_both_solutions_in_box(coords& candidate, coords box, coords except_candidate, int number)
{
	bool numbers_erased = false;

	coords candidate_box { candidate.row / 3, candidate.col / 3 };

	if (candidate_box.row == box.row)
	{
		for (int box_col = 0; box_col < 3; box_col++)
		{
			int col = box.col * 3 + box_col;

			if (candidate.row == except_candidate.row && col == except_candidate.col)
				continue;

			if (clear_field(candidate.row, col, number))
				numbers_erased = true;
		}
	}

	if (candidate_box.col == box.col)
	{
		for (int box_row = 0; box_row < 3; box_row++)
		{
			int row = box.row * 3 + box_row;

			if (row == except_candidate.row && candidate.col == except_candidate.col)
				continue;

			if (clear_field(row, candidate.col, number))
				numbers_erased = true;
		}
	}

	return numbers_erased;
}

bool Solver::process_boxes(QList<coords>& solution, QHash<coords, coords>& boxes_to_process, QHash<coords, coords>& boxes_avoid, int number)
{
	bool numbers_erased = false;

	for (auto& box : boxes_to_process.keys())
	{
		if (boxes_avoid.find(box) == boxes_to_process.end())
		{
			for (auto& candidate : solution)
			{				
				// Видаляємо кандидатів, які знаходяться у квадраті поруч з кандидатом одного рішення і вони на одному рядку\стовпці з кандидатом іншого рішення
				if (delete_candidates_affected_by_both_solutions_in_box(candidate, box, boxes_to_process[box], number))
					numbers_erased = true;	
			}
		}
	}

	return numbers_erased;
}

bool Solver::find_candidates_affected_by_both_solutions_in_box(QList<coords>& first_solution, QList<coords>& second_solution, int number)
{
	bool numbers_erased = false;
	

	// Одразу збегіраємо квадрати, що були задіяні в лацнюгу, щоб не використовувати ті квадрати, що були в двох рішеннях одночасно
	// бо ця функція працює лише, якщо квадрат задіяний в одному рішені
	QHash<coords, coords> first_solution_boxes;
	QHash<coords, coords> second_solution_boxes;

	for (auto& candidate : first_solution)
	{
		// Визначення позиції квадрата
		coords box_coords { candidate.row / 3, candidate.col / 3 };
		first_solution_boxes.emplace(box_coords, candidate);
	}

	for (auto& candidate : second_solution)
	{
		coords box_coords { candidate.row / 3, candidate.col / 3};
		second_solution_boxes.emplace(box_coords, candidate);
	}

	if (process_boxes(second_solution, first_solution_boxes, second_solution_boxes, number))
		numbers_erased = true;
	if (process_boxes(first_solution, second_solution_boxes, first_solution_boxes, number))
		numbers_erased = true;


	return numbers_erased;
}

bool Solver::find_candidates_affected_by_both_solutions(QPair<QList<coords>, QList<coords>>& chain, int number)
{
	// Зібрати інформацію про те в яких рядках, стовпцях і квадратах знаходяться кандидат двох рішень
	// Видалити кандидатів які знаходяться на рядках першого рішення і стовпцях другого, і наоборот, рядках другого стовпцях першого
	// Пройтись по квадратах і повидаляти кандидатів, які є на рядках\стовпцях квадрату, окрім кандидата з протилежного рішення
	bool numbers_erased = false;


	// По ідеї працює, але треба подумати вдень ☺
	for (auto first_solution_coords : chain.first)
	{
		for (auto second_solution_coords : chain.second)
		{
			if (first_solution_coords.row == second_solution_coords.row || first_solution_coords.col == second_solution_coords.col)
				continue;

			if (clear_field(first_solution_coords.row, second_solution_coords.col, number))
				numbers_erased = true;
			if (clear_field(second_solution_coords.row, first_solution_coords.col, number))
				numbers_erased = true;
		}
	}


	if (find_candidates_affected_by_both_solutions_in_box(chain.first, chain.second, number))
		numbers_erased = true;

	return numbers_erased;
}


QList<QPair<QList<coords>, QList<coords>>> Solver::find_bi_location_chains(int number)
{
	auto links = find_all_bi_location_links(number);

	QSet<coords> used_candidates;

	QList<QPair<QList<coords>, QList<coords>>> chains;

	for (auto& link : links)
	{
		// Для перевірки зв'язку достатньо перевірити одного з кандидатів
		if (used_candidates.find(link.first) == used_candidates.end())
		{
			chains.push_back(find_bi_location_chain(link, links, used_candidates));
		}
	}

	return chains;
}


QList<QPair<coords,coords>> Solver::find_all_bi_location_links(int number)
{
	QList<QPair<coords,coords>> links;

	// Пошук зв'язків по рядках
	for (int row = 0; row < 9; row++)
	{
		QList<coords> link;

		for (int col = 0; col < 9; col++)
		{
			if (notes[row][col].contains(number))
				link.emplace_back(row,col);
		}

		if (link.size() == 2)
			links.emplace_back(link[0], link[1]);
	}

	// Пошук зв'язків по стовпцях
	for (int col = 0; col < 9; col++)
	{
		QList<coords> link;

		for (int row = 0; row < 9; row++)
		{
			if (notes[row][col].contains(number))
				link.emplace_back(row,col);
		}
		if (link.size() == 2)
			links.emplace_back(link[0], link[1]);
	}


	// Пошук зв'язків по квадратах
	for (int box_row = 0; box_row < 3; box_row++)
	{
		for (int box_col = 0; box_col < 3; box_col++)
		{
			QList<coords> link;
			for (int row = 0; row < 3; row++)
			{
				for (int col = 0; col < 3; col++)
				{
					if (notes[box_row * 3 + row][box_col * 3 + col].contains(number))
						link.emplace_back(box_row * 3 + row, box_col * 3 + col);
				}
			}
			if (link.size() == 2)
				links.emplace_back(link[0],link[1]);
		}
	}


	return links;
}

QPair<QList<coords>, QList<coords>> Solver::find_bi_location_chain(QPair<coords, coords>& link, QList<QPair<coords,coords>>& links, QSet<coords>& used_candidates)
{
	QList<coords> first_possible_solution;
	QList<coords> second_possible_solution;

	first_possible_solution.push_back(link.first);
	second_possible_solution.push_back(link.second);
	used_candidates.insert(link.first);
	used_candidates.insert(link.second);

	int first_array_index = 0;
	int second_array_index = 0;

	while (first_array_index < first_possible_solution.size() || second_array_index < second_possible_solution.size())
	{
		while (first_array_index < first_possible_solution.size())
		{
			auto candidates = find_bi_location_links_of_candidate(first_possible_solution[first_array_index], links);
			
			for (auto& candidate : candidates)
			{
				if (used_candidates.find(candidate) == used_candidates.end())
				{
					used_candidates.insert(candidate);
					second_possible_solution.push_back(candidate);
				}
			}

			first_array_index++;
		}

		while (second_array_index < second_possible_solution.size())
		{
			auto candidates = find_bi_location_links_of_candidate(second_possible_solution[second_array_index], links);
			
			for (auto& candidate : candidates)
			{
				if (used_candidates.find(candidate) == used_candidates.end())
				{
					used_candidates.insert(candidate);
					first_possible_solution.push_back(candidate);
				}
		
			}

			second_array_index++;
		}
	}

	return { first_possible_solution, second_possible_solution };
}

QList<coords> Solver::find_bi_location_links_of_candidate(coords candidate, QList<QPair<coords,coords>>& links)
{
	QList<coords> linked_candidates;

	for (auto& link : links)
	{
		if (link.first == candidate)
			linked_candidates.push_back(link.second);
		else if (link.second == candidate)
			linked_candidates.push_back(link.first);
	}

	return linked_candidates;
}


bool Solver::xy_wing()
{
	auto chains = get_chains(2);

	bool is_candidates_removed = false;
	
	for (int hinge_number : chains.keys())
	{
		for (auto& chain : chains[hinge_number])
		{
			if (clear_candidates_that_seen_from_chains(chain, hinge_number))
				is_candidates_removed = true;
		}
	}
	return is_candidates_removed;
}

bool Solver::xyz_wing()
{
	auto chains = get_chains(3);

	bool is_candidates_removed = false;
	
	for (int hinge_number : chains.keys())
	{
		for (auto& chain : chains[hinge_number])
		{
			if (clear_candidates_that_seen_from_chains(chain, hinge_number))
				is_candidates_removed = true;
		}
	}

	return is_candidates_removed;
}

//bool Solver::wxyz_wing()
//{
//	auto chains = get_chains(4);
//
//	bool is_candidates_removed = false;
//	
//	for (int hinge_number : chains.keys())
//	{
//		for (auto& chain : chains[hinge_number])
//		{
//			if (chain[0].row == 3 && chain[0].col == 6 &&
//				chain[1].row == 1 && chain[1].col == 6)
//				int nothing = 0;
//			if (clear_candidates_that_seen_from_chains(chain, hinge_number))
//				is_candidates_removed = true;
//		}
//	}
//
//	// помилка в тому, що не всі поля мають мати 3 кандидати? 
//
//	return is_candidates_removed;
//}


bool Solver::clear_candidates_that_seen_from_chains(QList<coords>& chain, int number)
{
	auto start_cell_numbers = notes[chain[0].row][chain[0].col];
	// xyz/wxyz wing
	if (start_cell_numbers.size() != 2)
		return clear_candidates_from_box_that_seen_from_chains(chain, number);


	// Перевірка, чи якась з клітинок лежить на квадраті початкової клітинки
	int start_cell_box = chain[0].row / 3 * 3 + chain[0].col / 3;

	for (int index = 1; index < chain.size(); index++)
	{
		int cell_box = chain[index].row / 3 * 3 + chain[index].col / 3;

		if (cell_box == start_cell_box)
			return clear_candidates_from_box_that_seen_from_chains(chain, number);
	}

	// Якщо клітинки на різних стовпцях і рядках
	return clear_candidate_that_seen_from_2_cells(chain, number);
}

// Видалення кандидата з поля, яке видно з двох клітинок ланцюга, окрім початкової клітинки
// Клітинки ланцюга мають лежати на різних стовпцях і рядках, щоб ця стратегія працювала
bool Solver::clear_candidate_that_seen_from_2_cells(QList<coords>& chain, int number)
{
	auto& start_cell = chain[0];
	auto& first_cell = chain[1];
	auto& second_cell = chain[2];

	if (first_cell.row != start_cell.row && second_cell.col != start_cell.col)
		return clear_field(first_cell.row, second_cell.col, number);
	else if (second_cell.row != start_cell.row && first_cell.col != start_cell.col)
		return clear_field(second_cell.row, first_cell.col, number);

	return false;
}


bool Solver::clear_candidates_from_box_that_seen_from_chains(QList<coords>& chain, int number)
{
	// У xyz/wxyz крилах видалення кандидатів відбувається виключно у квадраті початкового поля, тому що це єдиний квадрат, який видно
	// всім клітинкам ланцюга, окрім звичайно випадку, коли клітинки ланцюга знаходяться на одному рядку\стовпці
	// Але це, по ідеї, покривається стратегією прихованої трійки\четвірки (яку на момент створення цього алгоритму я не створив ☻)
	auto start_cell_numbers = notes[chain[0].row][chain[0].col];
	if (start_cell_numbers.size() != 2)
	{
		coords start_cell_box { chain[0].row / 3, chain[0].col / 3 };

		for (int index = 1; index < chain.size(); index++)
		{
			coords cell_box { chain[index].row / 3, chain[index].col / 3 };

			if (start_cell_box != cell_box)
				return clear_candidates_from_box_that_seen_from_cell(chain, chain[index], start_cell_box, number);
		}
	}

	// У xy крилах для видалення кандидатів треба, щоб лише дві клітинки з ланцюга, окрім початкової клітинки, бачили цих кандидатів
	// То ж, видалення може відбутись у квадратах обох клітинок.
	coords first_box { chain[1].row / 3, chain[1].col / 3 };
	coords second_box { chain[2].row / 3, chain[2].col / 3 };


	bool candidates_removed = false;
	if (clear_candidates_from_box_that_seen_from_cell(chain, chain[1], second_box, number))
		candidates_removed = true;
	if (clear_candidates_from_box_that_seen_from_cell(chain, chain[2], first_box, number))
		candidates_removed = true;

	return candidates_removed;
}

bool Solver::clear_candidates_from_box_that_seen_from_cell(QList<coords>& except_cells, coords& cell, coords& box_to_clear, int number)
{
	bool numbers_erased = false;


	coords candidate_box { cell.row / 3, cell.col / 3 };

	if (candidate_box.row == box_to_clear.row)
	{
		for (int box_col = 0; box_col < 3; box_col++)
		{
			int col = box_to_clear.col * 3 + box_col;
			coords cell_to_clear(cell.row, col);

			if (except_cells.contains(cell_to_clear))
				continue;

			if (clear_field(cell.row, col, number))
				numbers_erased = true;
		}
	}

	if (candidate_box.col == box_to_clear.col)
	{
		for (int box_row = 0; box_row < 3; box_row++)
		{
			int row = box_to_clear.row * 3 + box_row;
			coords cell_to_clear(row, cell.col);

			if (except_cells.contains(cell_to_clear))
				continue;

			if (clear_field(row, cell.col, number))
				numbers_erased = true;
		}
	}

	return numbers_erased;
}

QHash<int, QList<QList<coords>>> Solver::get_chains(int start_cell_size)
{
	QHash<int, QList<QList<coords>>> chains;

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			if (notes[row][col].size() == start_cell_size)
				build_wing_chains(row, col, chains);
		}
	}

	return chains;
}

void Solver::build_wing_chains(int row, int col, QHash<int, QList<QList<coords>>>& chains)
{
	auto& start_cell = notes[row][col];

	QList<coords> cells;

	if (start_cell.size() == 2)
		cells = get_cells_that_see_cell_xy_wing(row, col);
	else
		cells = get_cells_that_see_cell(row, col);



	QHash<int,QList<coords>> cells_by_numbers;

	for (auto& cell : cells)
	{
		for (int candidate : notes[cell.row][cell.col])
			cells_by_numbers[candidate].push_back(cell);
	}

	bool dont_use_start_cell_candidates = start_cell.size() == 2;
	

	for (int number : cells_by_numbers.keys())
	{
		if (dont_use_start_cell_candidates)
			if (start_cell.contains(number))
				continue;
		
		// xy/xyz крила мають 2 клітинки в ланцюгу, якщо не враховувати початкову клітинку
		// wxyz мають 3
		int cells_amount = start_cell.size() == 4 ? 3 : 2;

		if (cells_by_numbers[number].size() >= cells_amount)
			build_wings_with_hinge_number(cells_by_numbers[number], cells_amount, {row,col}, chains, number);
	}
}

QList<coords> Solver::get_cells_that_see_cell(int start_cell_row, int start_cell_col)
{
	auto& start_cell = notes[start_cell_row][start_cell_col];


	// xyz крила дозволяють лише два кандидати на поле, а wxyz і 2 і 3. 
	int maximum_cell_size = start_cell.size() <= 3 ? 2 : 3;


	QSet<int> start_cell_numbers;

	for (int number : start_cell)
		start_cell_numbers.insert(number);

	QList<coords> cells;

	// Збираємо поля, що бачать початкове поле по рядку
	for (int row = 0; row < 9; row++)
	{
		if (row == start_cell_row)
			continue;

		auto& cell = notes[row][start_cell_col];

		if (cell.size() == 2 || cell.size() == maximum_cell_size)
		{
			bool use_this_cell = false;

			for (int candidate : cell)
			{
				if (!start_cell_numbers.contains(candidate))
				{
					use_this_cell = false;
					break;
				}
				use_this_cell = true;
			}

			if (use_this_cell)
				cells.emplace_back(row, start_cell_col);
				
		}
	}

	// Що бачать початкове поле по стовпцю
	for (int col = 0; col < 9; col++)
	{
		if (col == start_cell_col)
			continue;

		auto& cell = notes[start_cell_row][col];

		if (cell.size() == 2 || cell.size() == maximum_cell_size)
		{
			bool use_this_cell = false;

			for (int candidate : cell)
			{
				if (!start_cell_numbers.contains(candidate))
				{
					use_this_cell = false;
					break;
				}
				use_this_cell = true;
			}

			if (use_this_cell)
				cells.emplace_back(start_cell_row, col);
		}
	}


	int box_row = start_cell_row / 3;
	int box_col = start_cell_col / 3;
	
	// Що знаходяться в одному квадраті
	for (int row_in_box = 0; row_in_box < 3; row_in_box++)
	{
		for (int col_in_box = 0; col_in_box < 3; col_in_box++)
		{
			int row = box_row * 3 + row_in_box;
			int col = box_col * 3 + col_in_box;

			if (row == start_cell_row && col == start_cell_col)
				continue;

			auto& cell = notes[row][col];

			if (cells.contains({row, col}))
				continue;

			if (cell.size() == 2 || cell.size() == maximum_cell_size)
			{
				bool use_this_cell = false;

				for (int candidate : cell)
				{
					if (!start_cell_numbers.contains(candidate))
					{
						use_this_cell = false;
						break;
					}
					use_this_cell = true;
				}

				if (use_this_cell)
					cells.emplace_back(row, col);
			}

		}
	}
	
	return cells;
}


QList<coords> Solver::get_cells_that_see_cell_xy_wing(int start_cell_row, int start_cell_col)
{
	// У xy крил спільне число не знаходиться у початковому числі
	// Але в xyz, wxyz крилах поля мають мати виключно кандидатів з початкового поля

	auto& start_cell = notes[start_cell_row][start_cell_col];


	QSet<int> start_cell_numbers;

	for (int number : start_cell)
		start_cell_numbers.insert(number);

	QList<coords> cells;

	for (int row = 0; row < 9; row++)
	{
		if (row == start_cell_row)
			continue;

		auto& cell = notes[row][start_cell_col];

		if (cell.size() == 2)
		{			
			for (int candidate : cell)
			{
				if (start_cell_numbers.contains(candidate))
				{
					cells.emplace_back(row, start_cell_col);
					break;
				}
			}
		}
	}

	for (int col = 0; col < 9; col++)
	{
		if (col == start_cell_col)
			continue;

		auto& cell = notes[start_cell_row][col];

		if (cell.size() == 2)
		{			
			for (int candidate : cell)
			{
				if (start_cell_numbers.contains(candidate))
				{
					cells.emplace_back(start_cell_row, col);
					break;
				}
			}
		}
	}


	int box_row = start_cell_row / 3;
	int box_col = start_cell_col / 3;
	
	for (int row_in_box = 0; row_in_box < 3; row_in_box++)
	{
		for (int col_in_box = 0; col_in_box < 3; col_in_box++)
		{
			int row = box_row * 3 + row_in_box;
			int col = box_col * 3 + col_in_box;

			if (row == start_cell_row && col == start_cell_col)
				continue;

			auto& cell = notes[row][col];

			if (cells.contains({row, col}))
				continue;

			if (cell.size() == 2)
			{
				for (int candidate : cell)
				{
					if (start_cell_numbers.contains(candidate))
					{
						cells.emplace_back(row, col);
						break;
					}
				}
			}
		}
	}
	
	return cells;
}


void Solver::build_wings_with_hinge_number(QList<coords>& cells, int cells_amount, coords start_cell, QHash<int, QList<QList<coords>>>& chains, int hinge_number)
{
	auto combinations = get_indexes_combinations(cells_amount, cells.size());

	if (cells_amount == 3)
		build_chains_with_combinations_wxyz(cells, cells_amount, start_cell, chains, combinations, hinge_number);
	else
		build_chains_with_combinations(cells, cells_amount, start_cell, chains, combinations, hinge_number);
}

QList<QList<int>> Solver::get_indexes_combinations(int indexes_amount, int array_size)
{
	QList<int> indexes;

	for (int i = 0; i < indexes_amount; i++)
		indexes.push_back(i);

	QList<QList<int>> combinations;

	while (true)
	{
		QList<int> combination;

		for (int i = 0; i < indexes_amount; i++)
			combination.push_back(indexes[i]);

		combinations.push_back(combination);

			
		int i;
		for (i = indexes_amount - 1; i >= 0; i--)
		{
			if (indexes[i] < array_size - indexes_amount + i)
			{
				indexes[i]++;

				for (int j = i + 1; j < indexes_amount; ++j) 
					indexes[j] = indexes[j - 1] + 1;

				break;
			}
		}

		if (i < 0)
            break;

	}

	return combinations;
}

void Solver::build_chains_with_combinations(QList<coords>& cells, int cells_amount, coords start_cell, QHash<int, QList<QList<coords>>>& chains, QList<QList<int>>& combinations, int hinge_number)
{
	int start_cell_box = start_cell.row / 3 * 3 + start_cell.col / 3;

	auto& start_cell_numbers = notes[start_cell.row][start_cell.col];

	
	QSet<int> used_numbers_in_start_cell;

	for (int number : start_cell_numbers)
		used_numbers_in_start_cell.insert(number);

	if (start_cell_numbers.size() == 3)
	{
		if (!used_numbers_in_start_cell.contains(hinge_number))
			return;
	}


	for (auto& combination: combinations)
	{
		bool cell_in_start_cell_row = false;
		bool cell_in_start_cell_col = false;
		bool cell_in_start_cell_box = false;

		QSet<int> used_numbers;

		for (int index : combination)
		{
			auto& cell = cells[index];

			QSet<int> used_numbers_in_cell;

			for (int number : notes[cell.row][cell.col])
			{
				if (start_cell_numbers.size() == 3)
					if (!used_numbers_in_start_cell.contains(number))
						continue;

				used_numbers.insert(number);
				used_numbers_in_cell.insert(number);
			}

			if (!used_numbers_in_cell.contains(hinge_number))
				continue;

			int cell_box = cell.row / 3 * 3 + cell.col / 3;

			// Щоб зрозуміти, де знаходяться кандидати, щоб вирішити чи підходить нам комбінація
			if (cell_box == start_cell_box)
				cell_in_start_cell_box = true;
			else if (start_cell.row == cell.row)
				cell_in_start_cell_row = true;
			else if (start_cell.col == cell.col)
				cell_in_start_cell_col = true;
			
		}
		
		
		if (start_cell_numbers.size() == 2)
			add_chain_if_valid_xy_wing(cell_in_start_cell_row, cell_in_start_cell_col, cell_in_start_cell_box, used_numbers, start_cell, combination, cells, chains, hinge_number);
		else
			add_chain_if_valid_xyz_wing(cell_in_start_cell_row, cell_in_start_cell_col, cell_in_start_cell_box, used_numbers, start_cell, combination, cells, chains, hinge_number);

	}
}

void Solver::add_chain_if_valid_xy_wing(bool cell_in_start_cell_row, bool cell_in_start_cell_col, bool cell_in_start_cell_box, QSet<int>& used_numbers, coords& start_cell, QList<int>& combination, QList<coords>& cells,  QHash<int, QList<QList<coords>>>& chains, int hinge_number)
{
	// xy, працює у двох станах:
	// Або одна клітинка знаходиться на стовпці початкової клітинки, а інша клітинка на рядку початкової клітинки
	// Або одна клітинка в квадраті початкової клітинки, а інша або на рядку, або на стовпці
	// Решта нам не підходять
	if (cell_in_start_cell_row && cell_in_start_cell_col || 
			(cell_in_start_cell_box && (cell_in_start_cell_col || cell_in_start_cell_row)))
	{
		if (used_numbers.size() != 3)
			return;

		QList<coords> chain;
		chain.push_back(start_cell);

		for (int index : combination)
			chain.push_back(cells[index]);



		bool at_least_one_cell_not_in_start_cell_box = false;
		coords start_cell_box { chain[0].row / 3, chain[0].col / 3 };

		for (auto& cell : chain)
		{
			coords cell_box { cell.row / 3, cell.col / 3 };
			if (start_cell_box != cell_box)
				at_least_one_cell_not_in_start_cell_box = true;
		}


		if (at_least_one_cell_not_in_start_cell_box)
			chains[hinge_number].push_back(chain);
	}
}
void Solver::add_chain_if_valid_xyz_wing(bool cell_in_start_cell_row, bool cell_in_start_cell_col, bool cell_in_start_cell_box, QSet<int>& used_numbers, coords& start_cell, QList<int>& combination, QList<coords>& cells,  QHash<int, QList<QList<coords>>>& chains, int hinge_number)
{
	if (cell_in_start_cell_box && (cell_in_start_cell_col || cell_in_start_cell_row))
	{
		if (used_numbers.size() != 3)
			return;

		QList<coords> chain;
		chain.push_back(start_cell);

		for (int index : combination)
			chain.push_back(cells[index]);



		bool at_least_one_cell_not_in_start_cell_box = false;
		coords start_cell_box { chain[0].row / 3, chain[0].col / 3 };

		for (auto& cell : chain)
		{
			coords cell_box { cell.row / 3, cell.col / 3 };
			if (start_cell_box != cell_box)
				at_least_one_cell_not_in_start_cell_box = true;
		}


		if (at_least_one_cell_not_in_start_cell_box)
			chains[hinge_number].push_back(chain);
	}
}

void Solver::build_chains_with_combinations_wxyz(QList<coords>& cells, int cells_amount, coords start_cell, QHash<int, QList<QList<coords>>>& chains, QList<QList<int>>& combinations, int hinge_number)
{
	int start_cell_box = start_cell.row / 3 * 3 + start_cell.col / 3;


	auto& start_cell_numbers = notes[start_cell.row][start_cell.col];

	QSet<int> used_numbers_in_start_cell;

	for (int number : start_cell_numbers)
		used_numbers_in_start_cell.insert(number);

	if (!used_numbers_in_start_cell.contains(hinge_number))
		return;

	for (auto& combination: combinations)
	{
		bool cell_in_start_cell_box = false;
		bool cell_in_start_cell_row = false;
		bool cell_in_start_cell_col = false;
		QSet<int> used_numbers;

		for (int index : combination)
		{
			auto& cell = cells[index];

			QSet<int> used_numbers_in_cell;

			for (int number : notes[cell.row][cell.col])
			{
				if (!used_numbers_in_start_cell.contains(number))
					continue;

				used_numbers.insert(number);
				used_numbers_in_cell.insert(number);
			}

			if (!used_numbers_in_cell.contains(hinge_number))
				continue;


			int cell_box = cell.row / 3 * 3 + cell.col / 3;

			// Умовами для того, щоб ланцюг працював є
			// Використання полів на одному рядку з початковим полем АБО поля на одному стовпчику з початковим полем. Якщо поля будуть і там, і там, то вони вказуватимуть на початкове поле.
			// Хоча б одне поле має бути
			// 
			if (start_cell_box == cell_box)
				cell_in_start_cell_box = true;
			else if (start_cell.row == cell.row)
				cell_in_start_cell_row = true;
			else if (start_cell.col == cell.col)
				cell_in_start_cell_col = true;
		}

		if (cell_in_start_cell_box && cell_in_start_cell_row != cell_in_start_cell_col)
		{
			if (used_numbers.size() != 4)
				continue;

			QList<coords> chain;
			chain.push_back(start_cell);

			for (int index : combination)
				chain.push_back(cells[index]);

			bool at_least_one_cell_not_in_start_cell_box = false;
			coords start_cell_box { chain[0].row / 3, chain[0].col / 3 };

			for (auto& cell : chain)
			{
				coords cell_box { cell.row / 3, cell.col / 3 };
				if (start_cell_box != cell_box)
					at_least_one_cell_not_in_start_cell_box = true;
			}
			if (at_least_one_cell_not_in_start_cell_box)
				chains[hinge_number].push_back(chain);
		}

	}
}