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
	void change_field_colors(QColor field_color, QColor field_border);
signals:
	void leave();
	void field_ready();

private:
	void fill_field();
	void update_time();
	void update_errors();
	void game_won();
	void game_lost();

	void save_record(Record& record);

	Field * field;

	Result * result_window;

	QTimer * timer;
	bool notepad_mode;
	int time_elapsed;
	int errors;
	int hints;
	Difficulties difficulty;

	QLabel * difficulty_label;
	QLabel * time_label;
	QLabel * errors_label;
	QLabel * hints_label;
	QList<QLabel *> numbers_label;
};

// При натискані на "нова гра" має показуватись вибір складності (легка, нормальна, складна, надскладна) 
// (Воно має бути на вже очищеному екрані після головного меню, де має бути "вікно" з вертикальним показом складності + вихід у меню)
// Після того, треба зробити судоку відповідної складності (кляті алгоритми((( ) і показувати це судоку на екран

// Додаткове, просунуті підказки, які показують на клітинку та кажуть, яку техніку тут варто використати