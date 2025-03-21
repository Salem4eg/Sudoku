#pragma once
#include "Structures.h"
#include <QFile>
#include "SQLite/sqlite3.h"


// Клас, що відповідає за завантаження\збереження ігрової інформації
class GameInfo
{
public:
	GameInfo();
	~GameInfo();

public slots:
	void save_settings(Settings settings);
	Settings get_settings();
	
	void save_game(const QList<QList<int>>& numbers, const QList<QList<QList<int>>>& notes, const QList<QList<int>>& solution, const Record& record);
	void load_game(QList<QList<int>>& numbers, QList<QList<QList<int>>>& notes, QList<QList<int>>& solution, Record& record);
	
	void save_records(const QList<QList<QList<Record>>>& records);
	QList<Record> get_records();

	QPair<QList<QList<int>>,QList<QList<int>>> get_random_hard_sudoku_and_solution();

	bool has_settings();
	bool has_saved_game();
	void clear_saved_game();
private:
	void create_tables();
	void save_record(const Record& record);
	QList<QList<int>> get_array_from_string(QString& string);
	int get_max_id();
	void clear_records();

	sqlite3 * database;
};

