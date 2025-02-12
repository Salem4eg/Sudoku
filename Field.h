#pragma once

#include <QWidget>
#include <QList>
#include <QKeyEvent>
#include "Cell.h"
#include <QGridLayout>


class Field  : public QWidget
{
	Q_OBJECT


public slots:
	void set_active_cell(Cell * cell);
	void toggle_notepad_mode();
	void change_field_theme(QColor field, QColor field_border);
signals:
	void wrong_number();
	void finished_field();
	void toggle_notepad();

public:
	Field(QWidget *parent);
	~Field();

	void set_numbers(QList<QList<int>> finished_field, QList<QList<int>> unfinished_field);

	void clear();
	bool show_hint();

	void load_field(QList<QList<int>> numbers, QList<QList<QList<int>>> notes, QList<QList<int>> completed);
	QPair<QList<QList<int>>, QList<QList<QList<int>>>> get_field();
	QList<QList<int>> get_completed_field();
	
protected:
	void keyPressEvent(QKeyEvent * event) override;
	QList<QList<Cell *>> get_cells();
private:
	bool is_correct_number(int number, QPoint coords);
	void set_cells_highlighting_style();
	void set_cells_default_style();

	QList<QList<Cell *>> field;
	QHash<int, QList<Cell *>> cells_by_number;
	QHash<int, QList<Cell *>> cells_by_candidate;
	Cell * active_cell;
	bool notepad_mode;
	QList<QList<int>> completed_field;
	QList<QList<int>> uncompleted_field;
	int finished_cells;
};
