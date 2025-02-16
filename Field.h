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
	void highlight_number(int number);
	void fill_candidates_at_start(bool fill);
	void remove_invalid_candidates(bool remove);
signals:
	void wrong_number();
	void finished_field();
	void toggle_notepad();
	void number_finished(int number);

public:
	Field(QWidget *parent);
	~Field();

	void set_numbers(QList<QList<int>> finished_field, QList<QList<int>> unfinished_field);

	void clear();
	bool show_hint();
	void set_cells_default_style();
	void set_cells_highlighting_style();

	void load_field(QList<QList<int>> numbers, QList<QList<QList<int>>> notes, QList<QList<int>> completed);
	QPair<QList<QList<int>>, QList<QList<QList<int>>>> get_field();
	QList<QList<int>> get_completed_field();
	
protected:
	void keyPressEvent(QKeyEvent * event) override;
	QList<QList<Cell *>> get_cells();
private:
	void unhighlight_number(int number);
	void clear_invalid_candidates();
	bool is_correct_number(int number, QPoint coords);
	void fill_candidates();
	bool is_valid_candidate(int number, int cell_row, int cell_col);

	QList<QList<Cell *>> field;
	// Клітинки на яких є число | використовується для підсвітки клітинок
	QHash<int, QList<Cell *>> cells_by_number;
	// Клітинки на яких є кандидат | використовується для підсвітки кандидатів
	QHash<int, QList<Cell *>> cells_by_candidate;
	Cell * active_cell;
	bool notepad_mode;
	// Використовується для перевірки на помилки
	QList<QList<int>> completed_field;
	// Зберігає вже завершені клітинки
	QList<QList<int>> uncompleted_field;
	int finished_cells;

	QList<int> placed_count;

	int highlighted_number;

	bool auto_fill_candidates_at_start;
	bool auto_remove_invalid_candidates;
};
