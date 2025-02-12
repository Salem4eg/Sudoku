#pragma once
#include "Structures.h"
#include <QFile>

class GameInfo
{
public:
	GameInfo();
	void save_game(QList<QList<int>>& numbers, QList<QList<QList<int>>>& notes, Record record, QList<QList<int>>& completed_field);
	void save_records(QList<QList<QList<Record>>> records);

	void clear_saved_game_file();
	bool is_saved_game_file_empty();

	void load_game(QList<QList<int>>& numbers, QList<QList<QList<int>>>& notes, Record& record, QList<QList<int>>& completed_field);
	QList<Record> get_records();

private:
	QString records_filename;
	QString saved_game_filename;

	QFile data;
};

