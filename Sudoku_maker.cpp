#include "Sudoku_maker.h"

Sudoku_maker::Sudoku_maker(QObject * parent) : QObject(parent)
{
}

Sudoku_maker::~Sudoku_maker()
{

}

void Sudoku_maker::create_hard_sudoku(int tries, int completed_numbers)
{
	Sudoku_generator generator;
	Solver solver;

	generator.generate();

	for (int i = 0; i < tries; i++)
	{
		generator.createPuzzle(completed_numbers);
		solver.load_puzzle(generator.get_uncompleted_sudoku());

		if (solver.solve_with_hard_techniques())
		{
			generator.generate();
			emit hard_sudoku_created(generator.get_uncompleted_sudoku(), generator.get_completed_sudoku());
		}
	}
	emit finished();
}

void Sudoku_maker::create_normal_sudoku(int tries, int completed_numbers)
{
	Sudoku_generator generator;
	Solver solver;

	generator.generate();

	for (int i = 0; i < tries; i++)
	{
		generator.createPuzzle(completed_numbers);
		solver.load_puzzle(generator.get_uncompleted_sudoku());

		if (solver.solve_with_normal_techniques())
		{
			generator.generate();
			emit normal_sudoku_created(generator.get_uncompleted_sudoku(), generator.get_completed_sudoku());
		}
	}
	emit finished();
}

void Sudoku_maker::create_easy_sudoku(int tries, int completed_numbers)
{
	Sudoku_generator generator;
	Solver solver;

	generator.generate();

	for (int i = 0; i < tries; i++)
	{
		generator.createPuzzle(completed_numbers);
		solver.load_puzzle(generator.get_uncompleted_sudoku());

		if (solver.solve_with_easy_techniques())
		{
			generator.generate();
			emit easy_sudoku_created(generator.get_uncompleted_sudoku(), generator.get_completed_sudoku());
		}
	}
	emit finished();
}