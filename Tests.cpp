#include "Tests.h"
#include <QScrollArea>
#include <QMessageBox>




Theme get_theme()
{
	Theme white;
	white.name = "white";
	white.background_color = "#ffffff";
	white.background = "Зображення/Білий фон.png";

	white.field = "#ffffff";
	white.field_border = "#000000";

	white.regular_text = "#000000";
	white.hovered_text = "#555555";
	white.inactive_text = "gray";

	white.regular_cell = "white";
	white.wrong_cell = "#fd8989";
	white.highlighted_cell = "#ebebeb";
	white.highlighted_active_cell = "#c8c8c8";
	white.highlighted_candidate = "#88ffff";

	return white;
}


Test_result::Test_result(QWidget* parent) : QMainWindow(parent)
{
	auto* window = new QWidget;

	setCentralWidget(window);



	test_window = new QMainWindow(window);
	test_window->setWindowTitle("Перевірка");
	auto test_layout = new QVBoxLayout(test_window);

	test = new Field_test(test_window);

	auto * test_label = new QLabel("asdf");
	test_layout->addWidget(test_label);

	test->setFixedSize(670,670);
	test_layout->addWidget(test);
	test_window->setMinimumSize(test->sizeHint());

	test->change_theme(get_theme());


	expected_window = new QMainWindow(window);
	expected_window->setWindowTitle("Очікуваний результат");
	auto expected_layout = new QVBoxLayout(expected_window);

	expected = new Field_test(expected_window);

	expected_layout->addWidget(expected);
	expected_window->setMinimumSize(expected->sizeHint());

	expected->change_theme(get_theme());
	expected->setFixedSize(670, 670);

	result_window = new QMainWindow(window);
	result_window->setWindowTitle("Кінцевий результат");


	result = new Field_test(result_window);
	auto result_layout = new QVBoxLayout(result_window);
	result_layout->addWidget(result);

	result_window->setMinimumSize(result->sizeHint());

	result->change_theme(get_theme());
	result->setFixedSize(670, 670);
}

void Test_result::closeEvent(QEvent* event)
{
	hide();
	event->ignore();
}

void Test_result::show_test(QList<QList<int>> numbers, QList<QList<QList<int>>> notes, QList<QList<QList<int>>> expected_notes,
	QList<QList<int>> result_numbers, QList<QList<QList<int>>> result_notes)
{
	test->load_test(numbers, notes);
	expected->load_test(numbers, expected_notes);
	result->load_test(result_numbers, result_notes);

	test_window->show();
	expected_window->show();
	result_window->show();
}


void Test_result::show_test(QList<QList<int>> field_without_algorithm, QList<QList<QList<int>>> notes_without_algorithm,
		QList<QList<int>> field_with_algorithm, QList<QList<QList<int>>> notes_with_algorithm,
		QList<QList<int>> completed_sudoku)
{
	test->load_test(field_without_algorithm, notes_without_algorithm);
	expected->load_test(field_with_algorithm, notes_with_algorithm);
	result->load_test(completed_sudoku, {});

	test_window->show();
	expected_window->show();
	result_window->show();
}

void Test_result::show_test(QList<QList<int>> numbers, QList<QList<QList<int>>> notes)
{
	test->load_test(numbers, notes);


	test_window->show();
}


int static amount_of_numbers(QList<QList<int>> field)
{
	int amount = 0;
	for (int row = 0; row < field.size(); row++)
	{
		for (int col = 0; col < field[0].size(); col++)
			if (field[row][col] != 0)
				amount++;
	}
	return amount;
}

void static clear(QList<QList<int>> field)
{
	for (int row = 0; row < field.size(); row++)
	{
		for (int col = 0; col < field[0].size(); col++)
			field[row][col] = 0;
	}
}


bool static isIdentical(const QList<QList<int>>& first, const QList<QList<int>>& second)
{
	for (int row = 0; row < first.size(); row++)
	{
		for (int col = 0; col < first.size(); col++)
		{
			if (first[row][col] != second[row][col] && (first[row][col] != 0 && second[row][col] != 0))
				return false;
		}
	}
	return true;
}


Testing_algorithms::Testing_algorithms(QWidget* parent) : QMainWindow(parent)
{
	auto* window = new QWidget;
	auto* layout = new QVBoxLayout(window);

	setCentralWidget(window);

	setWindowTitle("Перевірка алгоритмів");

	resize(800, 600);

	auto* result_widget = new QWidget(window);
	test_results = new QVBoxLayout(result_widget);

	layout->addWidget(result_widget);

	test = new Test_result(window);

	/*test_x_wing();*/
	/*test_swordfish();*/
	/*test_jellyfish();*/
	/*check_with_random_generated_tests();*/
}

/*

	QList<QList<int>> numbers
	{
		{ 0, 0, 0,		0, 0, 0,	 0, 0, 0 },
		{ 0, 0, 0,		0, 0, 0,	 0, 0, 0 },
		{ 0, 0, 0,		0, 0, 0,	 0, 0, 0 },

		{ 0, 0, 0,		0, 0, 0,	 0, 0, 0 },
		{ 0, 0, 0,		0, 0, 0,	 0, 0, 0 },
		{ 0, 0, 0,		0, 0, 0,	 0, 0, 0 },

		{ 0, 0, 0,		0, 0, 0,	 0, 0, 0 },
		{ 0, 0, 0,		0, 0, 0,	 0, 0, 0 },
		{ 0, 0, 0,		0, 0, 0,	 0, 0, 0 }
	};


	QList<QList<QList<int>>> notes
	{
		{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
		{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
		{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },

		{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
		{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
		{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },

		{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
		{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
		{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
	};


*/

auto get_xy_wing_test_1_numbers()
{
	QList<QList<int>> numbers
	{
		{ 9, 0, 0,		2, 4, 0,	 0, 0, 0 },
		{ 0, 5, 0,		6, 9, 0,	 2, 3, 1 },
		{ 0, 2, 0,		0, 5, 0,	 0, 9, 0 },

		{ 0, 9, 0,		7, 0, 0,	 3, 2, 0 },
		{ 0, 0, 2,		9, 3, 5,	 6, 0, 7 },
		{ 0, 7, 0,		0, 0, 2,	 9, 0, 0 },

		{ 0, 6, 9,		0, 2, 0,	 0, 7, 3 },
		{ 5, 1, 0,		0, 7, 9,	 0, 6, 2 },
		{ 2, 0, 7,		0, 8, 6,	 0, 0, 9 }
	};

	return numbers;
}

auto get_xy_wing_test_1_notes()
{
  QList<QList<QList<int>>> notes
	{
		{ {   },		{ 3,8 },{ 1,3,6,8 },		{   },		{   },	{ 1,3,7,8 },			{ 5,7 },	{ 5,8 },	{ 5,6,8 }, },
		{ { 4,7,8 },	{   },	{ 4,8 },			{   },		{   },	{ 7,8 },				{   },		{   },		{   }, },
		{ { 1,3,6,7,8 },{   },	{ 1,3,6,8 },		{ 1,8 },	{   },	{ 1,3,7,8 },			{ 4,7 },	{   },		{ 4,6,8 }, },

		{ { 1,4,6,8 },	{   },	{ 1,4,5,6,8 },		{   },		{ 1,6 },{ 4,8 },				{   },		{   },		{ 4,5,8 }, },
		{ { 1,4,8 },	{ 4,8 },{   },				{   },		{   },	{   },					{   },		{ 1,4,8 },	{   }, },
		{ { 1,3,4,6,8 },{   },	{ 1,3,4,5,6,8 },	{ 4,8 },	{ 1,6 },{   },					{   },		{ 1,4,5,8 },{ 4,5,8 }, },

		{ { 4,8 },		{   },	{   },				{ 1,4,5 },	{   },	{ 1,4 },				{ 1,4,5,8 },{   },		{   }, },
		{ {   },		{   },	{ 3,4,8 },			{ 4,3 },	{   },	{   },					{ 4,8 },	{   },		{   }, },
		{ {   },		{ 4,3 },{   },				{ 1,3,4,5 },{   },	{   },					{ 1,4,5 },	{ 4,5 },	{   }, },
	};
	
	return notes;
}

void Testing_algorithms::test_xy_wing()
{
	Solver_test solver;

	QList<QList<int>> numbers;
	QList<QList<QList<int>>> notes;


	{
		numbers = get_xy_wing_test_1_numbers();
		notes = get_xy_wing_test_1_notes();

		solver.load_test(notes);
		while (solver.xy_wing())
		{
		}

		QList<QList<QList<int>>> expected_to_remove{ 9, QList<QList<int>>(9, QList<int>(9, 0)) };
		expected_to_remove[7][2] = { 4 };
		auto result_test = solver.get_notes();

		add_test(numbers, notes, expected_to_remove, numbers, result_test, ";3");
	}
}

auto get_xyz_wing_test_1_numbers()
{
	QList<QList<int>> numbers
	{
		{ 0, 9, 2,		0, 0, 1,	 7, 5, 0 },
		{ 5, 0, 0,		2, 0, 0,	 0, 0, 8 },
		{ 0, 0, 0,		0, 3, 0,	 2, 0, 0 },

		{ 0, 7, 5,		0, 0, 4,	 9, 6, 0 },
		{ 2, 0, 0,		0, 6, 0,	 0, 7, 5 },
		{ 0, 6, 9,		7, 0, 0,	 0, 3, 0 },

		{ 0, 0, 8,		0, 9, 0,	 0, 2, 0 },
		{ 7, 0, 0,		0, 0, 3,	 0, 8, 9 },
		{ 9, 0, 3,		8, 0, 0,	 0, 4, 0 }
	};

	return numbers;
}

auto get_xyz_wing_test_1_notes()
{
	QList<QList<QList<int>>> notes
	{
		{ { 3,8 },		{   },		{   },				{ 4,6 },	{ 4,8 },	{   },				{   },		{   },	{ 3,4,6 }, },
		{ {   },		{ 1,3,4 },	{ 1,4,6,7 },		{   },		{ 4,7 },	{ 6,7,9 },			{ 3,4,6 },	{ 1,9 },{   }, },
		{ { 1,4,6 },	{ 1,4,8 },	{ 1,4,6,7 },		{ 4,5,6,9 },{   },		{ 5,6,7,8,9 },		{   },		{ 1,9 },{ 4,6 }, },

		{ { 3,8 },		{   },		{   },				{ 1,3 },	{ 1,2,8 },	{   },				{   },		{   },	{ 1,2 }, },
		{ {   },		{ 3,8 },	{ 1,4 },			{ 1,3,9 },	{   },		{ 8,9 },			{ 1,4,8 },	{   },	{   }, },
		{ { 1,4 },		{   },		{   },				{   },		{ 1,2,5 },	{ 2,5,8 },			{ 1,4,8 },	{   },	{ 1,2,4 }, },

		{ { 1,4,6 },	{ 1,4,5 },	{   },				{ 1,4,5,6 },{   },		{ 5,6,7 },			{ 1,3,5,6 },{   },	{ 1,3,6,7 }, },
		{ {   },		{ 1,2,4,5 },{ 1,4,6 },			{ 1,4,5,6 },{ 1,2,4,5 },{   },				{ 1,5,6 },	{   },	{   }, },
		{ {   },		{ 1,2,5 },	{   },				{   },		{ 1,2,5,7 },{ 2,5,6,7 },		{ 1,5,6 },	{   },	{ 1,6,7 }, },
	};

	return notes;
}

void Testing_algorithms::test_xyz_wing()
{
	Solver_test solver;

	QList<QList<int>> numbers;
	QList<QList<QList<int>>> notes;


	{
		numbers = get_xyz_wing_test_1_numbers();
		notes = get_xyz_wing_test_1_notes();

		solver.load_test(notes);
		while (solver.xyz_wing())
		{
		}

		QList<QList<QList<int>>> expected_to_remove{ 9, QList<QList<int>>(9, QList<int>(9, 0)) };
		expected_to_remove[5][6] = { 1 };
		auto result_test = solver.get_notes();

		add_test(numbers, notes, expected_to_remove, numbers, result_test, ";3");
	}
}

auto get_wxyz_wing_test_1_numbers()
{
	QList<QList<int>> numbers
	{
		{ 0, 0, 0,		0, 2, 4,	 7, 3, 0 },
		{ 5, 4, 0,		3, 7, 0,	 2, 6, 0 },
		{ 2, 3, 7,		0, 0, 0,	 0, 0, 4 },

		{ 7, 0, 0,		0, 3, 0,	 8, 4, 0 },
		{ 0, 0, 3,		4, 8, 1,	 0, 0, 0 },
		{ 0, 8, 4,		0, 6, 0,	 0, 0, 3 },

		{ 3, 0, 0,		0, 0, 0,	 0, 5, 9 },
		{ 0, 7, 0,		0, 9, 3,	 0, 0, 2 },
		{ 0, 0, 6,		2, 0, 0,	 3, 0, 0 }
	};

	return numbers;
}

auto get_wxyz_wing_test_1_notes()
{
	QList<QList<QList<int>>> notes
	{
		{ { 1,6,8,9 },	{ 1,6,9 },		{ 1,8,9 },			{ 1,5,8,9 },	{   },		{   },				{   },		{   },		{ 1,5,8 }, },
		{ {   },		{   },			{ 1,8,9 },			{   },			{   },		{ 8,9 },			{   },		{   },		{ 1,8 }, },
		{ {   },		{   },			{   },				{ 1,5,6,8 },	{ 1,5 },	{ 5,6,8 },			{ 1,5,9 },	{ 1,8,9 },	{   }, },

		{ {   },		{ 1,2,5,6,9 },	{ 1,2,5,9 },		{ 5,9 },		{   },		{ 2,5,9 },			{   },		{   },		{ 1,5,6 }, },
		{ { 6,9 },		{ 2,5,6,9 },	{   },				{   },			{   },		{   },				{ 5,9 },	{ 2,7,9 },	{ 5,6,7 }, },
		{ { 1,9 },		{   },			{   },				{ 5,7,9 },		{   },		{ 2,5,7,9 },		{ 1,5,9 },	{ 1,2 },	{   }, },

		{ {   },		{ 1,2 },		{ 1,2,8 },			{ 1,6,7,8 },	{ 1,4 },	{ 6,7,8 },			{ 4,6 },	{   },		{   }, },
		{ { 1,4,8 },	{   },			{ 1,5,8 },			{ 5,6,8 },		{   },		{   },				{ 4,6 },	{ 1,8 },	{   }, },
		{ { 1,4,8,9 },	{ 1,5,9 },		{   },				{   },			{ 1,4,5 },	{ 5,8 },			{   },		{ 1,7,8 },	{ 1,7,8 }, },
	};

	return notes;
}

void Testing_algorithms::test_wxyz_wing()
{
	Solver_test solver;

	QList<QList<int>> numbers;
	QList<QList<QList<int>>> notes;


	{
		numbers = get_wxyz_wing_test_1_numbers();
		notes = get_wxyz_wing_test_1_notes();

		solver.load_test(notes);
		//while (solver.wxyz_wing())
		{
		}

		QList<QList<QList<int>>> expected_to_remove{ 9, QList<QList<int>>(9, QList<int>(9, 0)) };
		expected_to_remove[3][1] = { 9 };
		auto result_test = solver.get_notes();

		add_test(numbers, notes, expected_to_remove, numbers, result_test, ";3");
	}
}

void Testing_algorithms::test_naked_candidates()
{

}
auto get_hidden_candidates_test_1_numbers()
{
	QList<QList<int>> numbers
	{
		{ 4, 0, 0,		0, 0, 0,	 9, 3, 8 },
		{ 0, 3, 2,		0, 9, 4,	 1, 0, 0 },
		{ 0, 9, 5,		3, 0, 0,	 2, 4, 0 },

		{ 3, 7, 0,		6, 0, 9,	 0, 0, 4 },
		{ 5, 2, 9,		0, 0, 1,	 6, 7, 3 },
		{ 6, 0, 4,		7, 0, 3,	 0, 9, 0 },

		{ 9, 5, 7,		0, 0, 8,	 3, 0, 0 },
		{ 0, 0, 3,		9, 0, 0,	 4, 0, 0 },
		{ 2, 4, 0,		0, 3, 0,	 7, 0, 9 }
	};

	return numbers;
}

auto get_hidden_candidates_test_1_notes()
{
	QList<QList<QList<int>>> notes
	{
		{ {   }, { 1,6 }, { 1,6 },		 { 1,2,5 }, { 1,2,5,6,7 }, { 2,5,6,7 },		 {   }, {   }, {   }, },
		{ { 7,8 }, {   }, {   },		 { 5,8 }, {   }, {   },		 {   }, { 5,6 }, { 5,7,6 }, },
		{ { 1,7,8 }, {   }, {   },		 {   }, { 1,6,7,8 }, { 7,6 },		 {   }, {   }, {  7,6 }, },

		{ {   }, {   }, { 1,8 },		 {   }, { 2,5,8 }, {   },		 { 5,8 }, { 1,2,5,8 }, {   }, },
		{ {   }, {   }, {   },		 { 4,8 }, { 4,8 }, {   },		 {   }, {   }, {   }, },
		{ {   }, { 1,8 }, {   },		 {   }, { 2,5,8 }, {   },		 { 5,8 }, {   }, { 1,2,5 }, },

		{ {   }, {   }, {   },		 { 1,2,4 }, { 1,2,4,6 }, {   },		 {   }, { 1,2,6 }, { 1,2,6 }, },
		{ { 1,8 }, { 1,6,8 }, {   },		 {   }, { 1,2,5,6,7 }, { 2,5,6,7 },		 {   }, { 1,2,5,6,8 }, { 1,2,5,6 }, },
		{ {   }, {   }, { 1,6,8 },		 { 1,5 }, {   }, { 5,6 },		 {   }, { 1,5,6,8 }, {   }, },
	};

	return notes;
}

void Testing_algorithms::test_hidden_candidates()
{
	Solver_test solver;

	QList<QList<int>> numbers;
	QList<QList<QList<int>>> notes;


	{
		numbers = get_hidden_candidates_test_1_numbers();
		notes = get_hidden_candidates_test_1_notes();

		solver.load_test(notes);

		while (solver.hidden_numbers())
		{
		}

		QList<QList<QList<int>>> expected_to_remove{ 9, QList<QList<int>>(9, QList<int>(9, 0)) };
		expected_to_remove[0][3] = { 1 };
		expected_to_remove[0][4] = { 1, 6 };
		expected_to_remove[0][5] = { 6 };
		expected_to_remove[2][0] = { 1,7 };
		expected_to_remove[2][4] = { 6,7 };
		auto result_test = solver.get_notes();

		add_test(numbers, notes, expected_to_remove, numbers, result_test, ";3");
	}
}

void Testing_algorithms::test_x_wing()
{
	// перевірка по стовпцях
	{
		QList<QList<int>> test
		{
			{ 0, 0, 0,		0, 0, 0,	 0, 9, 4 },
			{ 7, 6, 0,		9, 1, 0,	 0, 5, 0 },
			{ 0, 9, 0,		0, 0, 2,	 0, 8, 1 },

			{ 0, 7, 0,		0, 5, 0,	 0, 1, 0 },
			{ 0, 0, 0,		7, 0, 9,	 0, 0, 0 },
			{ 0, 8, 0,		0, 3, 1,	 0, 6, 7 },

			{ 2, 4, 0,		1, 0, 0,	 0, 7, 0 },
			{ 0, 1, 0,		0, 9, 0,	 0, 4, 5 },
			{ 9, 0, 0,		0, 0, 0,	 1, 0, 0 }
		};


		QList<QList<QList<int>>> notes
		{
			{	{  }, {  }, {  },		 {  }, {  }, {  },			{  }, {  }, {  },	 },
			{	{  }, {  }, {  },		 {  }, {  }, {  },			{  }, {  }, { 2 },	 },
			{	{  }, {  }, {  },		 {  }, {  }, {  },			{  }, {  }, {  },	 },

			{	{  }, {  }, {  },		 {  }, {  }, {  },			{  }, {  }, { 2 },	 },
			{	{  }, { 2 }, { 2 },		 {  }, { 2 }, {  },	{ 2 }, { 2 }, { 2 },	 },
			{	{  }, {  }, {  },		 {  }, {  }, {  },			{  }, {  }, {  },	 },

			{	{  }, {  }, {  },		 {  }, {  }, {  },			{  }, {  }, {  },	 },
			{	{  }, {  }, {  },		 {  }, {  }, {  },			{ 2 }, {  }, {  },	 },
			{	{  }, {  }, {  },		 { 2 }, { 2 }, {  },			{  }, { 2 }, { 2 },	 },
		};

		QList<QList<QList<int>>> expected_to_remove_notes
		{
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },

			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, { 2 }, { 2 },		 {  }, {  }, {  },		 { 2 }, {  }, { 2 }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },

			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 { 2 }, {  }, {  },		 {  }, {  }, { 2 }, },
		};



		Solver_test solver;

		solver.load_test(test, notes);

		solver.x_wings();

		auto result_numbers = solver.get_sudoku();
		auto result_notes = solver.get_notes();


		add_test(test, notes, expected_to_remove_notes, result_numbers, result_notes, "По стовпцях");

	}


	// перевірка по рядках
	{
		QList<QList<QList<int>>> notes
		{
			{ { 2 }, {  }, { 2 },		 { 2 }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },			 { 2 }, {  }, {  },		 {  }, {  }, {  }, },
			{ { 2 }, {  }, { 2 },		 {  }, {  }, {  },		 {  }, {  }, {  }, },

			{ { 2 }, {  }, { 2 },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },

			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
		};

		QList<QList<QList<int>>> notes_to_remove
		{
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },

			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },

			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
		};

		QList<QList<int>> test(9, QList<int>(9, 0));

		Solver_test solver;

		solver.load_test(test, notes);

		solver.x_wings();

		auto result_numbers = solver.get_sudoku();
		auto result_notes = solver.get_notes();


		add_test(test, notes, notes_to_remove, result_numbers, result_notes, "По рядках");
	}


}

void Testing_algorithms::test_swordfish()
{
	// Риба-меч по стовпцях
	{
		QList<QList<QList<int>>> notes
		{
			{ {  }, { 2 }, { 2 },		 { 2 }, {  }, { 2 },	 {  }, { 2 }, {  }, },
			{ {  }, {  },  {  },		 {  },  {  }, {  },		 {  }, {  }, {  }, },
			{ { 2 },{ 2 }, { 2 },		 { 2 }, {  }, { 2 },	 {  }, {  }, {  }, },

			{ {  }, {  },  {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  },  {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, { 2 }, {  },		 { 2 }, { 2 }, { 2 },	 { 2 }, {  }, {  }, },

			{ { 2 }, {  }, { 2 },		 {  }, { 2 }, {  },		 {  }, { 2 }, {  }, },
			{ {  },  {  }, { 2 },		 {  }, { 2 }, {  },		 {  }, { 2 }, {  }, },
			{ { 2 }, {  }, {  },		 {  }, {  }, {  },		 { 2 }, {  }, {  }, },
		};

		QList<QList<QList<int>>> notes_to_remove
		{
			{ {  }, {  }, { 2 },		 {  }, {  }, {  },		 {  }, { 2 }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ { 2 }, {  }, { 2 },		 {  }, {  }, {  },		 {  }, {  }, {  }, },

			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, { 2 }, {  },		 {  }, {  }, {  }, },

			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
		};

		QList<QList<int>> test(9, QList<int>(9, 0));

		Solver_test solver;

		solver.load_test(test, notes);

		solver.swordfish();

		auto result_numbers = solver.get_sudoku();
		auto result_notes = solver.get_notes();

		add_test(test, notes, notes_to_remove, result_numbers, result_notes, "По стовпцях");
	}

	// Риба-меч по рядках
	{
		QList<QList<QList<int>>> notes
		{
			{ {   }, {  }, {  },	 {  }, {  }, {   },		 {   }, {   }, {   }, },
			{ { 2 }, {  }, {  },	 {  }, {  }, {   },		 { 2 }, { 2 }, {   }, },
			{ {   }, {  }, {  },	 {  }, {  }, { 2 },		 {   }, {   }, { 2 }, },

			{ { 2 }, {  }, { 2 },	 {  }, {  }, { 2 },		 {   }, {   }, {   }, },
			{ {   }, {  }, {  },	 {  }, {  }, { 2 },		 { 2 }, { 2 }, {   }, },
			{ { 2 }, {  }, { 2 },	 {  }, {  }, { 2 },		 {   }, {   }, {   }, },

			{ { 2 }, {  }, { 2 },	 {  }, {  }, {   },		 { 2 }, { 2 }, {   }, },
			{ { 2 }, {  }, { 2 },	 {  }, {  }, { 2 },		 {   }, {   }, {   }, },
			{ {   }, {  },  { 2 },	 {  }, {  }, {   },		 { 2 }, {   }, { 2 }, },
		};

		QList<QList<QList<int>>> notes_to_remove
		{
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ { 2 }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, { 2 },		 {  }, {  }, {  }, },

			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, { 2 },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },

			{ { 2 }, {  }, { 2 },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, {  },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
			{ {  }, {  }, { 2 },		 {  }, {  }, {  },		 {  }, {  }, {  }, },
		};

		QList<QList<int>> test(9, QList<int>(9, 0));

		Solver_test solver;

		solver.load_test(test, notes);

		solver.swordfish();

		auto result_numbers = solver.get_sudoku();
		auto result_notes = solver.get_notes();

		add_test(test, notes, notes_to_remove, result_numbers, result_notes, "По рядках");
	}
}

void Testing_algorithms::test_jellyfish()
{
	{
		QList<QList<QList<int>>> notes
		{
			{ {   }, { 7 }, {   },		 { 7 }, {   }, { 7 },		 {   }, {   }, {   }, },
			{ {   }, {   }, {   },		 {   }, { 7 }, {   },		 { 7 }, {   }, { 7 }, },
			{ {   }, { 7 }, {   },		 { 7 }, { 7 }, {   },		 { 7 }, { 7 }, { 7 }, },
				 		 		    	   			 		    		 	  		  
			{ { 7 }, {   }, {   },		 {   }, { 7 }, {   },		 {   }, {   }, { 7 }, },
			{ { 7 }, {   }, {   },		 {   }, {   }, {   },		 { 7 }, {   }, { 7 }, },
			{ { 7 }, {   }, {   },		 { 7 }, {   }, {   },		 {   }, { 7 }, {   }, },
				 		 		    	   			 		    		 	  		  
			{ {   }, {   }, {   },		 {   }, { 7 }, { 7 },		 {   }, { 7 }, { 7 }, },
			{ {   }, {   }, {   },		 {   }, { 7 }, {   },		 { 7 }, {   }, { 7 }, },
			{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
		};

		QList<QList<QList<int>>> notes_to_remove
		{
			{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
			{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
			{ {   }, {   }, {   },		 {   }, { 7 }, {   },		 { 7 }, {   }, { 7 }, },
																	    
			{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
			{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
			{ { 7 }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
																	    
			{ {   }, {   }, {   },		 {   }, { 7 }, {   },		 {   }, {   }, { 7 }, },
			{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
			{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
		};

		QList<QList<int>> test(9, QList<int>(9, 0));

		Solver_test solver;

		solver.load_test(test, notes);

		solver.jellyfish();

		auto result_numbers = solver.get_sudoku();
		auto result_notes = solver.get_notes();

		add_test(test, notes, notes_to_remove, result_numbers, result_notes, "По рядках");
	}

	{
		QList<QList<QList<int>>> notes
		{
			{ {   }, { 7 }, { 7 },		 { 7 }, { 7 }, {   },		 { 7 }, { 7 }, {   }, },
			{ {   }, {   }, { 7 },		 {   }, {   }, { 7 },		 { 7 }, {   }, {   }, },
			{ {   }, { 7 }, { 7 },		 {   }, { 7 }, {   },		 {   }, { 7 }, {   }, },
				 		 		    	    		  	      		 	   		   	  
			{ { 7 }, {   }, {   },		 {   }, {   }, {   },		 { 7 }, {   }, {   }, },
			{ {   }, { 7 }, { 7 },		 { 7 }, {   }, {   },		 { 7 }, { 7 }, {   }, },
			{ { 7 }, {   }, { 7 },		 {   }, {   }, { 7 },		 {   }, {   }, {   }, },
				 		 		    	    		  	      		 	   		   	  
			{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
			{ { 7 }, {   }, { 7 },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
			{ {   }, {   }, {   },		 { 7 }, { 7 }, { 7 },		 {   }, {   }, {   }, },
		};

		QList<QList<QList<int>>> notes_to_remove
		{
			{ {   }, {   }, { 7 },		 {   }, {   }, {   },		 { 7 }, {   }, {   }, },
			{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
			{ {   }, {   }, { 7 },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
																	    
			{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
			{ {   }, {   }, { 7 },		 {   }, {   }, {   },		 { 7 }, {   }, {   }, },
			{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
																	    
			{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
			{ {   }, {   }, {   },		 {   }, {   }, {   },		 {   }, {   }, {   }, },
			{ {   }, {   }, {   },		 {   }, {   }, { 7 },		 {   }, {   }, {   }, },
		};


		QList<QList<int>> test(9, QList<int>(9, 0));

		Solver_test solver;

		solver.load_test(test, notes);

		solver.jellyfish();

		auto result_numbers = solver.get_sudoku();
		auto result_notes = solver.get_notes();

		add_test(test, notes, notes_to_remove, result_numbers, result_notes, "По стовпцях");
	}
}


/*
* Перший тест є одним ланцюгом
* Другий тест є кількома ланцюгами
* Третій тест є одним ланцюгом з явно не правильним рішенням
*/


QList<QList<QList<int>>> get_bi_location_test1()
{
	QList<QList<QList<int>>> test
	{	 /*  0     1     2            3     4     5           6     7     8	*/
	/*0*/{ { 1 },{   },{   },		{ 1 },{   },{   },		{   },{   },{   },		},
	/*1*/{ { 1 },{   },{   },		{   },{ 1 },{   },		{   },{   },{   },		},
	/*2*/{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   },		},
		      	    
	/*3*/{ {   },{   },{ 1 },		{   },{   },{   },		{   },{   },{ 1 },		},
	/*4*/{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   },		},
	/*5*/{ {   },{ 1 },{   },		{   },{   },{   },		{ 1 },{   },{   },		},
		      	    
	/*6*/{ {   },{   },{ 1 },		{   },{ 1 },{   },		{   },{   },{   },		},
	/*7*/{ {   },{ 1 },{   },		{   },{   },{   },		{ 1 },{   },{   },		},
	/*8*/{ {   },{   },{   },		{ 1 },{   },{   },		{   },{   },{ 1 },		}
	};

	return test;
}

QList<QList<QList<int>>> get_bi_location_test2()
{
	QList<QList<QList<int>>> test
	{	 /*  0     1     2            3     4     5           6     7     8	*/
	/*0*/{ { 1 },{   },{   },		{   },{   },{   },		{   },{   },{   },	},
	/*1*/{ {   },{ 1 },{   },		{   },{   },{   },		{   },{   },{   },	},
	/*2*/{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   },	},
		 
	/*3*/{ { 1 },{   },{   },		{   },{   },{   },		{   },{   },{   },	},
	/*4*/{ {   },{ 1 },{   },		{   },{   },{   },		{   },{   },{   },	},
	/*5*/{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   },	},
		 
	/*6*/{ {   },{   },{   },		{   },{ 1 },{   },		{   },{ 1 },{   },	},
	/*7*/{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   },	},
	/*8*/{ {   },{   },{   },		{   },{   },{ 1 },		{ 1 },{   },{   },	}
	};

	return test;
}


QList<QList<QList<int>>> get_bi_location_test3()
{
	QList<QList<QList<int>>> test
	{	 /*  0     1     2            3     4     5           6     7     8	*/
	/*0*/{ {   },{ 1 },{   },		{   },{   },{   },		{ 1 },{   },{   }, },
	/*1*/{ {   },{ 1 },{   },		{   },{ 1 },{   },		{ 1 },{   },{ 1 }, },
	/*2*/{ { 1 },{   },{   },		{ 1 },{   },{   },		{ 1 },{   },{   }, },
		 
	/*3*/{ {   },{   },{ 1 },		{   },{   },{   },		{   },{   },{ 1 }, },
	/*4*/{ {   },{   },{   },		{ 1 },{   },{ 1 },		{   },{   },{   }, },
	/*5*/{ { 1 },{   },{   },		{   },{   },{   },		{ 1 },{   },{   }, },
		 
	/*6*/{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   }, },
	/*7*/{ {   },{   },{ 1 },		{   },{ 1 },{   },		{   },{   },{   }, },
	/*8*/{ {   },{ 1 },{   },		{   },{   },{ 1 },		{   },{   },{   }, }
	};

	return test;
}

QPair<QString, QString> get_chain_string(QPair<QList<coords>, QList<coords>>& chain)
{
	QString first_possible_solution;
	for (auto& coord : chain.first)
		first_possible_solution.push_back(QString::number(coord.row) + QString::number(coord.col) + " ");
	
	QString second_possible_solution;

	for (auto& coord : chain.second)
		second_possible_solution.push_back(QString::number(coord.row) + QString::number(coord.col) + " ");

	return { first_possible_solution, second_possible_solution };
}

void add_bi_location_test_result(QPair<QList<coords>, QList<coords>>& chain, QHBoxLayout * layout)
{
	auto possible_solution_strings = get_chain_string(chain);

	auto * chain_result_widget = new QWidget;
	chain_result_widget->setStyleSheet("background-color: red");

	layout->addWidget(chain_result_widget);

	auto * chain_result_layout = new QVBoxLayout(chain_result_widget);

	auto * first_possible_solution = new QLabel(possible_solution_strings.first);
	auto * second_possible_solution = new QLabel(possible_solution_strings.second);

	chain_result_layout->addWidget(first_possible_solution);
	chain_result_layout->addWidget(second_possible_solution);
}

void show_chains_result(QVBoxLayout * test_results, QList<QPair<QList<coords>, QList<coords>>>& chains, QString test_name)
{
	auto * result = new QWidget;
	result->setStyleSheet("background-color: gray");

	test_results->addWidget(result);

	auto * layout = new QHBoxLayout(result);

	auto * test_name_label = new QLabel(test_name);

	layout->addWidget(test_name_label);

	for (auto& chain : chains)
	{
		add_bi_location_test_result(chain, layout);
	}
}


void Testing_algorithms::test_bi_location_chain()
{
	Solver_test solver;

	auto test = get_bi_location_test1();
	solver.load_test(test);
	/* auto chain = solver.find_bi_location_chains(1);
	show_chains_result(test_results, chain, "Один великий ланцюг");

	test = get_bi_location_test2();
	solver.load_test(test);
	// chain = solver.find_bi_location_chains(1);
	show_chains_result(test_results, chain, "Кілька ланцюгів");

	test = get_bi_location_test3();
	solver.load_test(test);
	// chain = solver.find_bi_location_chains(1);
	show_chains_result(test_results, chain, "Один з ланцюгів має очевидно не правильним рішенням");
	*/
}


/*
QList<QList<QList<int>>> test
	{	   /*  0     1     2          3     4     5           6     7     8	*/
	/*0*//*{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   }, },
	/*1*//*{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   }, },
	/*2*//*{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   }, },
		 	   	     	   			          	    		    		    
	/*3*//*{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   }, },
	/*4*//*{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   }, },
	/*5*//*{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   }, },
		 	   	     	   			          	    		    		    
	/*6*//*{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   }, },
	/*7*//*{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   }, },
	/*8*//*{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   }, }
	}
*/

QList<QList<QList<int>>> get_simple_chain_test_1()
{
	QList<QList<QList<int>>> test
	{	 /*  0     1     2            3     4     5           6     7     8	*/
	/*0*/{ {   },{ 1 },{   },		{   },{   },{   },		{ 1 },{   },{   }, },
	/*1*/{ {   },{ 1 },{   },		{   },{ 1 },{   },		{ 1 },{   },{ 1 }, },
	/*2*/{ { 1 },{   },{   },		{ 1 },{   },{   },		{ 1 },{   },{   }, },
		 
	/*3*/{ {   },{   },{ 1 },		{   },{   },{   },		{   },{   },{ 1 }, },
	/*4*/{ {   },{   },{   },		{ 1 },{   },{ 1 },		{   },{   },{   }, },
	/*5*/{ { 1 },{   },{   },		{   },{   },{   },		{ 1 },{   },{   }, },
		 
	/*6*/{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   }, },
	/*7*/{ {   },{   },{ 1 },		{   },{ 1 },{   },		{   },{   },{   }, },
	/*8*/{ {   },{ 1 },{   },		{   },{   },{ 1 },		{   },{   },{   }, }
	};


	return test;
}

QList<QList<QList<int>>> get_simple_chain_test_2()
{
	QList<QList<QList<int>>> test
	{	 /*  0     1     2            3     4     5           6     7     8	*/
	/*0*/{ {   },{ 3 },{ 3 },		{ 3 },{   },{   },		{   },{ 3 },{   }, },
	/*1*/{ {   },{ 3 },{ 3 },		{   },{ 3 },{   },		{   },{   },{ 3 }, },
	/*2*/{ {   },{   },{   },		{ 3 },{   },{ 3 },		{   },{ 3 },{   }, },
		 	   	     	   			          	    		    		    
	/*3*/{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   }, },
	/*4*/{ {   },{   },{   },		{ 3 },{   },{ 3 },		{   },{   },{ 3 }, },
	/*5*/{ {   },{   },{   },		{   },{ 3 },{   },		{   },{ 3 },{   }, },
		 	   	     	   			          	    		    		    
	/*6*/{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   }, },
	/*7*/{ {   },{   },{ 3 },		{   },{ 3 },{   },		{   },{   },{   }, },
	/*8*/{ {   },{ 3 },{   },		{   },{ 3 },{ 3 },		{   },{   },{   }, }
	};


	return test;
}

QList<QList<QList<int>>> get_simple_chain_test_3()
{
	QList<QList<QList<int>>> test
	{	 /*  0     1     2            3     4     5           6     7     8	*/
	/*0*/{ {   },{   },{ 8 },		{ 8 },{   },{   },		{ 8 },{   },{   }, },
	/*1*/{ {   },{   },{ 8 },		{   },{ 8 },{   },		{ 8 },{   },{   }, },
	/*2*/{ { 8 },{   },{   },		{ 8 },{   },{   },		{ 8 },{   },{   }, },
																      	    
	/*3*/{ {   },{   },{ 8 },		{   },{   },{   },		{   },{   },{ 8 }, },
	/*4*/{ {   },{   },{   },		{ 8 },{   },{   },		{   },{   },{ 8 }, },
	/*5*/{ { 8 },{   },{   },		{   },{ 8 },{   },		{   },{   },{   }, },
																      	    
	/*6*/{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   }, },
	/*7*/{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   }, },
	/*8*/{ {   },{   },{   },		{   },{   },{   },		{   },{   },{   }, }
	};


	return test;
}

QList<QList<int>> get_simple_chain_test_4_numbers()
{
	QList<QList<int>> test 
	{
		{ 0,8,1,6,0,5,0,9,0 },
		{ 0,0,0,0,0,0,0,0,0 },
		{ 0,0,4,0,3,7,6,0,0 },
		{ 6,0,0,4,7,0,5,0,0 },
		{ 4,3,0,0,0,0,0,7,0 },
		{ 0,0,7,0,0,2,0,0,4 },
		{ 0,0,5,2,1,0,3,0,7 },
		{ 0,0,0,7,0,0,0,0,0 },
		{ 0,7,0,0,0,4,8,1,0 }
	};


	return test;
}

QList<QList<QList<int>>> get_simple_chain_test_4()
{
	QList<QList<QList<int>>> test
	{	 /*  0				  1			  2						  3					4		  5						6		  7					8	*/
	/*0*/{ { 2,3,7 },		{   },		{   },					{   },			{ 2,4 },	{   },				{ 2,4,7  },	{   },			{ 2,3  }, },
	/*1*/{ { 2,3,5,7,9  },	{ 2,5,6,9 },{ 2,3,6,9 },			{ 1,8,9 },		{ 2,4 },	{ 1,8,9 },			{ 1,4,7 },	{ 2,3,4,5,8 },	{ 1,2,3,5,8 }, },
	/*2*/{ { 2,5,9 },		{ 2,5,9 },	{   },					{ 1,8,9  },		{   },		{   },				{   },		{ 2,5,8 },		{ 1,2,5,8 }, },
			   					   								        					      				
	/*3*/{ {   },			{ 1,2,9 },	{ 2,8,9 },				{   },			{   },		{ 1,3,8,9 },		{   },		{ 2,3,8 },		{ 1,2,3,8,9 }, },
	/*4*/{ {   },			{   },		{ 2,8,9 },				{ 1,5,8,9 },	{ 5,8,9 },	{ 1,6,8,9 },		{ 1,2,9 },	{   },			{ 2,6,9 }, },
	/*5*/{ { 1,5,8,9 },		{ 1,5,9 },	{   },					{ 1,3,8,9 },	{ 6,9 },	{   },				{ 1,9 },	{ 3,6,8 },		{   }, },
			   								   										 			   	      											    
	/*6*/{ { 8,9 },			{ 4,6,9 },	{   },					{   },			{   },		{ 6,8,9 },			{   },		{ 4,6 },		{   }, },
	/*7*/{ { 1,2,3,8,9 },	{ 1,2,4,9 },{ 2,3,6,8,9 },			{   },			{ 5,8,9 },{ 3,6,9 },			{ 2,4,9 },	{ 2,4,5,6 },	{ 2,5,6,9 }, },
	/*8*/{ { 2,3,9 },		{   },		{ 2,3,6,9 },			{ 3,5,9 },		{ 5,6,9 },	{   },				{   },		{   },			{ 2,5,6,9 }, }
	};


	return test;
}

void Testing_algorithms::test_simple_chains()
{
	Solver_test solver;

	QList<QList<QList<int>>> test;
	QList<QList<int>> numbers{ 9, QList<int>(9, 0) };


	{
		test = get_simple_chain_test_2();

		solver.load_test(test);
		solver.simple_chains();

		QList<QList<QList<int>>> expected_to_remove{ 9, QList<QList<int>>(9, QList<int>(9, 0)) };
		expected_to_remove[1][4] = { 3 };
		auto result_test = solver.get_notes();

		add_test(numbers, test, expected_to_remove, numbers, result_test, "Одне число бачить два рішення");
	}


	{
		test = get_simple_chain_test_3();

		solver.load_test(test);
		solver.simple_chains();
		QList<QList<QList<int>>> expected_to_remove{ 9, QList<QList<int>>(9, QList<int>(9, 0)) };
		expected_to_remove[1][2] = { 8 };
		expected_to_remove[2][3] = { 8 };

		auto result_test = solver.get_notes();
		add_test(numbers, test, expected_to_remove, numbers, result_test, "Два числа бачить два рішення");
	
	}

	{
		auto numbers = get_simple_chain_test_4_numbers();
		test = get_simple_chain_test_4();
		solver.load_test(numbers, test);

		solver.simple_chains();
		solver.simple_chains();

		QList<QList<QList<int>>> expected_to_remove{ 9, QList<QList<int>>(9, QList<int>(9, 0)) };
		expected_to_remove[8][8] = { 6 };
		expected_to_remove[7][2] = { 6 };

		auto result_test = solver.get_notes();
		add_test(numbers, test, expected_to_remove, numbers, result_test, "Дві шістки");

	}
}

void Testing_algorithms::add_test(QList<QList<int>>& numbers, QList<QList<QList<int>>>& notes, QList<QList<QList<int>>>& expected_to_remove_notes,
	QList<QList<int>>& result_numbers, QList<QList<QList<int>>>& result_notes, QString test_name)
{
	bool is_working = is_removed(result_notes, expected_to_remove_notes);

	auto* result_widget = new QWidget;

	test_results->addWidget(result_widget);

	auto* result_layout = new QHBoxLayout(result_widget);

	auto* test_name_label = new QLabel(test_name);

	auto* test_result = new QLabel;

	if (is_working)
		test_result->setText("Працює");
	else
		test_result->setText("Не працює");

	auto* show_test = new QPushButton("Переглянути перевірку");

	connect(show_test, &QPushButton::pressed, [=]()
		{
			test->show_test(numbers, notes, expected_to_remove_notes, result_numbers, result_notes);
		});


	result_layout->addWidget(test_name_label);
	result_layout->addWidget(test_result);
	result_layout->addWidget(show_test);
}


bool Testing_algorithms::is_removed(QList<QList<QList<int>>>& result_notes, QList<QList<QList<int>>>& expected_to_remove_notes)
{
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			for (auto expected_to_remove : expected_to_remove_notes[row][col])
			{
				if (result_notes[row][col].contains(expected_to_remove))
				{
					return false;
				}
			}
		}
	}
	return true;
}


void Testing_algorithms::clear()
{
	for (int i = test_results->count(); i > 0; i--)
	{
		QLayoutItem* item = test_results->itemAt(i);

		if (item->widget())
			item->widget()->deleteLater();

		delete item;
	}
}


void Testing_algorithms::save_test()
{
	QString test_string;

	auto& test = tests[0];

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
			test_string += QString::number(test[row][col]);
	}

	QFile file("test.txt");
	file.open(QIODevice::WriteOnly | QIODevice::Text);

	QTextStream out(&file);
	out << test_string;
	file.close();

}

void Testing_algorithms::load_test()
{
	QFile file("test.txt");
	file.open(QIODevice::ReadOnly | QIODevice::Text);

	QTextStream in(&file);
	QString test_string;
	in >> test_string;
	file.close();

	QList<QList<int>> test(9, QList<int>(9, 0));

	for (int row = 0; row < 9; row++)
		for (int col = 0; col < 9; col++)
			test[row][col] = test_string[row * 9 + col].digitValue();

	tests.push_back(test);
}

void Testing_algorithms::show_test(const QList<QList<int>>& test, const QList<QList<QList<int>>>& notes)
{
	auto field = new Field_test(this);
	field->load_test(test, notes);
	field->show();
}


void Testing_algorithms::debug_wrong_algorithm()
{
	load_test();

	Solver_test solver;
	solver.load_puzzle(tests[0]);

	solver.solve_without_newest_algorythm();

	//test->show_test(solver.get_sudoku(), solver.get_notes());

	solver.simple_chains();
}

void Testing_algorithms::solve_without_some_algorithms(Solver& solver)
{
	// Вставляти лише правильні алгоритми
	while (solver.single_candidate() || solver.single_position() || solver.candidate_lines() ||
			solver.double_pairs() || solver.naked_pair() || solver.hidden_numbers())
	{ }
}

void Testing_algorithms::solve_with_wrong_algorithm(Solver& solver)
{
	// Ставити тут точку входу в зневаджувач
	while (solver.x_wings())
	{}
}


QList<QList<int>> Testing_algorithms::load_failed_random_generated_test()
{
	QFile file("failed_random_generated_test.txt");
	file.open(QIODevice::ReadOnly | QIODevice::Text);

	QTextStream in(&file);
	QString test_string;
	in >> test_string;
	file.close();

	int field_index = 0;
	QList<QList<int>> test { 9, QList<int>(9, 0) };

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			test[row][col] = test_string[field_index].digitValue();
			field_index++;
		}
	}

	return test;
}

void Testing_algorithms::test_simple_chains_with_random_generated_tests()
{
	generate_tests(100);

	Solver_test solver;

	for (int test = 0; test < 100; test++)
	{
		solver.load_puzzle(tests[test]);

		solver.solve_without_newest_algorythm();

		solver.solve_with_newest_algorythm();
	}
}

void Testing_algorithms::test_simple_chains_with_failed_random_generated_test()
{

	auto field = load_failed_random_generated_test();

	Solver_test solver;
	solver.load_puzzle(field);
	solver.solve_without_newest_algorythm();

	//solver.wxyz_wing();


}

void Testing_algorithms::save_failed_random_generated_test(int index)
{
	Solver_test solver;
	solver.load_puzzle(tests[index]);

	solver.solve_without_newest_algorythm();

	auto field_without_failed_algorithm = solver.get_sudoku();


	QString test_string;

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
			test_string += QString::number(field_without_failed_algorithm[row][col]);
	}

	QFile file("failed_random_generated_test.txt");
	file.open(QIODevice::WriteOnly | QIODevice::Text);

	QTextStream out(&file);
	out << test_string;
	file.close();
}

void Testing_algorithms::show_failed_random_generated_test(int index)
{
	Solver_test solver;
	solver.load_puzzle(tests[index]);

	solver.solve_without_newest_algorythm();

	show_test(solver.get_sudoku(), solver.get_notes());
}

void Testing_algorithms::check_with_random_generated_tests()
{
	generate_tests(1000);

	try
	{
		solve_generated_tests();
	}
	catch(std::exception error)
	{
		int failed_test_index = std::stoi(error.what());
		
		save_failed_random_generated_test(failed_test_index);
		show_failed_random_generated_test(failed_test_index);

		return;
	}

	auto* result_widget = new QWidget;

	test_results->addWidget(result_widget);

	auto* result_layout = new QHBoxLayout(result_widget);

	auto * label = new QLabel("Вирішувач працює справно");

	result_layout->addWidget(label, 0, Qt::AlignHCenter);
}

void Testing_algorithms::add_random_generated_test(QList<QList<int>>& field_without_algorithm, QList<QList<QList<int>>>& notes_without_algorithm, 
		QList<QList<int>>& field_with_algorithm, QList<QList<QList<int>>>& notes_with_algorithm,
		QList<QList<int>>& completed_sudoku)
{


	bool is_working = isIdentical(field_with_algorithm, completed_sudoku);

	auto* result_widget = new QWidget;

	test_results->addWidget(result_widget);

	auto* result_layout = new QHBoxLayout(result_widget);


	int completed_numbers = amount_of_numbers(field_with_algorithm); 

	auto* test_name_label = new QLabel(QString::number(completed_numbers));

	auto* test_result = new QLabel;

	if (is_working)
		test_result->setText("Усе правильно");
	else
		test_result->setText("Помилка");

	auto* show_test = new QPushButton("Переглянути перевірку");

	connect(show_test, &QPushButton::pressed, [=]()
		{
			test->show_test(field_without_algorithm, notes_without_algorithm, field_with_algorithm, notes_with_algorithm, completed_sudoku);
		});


	result_layout->addWidget(test_name_label);
	result_layout->addWidget(test_result);
	result_layout->addWidget(show_test);

}



void Testing_algorithms::generate_tests(int amount)
{
	Sudoku_generator generator;

	while (amount > 0)
	{
		generator.generate();
		generator.createPuzzle(25);

		tests.push_back(generator.get_uncompleted_sudoku());
		tests_answers.push_back(generator.get_completed_sudoku());

		amount--;
	}
}


void Testing_algorithms::solve_generated_tests()
{
	Solver_test solver;

	for (int test = 0; test < tests.size(); test++)
	{
		solver.load_puzzle(tests[test]);

		solver.solve_with_newest_algorythm();

		if (!isIdentical(solver.get_sudoku(),tests_answers[test]))
		{
			throw std::exception(std::to_string(test).c_str());
		}

	}
}









Testing_solver::Testing_solver(QWidget* parent) : QMainWindow(parent)
{
	auto* window = new QWidget;
	auto* window_layout = new QVBoxLayout(window);
	window->setWindowTitle("Перевірка вирішувача");

	setCentralWidget(window);

	test_field_window = new QMainWindow;
	auto* test_field_layout = new QVBoxLayout(test_field_window);
	test_field = new Field_test(test_field_window);

	test_field_layout->addWidget(test_field, 1);
	test_field_window->setMinimumSize(test_field->sizeHint());

	statistic_widget = new QWidget;
	results_widget = new QWidget;

	auto* scroll_area = new QScrollArea;
	scroll_area->setWidget(results_widget);
	scroll_area->setWidgetResizable(true);


	window_layout->addWidget(statistic_widget, 1);
	window_layout->addWidget(scroll_area, 2);



	check_solver_with_the_hardest_tests();
}

void Testing_solver::check_solver_with_the_hardest_tests()
{
	load_tests(1000);
	solve_tests();	// Використовуємо вирішувач судоку й записуємо результати
	fill_statistic_widget(); // створюємо верхню частину вікна, де записані загальні результати
	fill_results_widget(); // нижня частина вікна, де можна переглянути кожен результат
	show();
}


void Testing_solver::load_tests(int amount_of_tests)
{
	QString filename{ "top50000.txt" };
	QFile file(filename);

	file.open(QIODevice::ReadOnly | QIODevice::Text);

	if (!file.isOpen())
	{
		throw std::exception("Не вдалось відкрити файл для перевірки вирішувача");
	}
	QTextStream in(&file);

	tests.clear();

	for (int tests = 0; tests < amount_of_tests; tests++)
	{
		load_test(in);
	}
}

void Testing_solver::load_test(QTextStream& in)
{
	auto test_string = in.readLine();

	QList<QList<int>> test(9, QList<int>(9, 0));

	for (int i = 0; i < 81; i++)
	{
		if (test_string[i] != '0')
		{
			int row = i / 9;
			int col = i % 9;

			test[row][col] = test_string[i].digitValue();
		}
	}
	tests.push_back(test);
}



// Тут вирішувач буде вирішувати перевірки й зберігати результати
void Testing_solver::solve_tests()
{
	Solver_test solver;

	for (auto& test : tests)
	{
		solver.load_puzzle(test);

		solver.solve_with_newest_algorythm();
		//solver.solve_without_newest_algorythm();

		solver_result result;

		result.starting_amount = amount_of_numbers(test);
		result.ending_amount = amount_of_numbers(solver.get_sudoku());
		result.numbers = solver.get_sudoku();
		result.notes = solver.get_notes();

		results.push_back(result);
	}
}



// Зробити: додати результати й можливість переглядати результати

void Testing_solver::fill_results_widget()
{
	auto* layout = new QHBoxLayout(results_widget);

	auto* left_column = new QWidget;
	auto* right_column = new QWidget;

	auto* left_column_layout = new QVBoxLayout(left_column);
	auto* right_column_layout = new QVBoxLayout(right_column);

	layout->addWidget(left_column, 1, Qt::AlignHCenter);
	layout->addWidget(right_column, 1, Qt::AlignHCenter);

	int index = 0;

	for (const auto& result : results)
	{
		if (index % 2)
			add_result(result, left_column_layout);
		else
			add_result(result, right_column_layout);
		index++;
	}
}


void Testing_solver::add_result(const solver_result& result, QVBoxLayout* column)
{
	auto* widget = new QWidget;
	column->addWidget(widget);

	auto* layout = new QHBoxLayout(widget);

	auto* start_numbers = new QLabel(QString::number(result.starting_amount));
	auto* end_numbers = new QLabel(QString::number(result.ending_amount));
	auto* added_numbers = new QLabel(QString::number(result.ending_amount - result.starting_amount));
	auto* show = new QPushButton("Показати");

	layout->addWidget(start_numbers);
	layout->addWidget(end_numbers);
	layout->addWidget(added_numbers);
	layout->addWidget(show);

	connect(show, &QPushButton::pressed, [=]()
		{
			test_field->load_test(result.numbers, result.notes);
			test_field_window->show();
		});
}


void Testing_solver::fill_statistic_widget()
{
	fill_statistic();

	auto* layout = new QVBoxLayout(statistic_widget);


	auto* general_stats = new QWidget;
	auto* categories_stats = new QWidget;

	layout->addWidget(general_stats, 1);
	layout->addWidget(categories_stats, 2);

	auto* sudoku_amount = new QLabel("К-сть судоку: " + QString::number(statistic.amount_of_sudoku));
	auto* completed_sudoku = new QLabel("Вирішено: " + QString::number(statistic.completed_sudoku));
	auto* average_added_numbers = new QLabel("У середньому додано чисел: " + QString::number(statistic.average_added_numbers));

	auto* general_stats_layout = new QHBoxLayout(general_stats);

	general_stats_layout->addWidget(sudoku_amount);
	general_stats_layout->addWidget(completed_sudoku);
	general_stats_layout->addWidget(average_added_numbers);


	auto* categories_stats_layout = new QHBoxLayout(categories_stats);

	auto* first_col = new QWidget;
	auto* second_col = new QWidget;

	categories_stats_layout->addWidget(first_col);
	categories_stats_layout->addWidget(second_col);


	auto* first_col_layout = new QVBoxLayout(first_col);

	QString many_clues_winrate;

	if (statistic.many_clues.amount_of_sudoku != 0)
		many_clues_winrate = QString::number(statistic.many_clues.completed_sudoku / statistic.many_clues.amount_of_sudoku * 100);

	auto* many_clues = new QLabel(">=40 К-сть: " + QString::number(statistic.many_clues.amount_of_sudoku) + " "
		"Вирішено: " + QString::number(statistic.many_clues.completed_sudoku) + many_clues_winrate + " "
		"У середньому додано: " + QString::number(statistic.many_clues.average_added_numbers) + " ");


	QString moderate_clues_winrate;

	if (statistic.moderate_clues.amount_of_sudoku != 0)
		moderate_clues_winrate = QString::number(statistic.moderate_clues.completed_sudoku / statistic.moderate_clues.amount_of_sudoku * 100);

	auto* moderate_clues = new QLabel(">=30 К-сть: " + QString::number(statistic.moderate_clues.amount_of_sudoku) + " "
		"Вирішено: " + QString::number(statistic.moderate_clues.completed_sudoku) + moderate_clues_winrate + " "
		"У середньому додано: " + QString::number(statistic.moderate_clues.average_added_numbers) + " ");

	first_col_layout->addWidget(many_clues);
	first_col_layout->addWidget(moderate_clues);



	auto* second_col_layout = new QVBoxLayout(second_col);


	QString medium_clues_winrate;
	if (statistic.medium_clues.amount_of_sudoku != 0)
		medium_clues_winrate = QString::number(statistic.medium_clues.completed_sudoku / statistic.medium_clues.amount_of_sudoku * 100);

	auto* medium_clues = new QLabel(">=25 К-сть: " + QString::number(statistic.medium_clues.amount_of_sudoku) + " "
		"Вирішено: " + QString::number(statistic.medium_clues.completed_sudoku) + medium_clues_winrate + " "
		"У середньому додано: " + QString::number(statistic.medium_clues.average_added_numbers) + " ");



	QString few_clues_winrate;

	if (statistic.few_clues.amount_of_sudoku != 0)
		few_clues_winrate = QString::number(statistic.few_clues.completed_sudoku / statistic.few_clues.amount_of_sudoku * 100);

	auto* few_clues = new QLabel("<25 К-сть: " + QString::number(statistic.few_clues.amount_of_sudoku) + " "
		"Вирішено: " + QString::number(statistic.few_clues.completed_sudoku) + few_clues_winrate + " "
		"У середньому додано: " + QString::number(statistic.few_clues.average_added_numbers) + " ");


	second_col_layout->addWidget(medium_clues);
	second_col_layout->addWidget(few_clues);
}

void Testing_solver::fill_statistic()
{
	int many = 40;
	int moderate = 30;
	int medium = 25;

	statistic.amount_of_sudoku = results.size();


	for (const auto& result : results)
	{
		statistic.average_added_numbers += result.ending_amount - result.starting_amount;


		if (result.ending_amount == 81)
		{
			if (result.starting_amount >= many)
				statistic.many_clues.completed_sudoku++;
			else if (result.starting_amount >= moderate)
				statistic.moderate_clues.completed_sudoku++;
			else if (result.starting_amount >= medium)
				statistic.medium_clues.completed_sudoku++;
			else if (result.starting_amount < medium)
				statistic.few_clues.completed_sudoku++;

			statistic.completed_sudoku++;
		}

		if (result.starting_amount >= many)
		{
			statistic.many_clues.amount_of_sudoku++;
			statistic.many_clues.average_added_numbers += result.ending_amount - result.starting_amount;
		}
		else if (result.starting_amount >= moderate)
		{
			statistic.moderate_clues.amount_of_sudoku++;
			statistic.moderate_clues.average_added_numbers += result.ending_amount - result.starting_amount;
		}
		else if (result.starting_amount >= medium)
		{
			statistic.medium_clues.amount_of_sudoku++;
			statistic.medium_clues.average_added_numbers += result.ending_amount - result.starting_amount;
		}
		else if (result.starting_amount < medium)
		{
			statistic.few_clues.amount_of_sudoku++;
			statistic.few_clues.average_added_numbers += result.ending_amount - result.starting_amount;
		}
	}

	if (statistic.many_clues.amount_of_sudoku != 0)
		statistic.many_clues.average_added_numbers /= statistic.many_clues.amount_of_sudoku;

	if (statistic.moderate_clues.amount_of_sudoku != 0)
		statistic.moderate_clues.average_added_numbers /= statistic.moderate_clues.amount_of_sudoku;

	if (statistic.medium_clues.amount_of_sudoku != 0)
		statistic.medium_clues.average_added_numbers /= statistic.medium_clues.amount_of_sudoku;

	if (statistic.few_clues.amount_of_sudoku != 0)
		statistic.few_clues.average_added_numbers /= statistic.few_clues.amount_of_sudoku;

	statistic.average_added_numbers /= statistic.amount_of_sudoku;
}


