#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QTime>
#include <QThread>
#include <QTimer>


#include "Sudoku_maker.h"


/*
	Клас, який відповідає за створення та збереження складних судоку, що займають забагато часу для створення, щоб їх створювати під час гри
*/
class Maker_window : public QMainWindow
{
	Q_OBJECT

public:
	Maker_window();
	~Maker_window();

public slots:
	void add_new_sudoku(QList<QList<int>> sudoku, QList<QList<int>> solution);

	void generate_sudokus();

private:
	void save_sudokus();
	void change_count_label();
	void change_time_label();
	void change_data_label();

	QList<QThread *> threads;
	QList<QPair<QList<QList<int>>, QList<QList<int>>>> sudokus_and_solutions;

	int finished_threads = 0;

	QLabel * sudoku_count_label;
	QLabel * data_label;
	QLabel * time_label;

	QTimer * timer;
	int time;

	int completed_numbers;
	int tries;

	Difficulties required_difficulty;
};
