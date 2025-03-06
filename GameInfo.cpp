#include "GameInfo.h"
#include <QTextStream>
#include <QRandomGenerator>


GameInfo::GameInfo() : records_filename("records.txt"), saved_game_filename("saved_game.txt"), settings_filename("settings.txt")
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

	data.open(QIODevice::ReadOnly | QIODevice::Text);

	bool is_empty = false;

	if (data.size() == 0)
		is_empty = true;

	data.close();

	return is_empty;
}

bool GameInfo::is_settings_file_empty()
{
	data.setFileName(settings_filename);

	data.open(QIODevice::ReadOnly | QIODevice::Text);

	bool is_empty = false;

	if (data.size() == 0)
		is_empty = true;

	data.close();

	return is_empty;
}

QPair<QList<QList<int>>, QList<QList<int>>> GameInfo::get_hard_sudoku()
{
	data.setFileName("hard_sudoku.txt");

	data.open(QIODevice::ReadOnly | QIODevice::Text);

	QTextStream in(&data);

	// Один рядок займає 166 символів. 81 * 2 + 3 + 1. Рядок містить повне й не повне поле, між ними розмежовувач " | " і в кінці \n
	int range = data.size() / 166;

	int sudoku_index = QRandomGenerator::global()->bounded(0, range);

	int sudoku_position_in_file = sudoku_index * 166;

	data.seek(sudoku_position_in_file);

	QString string = data.readLine();

	QList<QList<int>> uncompleted_sudoku(9, QList<int>(9, 0));
	QList<QList<int>> completed_sudoku(9, QList<int>(9, 0));

	for (int i = 0; i < 81; i++)
	{
		int row = i / 9;
		int col = i % 9;
		uncompleted_sudoku[row][col] = string[i].digitValue();
	}

	for (int i = 0; i < 81; i++)
	{
		int row = i / 9;
		int col = i % 9;
		completed_sudoku[row][col] = string[i + 84].digitValue();
	}

	data.close();

	return { completed_sudoku, uncompleted_sudoku };
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

void GameInfo::load_settings(bool& fill_candidates, bool& remove_candidates, QString& theme_name)
{
	data.setFileName(settings_filename);

	data.open(QIODevice::ReadOnly | QIODevice::Text);

	QTextStream in(&data);

	QString fill;
	QString remove;

	in >> fill;
	in >> remove;
	in >> theme_name;

	fill_candidates = fill == "1";
	remove_candidates = remove == "1";

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

void GameInfo::save_settings(bool fill_candidates, bool remove_candidates, QString theme_name)
{
	data.setFileName(settings_filename);

	data.open(QIODevice::WriteOnly | QIODevice::Text);

	QTextStream out(&data);

	out << fill_candidates << " ";
	out << remove_candidates << " ";
	out << theme_name << "\n";

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