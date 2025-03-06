#pragma once
#include "Structures.h"
#include <QFile>


// Клас, що відповідає за завантаження\збереження ігрової інформації
class GameInfo
{
public:
	GameInfo();
	void save_game(QList<QList<int>>& numbers, QList<QList<QList<int>>>& notes, Record record, QList<QList<int>>& completed_field);
	void save_records(QList<QList<QList<Record>>> records);
	void save_settings(bool fill_candidates, bool remove_candidates, QString theme_name);

	void clear_saved_game_file();
	bool is_saved_game_file_empty();
	bool is_settings_file_empty();

	QPair<QList<QList<int>>,QList<QList<int>>> get_hard_sudoku();
	void load_game(QList<QList<int>>& numbers, QList<QList<QList<int>>>& notes, Record& record, QList<QList<int>>& completed_field);
	void load_settings(bool& fill_candidates, bool& remove_candidates, QString& theme_name);
	QList<Record> get_records();

private:
	QString records_filename;
	QString saved_game_filename;
	QString settings_filename;

	QFile data;
};

