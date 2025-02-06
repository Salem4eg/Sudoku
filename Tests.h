#pragma once

#include <QWidget>
#include <QLabel>
#include <QFile>

#include "Sudoku_generator.h"
#include <QMainWindow>

#include "Test_utilities.h"





class Test_result : QMainWindow
{
	Q_OBJECT;
public:
	Test_result(QWidget * parent = nullptr);

	void show_test(QList<QList<int>> numbers, QList<QList<QList<int>>> notes, QList<QList<QList<int>>> expected_notes,
		QList<QList<int>> result_numbers, QList<QList<QList<int>>> result_notes);

	void show_test(QList<QList<int>> field_without_algorithm, QList<QList<QList<int>>> notes_without_algorithm,
		QList<QList<int>> field_with_algorithm, QList<QList<QList<int>>> notes_with_algorithm,
		QList<QList<int>> completed_sudoku);

	void show_test(QList<QList<int>> numbers, QList<QList<QList<int>>> notes);

protected:
void closeEvent(QEvent * event);

private:

	QMainWindow * test_window;
	QMainWindow * expected_window;
	QMainWindow * result_window;

	Field_test * test;
	Field_test * expected;
	Field_test * result;
};


class Testing_algorithms : public QMainWindow
{
	Q_OBJECT;
public:
	Testing_algorithms(QWidget * parent = nullptr); 

	/*
	Додатковий ступінь перевірки алгоритму, який перевіряє алгоритми в звичайних судоку
	Перевіряє, чи правильно виставлені числа, або чи не зависне вирішувач
	*/
	void check_with_random_generated_tests();
	
	void test_x_wing();
	void test_swordfish();
	void test_jellyfish();

	void test_bi_location_chain();
	void test_simple_chains();

	void test_xy_wing();
	void test_xyz_wing();
	void test_wxyz_wing();


	void debug_wrong_algorithm();

	void save_failed_random_generated_test(int index);
	QList<QList<int>> load_failed_random_generated_test();
	void show_failed_random_generated_test(int index);
	void test_simple_chains_with_failed_random_generated_test();
	void test_simple_chains_with_random_generated_tests();
private:
	QVBoxLayout * test_results;
	Test_result * test;

	
	QList<QList<QList<int>>> tests;
	QList<QList<QList<int>>> tests_answers; 
	QList<int> wrong_answers;

	void generate_tests(int amount);
	// Вирішує випадково створені завдання і перевіряє на помилки
	void solve_generated_tests();

	// Вирішуємо завдання тими алгоритмами, що точно справні
	void solve_without_some_algorithms(Solver& solver);
	void solve_with_wrong_algorithm(Solver& solver);

	void save_test();
	void load_test();
	void show_test(const QList<QList<int>>& test, const QList<QList<QList<int>>>& notes);

	void clear();
	bool is_removed(QList<QList<QList<int>>>& result_notes, QList<QList<QList<int>>>& expected_to_remove_notes);
	// Для наперед створених завдань
	void add_test(QList<QList<int>>& numbers, QList<QList<QList<int>>>& notes, QList<QList<QList<int>>>& expected_to_remove_notes,
	QList<QList<int>>& result_numbers, QList<QList<QList<int>>>& result_notes, QString test_name = "Test");

	// Для випадково створених завдань
	void add_random_generated_test(QList<QList<int>>& field_without_algorithm, QList<QList<QList<int>>>& notes_without_algorithm, 
		QList<QList<int>>& field_with_algorithm, QList<QList<QList<int>>>& notes_with_algorithm,
		QList<QList<int>>& completed_sudoku);

};





class Testing_solver : public QMainWindow
{
	Q_OBJECT;

	struct solver_result
	{
		int starting_amount;
		int ending_amount;
		QList<QList<int>> numbers;
		QList<QList<QList<int>>> notes;
	};

	struct sudoku_category_statistic
	{
		int amount_of_sudoku;
		int completed_sudoku;
		int average_added_numbers;
	};

	struct solver_statistic
	{
		int amount_of_sudoku;
		int completed_sudoku;
		int average_added_numbers;


		sudoku_category_statistic many_clues;
		sudoku_category_statistic moderate_clues;
		sudoku_category_statistic medium_clues;
		sudoku_category_statistic few_clues;

	};

public:
	Testing_solver(QWidget * parent = nullptr);

	
	void check_solver_with_the_hardest_tests();


	
protected:
	void load_test(QTextStream& in);
	void load_tests(int amount_of_tests);
	

	// Вирішує завдання з tests, що завантажені з load_test й зберігає результати в results
	void solve_tests();

	

	void fill_statistic_widget();
	void fill_statistic();
	void fill_results_widget();
	
	void add_result(const solver_result& result, QVBoxLayout* column);

private:
	QList<QList<QList<int>>> tests;
	QList<solver_result> results;
	solver_statistic statistic;


	QWidget * statistic_widget;
	QWidget	* results_widget;
	Solver_test * solver_test;
	Field_test * test_field;
	QMainWindow * test_field_window;
};