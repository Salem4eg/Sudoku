#pragma once

#include <QWidget>
#include "Field.h"

#include "Sudoku_generator.h"
#include "Solver.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include "Result.h"

class Play_menu : public QWidget
{
	Q_OBJECT

public:
	Play_menu(QWidget *parent = nullptr);
	~Play_menu();

public slots:
	void start_game(Difficulties game_difficulty = Difficulties::hard);
	void continue_game();
	void change_theme(Theme theme);

	void fill_field(QPair<QList<QList<int>>, QList<QList<int>>> solution_and_sudoku);

	void get_saved_game(QList<QList<int>>& numbers, QList<QList<QList<int>>>& notes, QList<QList<int>>& completed_field, Record& stats);
	void load_game(QList<QList<int>> numbers, QList<QList<QList<int>>> notes, QList<QList<int>> completed_field, Record stats);

	void fill_candidates_at_start(bool fill);
	void remove_invalid_candidates(bool remove);
signals:
	void leave();
	void field_ready();
	void need_to_save_game(bool save);
	void test();
	void new_record(Record record);
	void game_finished(bool finished);
	void request_new_hard_sudoku();


private:


	QPair<QList<QList<int>>, QList<QList<int>>> generate_sudoku();

	void increase_time();
	void update_time_label();

	void increase_errors();
	void update_errors_label();

	void increase_hints();
	void update_hints_label();

	void update_difficulty_label();

	void game_won();
	void game_lost();



	void set_inactive_number_button(int number);
	void set_default_number_button();

	

	Field * field;

	Result * result_window;

	QTimer * timer;
	bool notepad_mode;
	int time_elapsed;
	int errors;
	int hints;
	Difficulties difficulty;

	bool has_unfinished_game;

	QLabel * difficulty_label;
	QLabel * time_label;
	QLabel * errors_label;
	QLabel * hints_label;
	QList<QPushButton *> numbers_button;
};
