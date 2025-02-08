#pragma once
#include "Structures.h"

class GameInfo
{
	void save_game(QList<QList<int>> numbers, QList<QList<QList<int>>> notes, Record record);
	void save_record(Record record);

	QList<Record> get_records();

};

