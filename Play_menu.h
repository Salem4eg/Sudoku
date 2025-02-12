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
#include "GameInfo.h"

class Play_menu : public QWidget
{
	Q_OBJECT

public:
	Play_menu(QWidget *parent = nullptr);
	~Play_menu();

public slots:
	void start_game(Difficulties game_difficulty = Difficulties::hard);
	void continue_game();
	void change_field_colors(QColor field_color, QColor field_border);
	bool has_saved_game();
	void save_game();
signals:
	void leave();
	void field_ready();
	void game_saved(bool saved);
	void test();
	void new_record(Record record);


private:
	void fill_field();

	void increase_time();
	void update_time_label();

	void increase_errors();
	void update_errors_label();

	void increase_hints();
	void update_hints_label();

	void update_difficulty_label();

	void game_won();
	void game_lost();


	void load_game();

	Field * field;

	Result * result_window;

	GameInfo game_info;

	QTimer * timer;
	bool notepad_mode;
	int time_elapsed;
	int errors;
	int hints;
	Difficulties difficulty;

	bool need_to_save_game;

	QLabel * difficulty_label;
	QLabel * time_label;
	QLabel * errors_label;
	QLabel * hints_label;
	QList<QLabel *> numbers_labels;
};

// При натискані на "нова гра" має показуватись вибір складності (легка, нормальна, складна, надскладна) 
// (Воно має бути на вже очищеному екрані після головного меню, де має бути "вікно" з вертикальним показом складності + вихід у меню)
// Після того, треба зробити судоку відповідної складності (кляті алгоритми((( ) і показувати це судоку на екран

// Додаткове, просунуті підказки, які показують на клітинку та кажуть, яку техніку тут варто використати