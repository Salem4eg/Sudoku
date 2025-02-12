#include "GameInfo.h"
#include <QTextStream>


GameInfo::GameInfo() : records_filename("records.txt"), saved_game_filename("saved_game.txt")
{

}

void GameInfo::save_game(QList<QList<int>>& numbers, QList<QList<QList<int>>>& notes, Record record, QList<QList<int>>& completed_field)
{
	data.setFileName(saved_game_filename);

	data.open(QIODevice::WriteOnly | QIODevice::Text);

	QString numbers_string;
	numbers_string.reserve(81);

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			numbers_string += QString::number((numbers[row][col]));
		}
	}

	QString completed_field_string;
	completed_field_string.reserve(81);

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			completed_field_string += QString::number((completed_field[row][col]));
		}
	}

	QString notes_string;
	notes_string.reserve(81);

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			for (int number : notes[row][col])
			{
				notes_string += QString::number(number);
			}
			notes_string += "|";
		}
	}


	QTextStream out(&data);

	out << numbers_string << "\n";
	out << completed_field_string << "\n";
	out << notes_string << "\n";
	out << record.time << " ";
	out << record.errors << " ";
	out << record.hints << " ";
	out << record.difficulty << "\n";

	data.close();
}

void GameInfo::clear_saved_game_file()
{
	data.setFileName(saved_game_filename);

	data.open(QIODevice::WriteOnly | QIODevice::Text);

	data.close();
}

bool GameInfo::is_saved_game_file_empty()
{
	data.setFileName(saved_game_filename);

	data.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

	bool is_empty = false;

	if (data.size() == 0)
		is_empty = true;

	data.close();

	return is_empty;
}

void GameInfo::load_game(QList<QList<int>>& numbers, QList<QList<QList<int>>>& notes, Record& record, QList<QList<int>>& completed_field)
{
	data.setFileName(saved_game_filename);

	data.open(QIODevice::ReadOnly | QIODevice::Text);

	QTextStream in(&data);

	QString numbers_string;
	QString completed_field_string;
	QString notes_string;
	QString time;
	QString errors;
	QString hints;
	QString difficulty;

	in >> numbers_string;
	in >> completed_field_string;
	in >> notes_string;
	in >> time;
	in >> errors;
	in >> hints;
	in >> difficulty;

	for (int index = 0; index < 81; index++)
	{
		int row = index / 9;
		int col = index % 9;

		numbers[row][col] = numbers_string[index].digitValue();
	}

	for (int index = 0; index < 81; index++)
	{
		int row = index / 9;
		int col = index % 9;

		completed_field[row][col] = completed_field_string[index].digitValue();
	}

	int row = 0;
	int col = 0;

	QList<int> cell;
	
	for (QChar ch : notes_string)
	{
		if (ch == '|')
		{
			notes[row][col] = cell;
			cell.clear();

			col++;

			if (col >= 9)
			{
				row++;
				col %= 9;
			}
			continue;
		}

		cell.push_back(ch.digitValue());
	}

	record.time = time.toInt();
	record.errors = errors.toInt();
	record.hints = hints.toInt();
	record.difficulty = Difficulties(difficulty.toInt());

	data.close();
}

void GameInfo::save_records(QList<QList<QList<Record>>> records)
{
	data.setFileName(records_filename);

	data.open(QIODevice::WriteOnly | QIODevice::Text);

	QTextStream out(&data);

	for (auto& difficulty : records)
	{
		int category_index = 0;

		for (auto& category : difficulty)
		{
			for (auto& record : category)
			{
				out << record.time << " ";
				out << record.errors << " ";
				out << record.hints << " ";
				out << record.difficulty << "\n";
			}
		}
		category_index++;
	}

	data.close();
}

QList<Record> GameInfo::get_records()
{
	QList<Record> records;

	data.setFileName(records_filename);

	data.open(QIODevice::ReadOnly | QIODevice::Text);

	QTextStream in(&data);

	while (!in.atEnd())
	{
		Record record;

		in >> record.time;

		if (in.status() != QTextStream::Ok)
			break;

		in >> record.errors;
		in >> record.hints;

		int difficulty;
		in >> difficulty;

		if (difficulty >= 0 && difficulty <= 3)
			record.difficulty = static_cast<Difficulties>(difficulty);
		else
			record.difficulty = easy;

		records.push_back(record);
	}

	data.close();

	return records;
}