#pragma once


#include "Field.h"
#include "Solver.h"

class Field_test : public Field
{
public:
	Field_test(QWidget* parent) : Field(parent)
	{
	
	}
	void load_test(QList<QList<int>> test_field, QList<QList<QList<int>>> notes)
	{
		clear();
		auto field = get_cells();
	
		for (int row = 0; row < 9; row++)
		{
			for (int col = 0; col < 9; col++)
			{
				if (test_field[row][col] != 0)
					field[row][col]->set_number(test_field[row][col]);
				else
				{
					for (int number : notes[row][col])
					{
						if (number != 0)
							field[row][col]->add_candidate(number);
					}
				}
			}
		}
	}

protected:
	void closeEvent(QEvent* event)
	{
		hide();
		event->ignore();
	}
};





class Solver_test : public Solver
{
public:
	void load_test(QList<QList<int>> test_field, QList<QList<QList<int>>> test_notes)
	{
		field = test_field;
		notes = test_notes;
	}
	void load_test(QList<QList<QList<int>>> test_notes)
	{
		field = { 9, QList<int>{9, 0}};
		notes = test_notes;
	}

	void solve_with_newest_algorythm()
	{
		while (single_candidate() || single_position() || candidate_lines() ||
				double_pairs() || naked_pair() || hidden_numbers() || x_wings() || swordfish() || jellyfish() || 
			simple_chains() || xy_wing() || xyz_wing())
		{
		}
	}
	void solve_without_newest_algorythm()
	{
		while (single_candidate() || single_position() || candidate_lines() ||
				double_pairs() || naked_pair() || hidden_numbers() || x_wings() || swordfish() || jellyfish() ||
			simple_chains() || xy_wing() || xyz_wing())
		{
		}
	}
};


