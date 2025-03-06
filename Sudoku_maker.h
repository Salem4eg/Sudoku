#pragma once

#include <QObject>

#include "Sudoku_generator.h"
#include "Solver.h"

class Sudoku_maker : public QObject
{
	Q_OBJECT

public:

	Sudoku_maker(QObject * parent = nullptr);
	~Sudoku_maker();

public slots:
	void create_hard_sudoku(int tries, int completed_numbers);
	void create_normal_sudoku(int tries, int completed_numbers);
	void create_easy_sudoku(int tries, int completed_numbers);

signals:
	void hard_sudoku_created(QList<QList<int>> sudoku, QList<QList<int>> completed_sudoku);
	void normal_sudoku_created(QList<QList<int>> sudoku, QList<QList<int>> completed_sudoku);
	void easy_sudoku_created(QList<QList<int>> sudoku, QList<QList<int>> completed_sudoku);
	void finished();
};