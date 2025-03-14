#include "GameInfo.h"
#include <QTextStream>
#include <QRandomGenerator>


GameInfo::GameInfo()
{
	sqlite3_open("database.db", &database);

	create_tables();
}

GameInfo::~GameInfo()
{
	sqlite3_close(database);
}

void GameInfo::create_tables()
{
	QString create_settings_table("CREATE TABLE IF NOT EXISTS SETTINGS("
								  "ID							INTEGER		PRIMARY KEY CHECK (ID = 1),"
								  "FILL_CANDIDATES_AT_START		INTEGER		NOT NULL,"
								  "AUTO_REMOVE_CANDIDATES		INTEGER		NOT NULL,"
								  "THEME						TEXT		NOT NULL);"
	);

	char * error_message;

	int result = sqlite3_exec(database, create_settings_table.toUtf8().constData(), nullptr, nullptr, &error_message);

	if (result != SQLITE_OK)
		throw std::exception("Не вдалось створити таблицю налаштувань");



    QString create_saved_game_table("CREATE TABLE IF NOT EXISTS SAVED_GAME("
                                    "ID             INTEGER      PRIMARY KEY CHECK (ID = 1),"
                                    "NUMBERS        TEXT,"
                                    "NOTES          TEXT,"
                                    "SOLUTION       TEXT,"
                                    "ELAPSED_TIME   INTEGER,"
                                    "ERRORS         INTEGER,"
                                    "HINTS          INTEGER,"
                                    "DIFFICULTY     INTEGER);");

    result = sqlite3_exec(database, create_saved_game_table.toUtf8().constData(), nullptr, nullptr, &error_message);

    if (result != SQLITE_OK)
        throw std::exception("Не вдалось створити таблицю збереженої гри");


    QString create_records_table("CREATE TABLE IF NOT EXISTS RECORDS("
                                 "ELAPSED_TIME  INTEGER,"
                                 "ERRORS        INTEGER,"
                                 "HINTS         INTEGER,"
                                 "DIFFICULTY    INTEGER);");

    result = sqlite3_exec(database, create_records_table.toUtf8().constData(), nullptr, nullptr, &error_message);

    if (result != SQLITE_OK)
        throw std::exception("Не вдалось створити таблицю рекордів");


    QString create_hard_sudoku_table("CREATE TABLE IF NOT EXISTS HARD_SUDOKUS("
                                     "ID        INTEGER PRIMARY KEY,"
                                     "SUDOKU    TEXT,"
                                     "SOLUTION  TEXT);");

    result = sqlite3_exec(database, create_hard_sudoku_table.toUtf8().constData(), nullptr, nullptr, &error_message);

    if (result != SQLITE_OK)
        throw std::exception("Не вдалось створити таблицю складних судоку");

}

void GameInfo::save_settings(Settings settings)
{
    sqlite3_stmt * stmt;

    QString insert_or_replace_settings("INSERT OR REPLACE INTO SETTINGS("
                                       "ID, FILL_CANDIDATES_AT_START, AUTO_REMOVE_CANDIDATES, THEME)"
                                       "VALUES (1, ?, ?, ?);"
    );

    int result = sqlite3_prepare_v2(database, insert_or_replace_settings.toUtf8().constData(), -1, &stmt, nullptr);

    if (result != SQLITE_OK) 
    {
        throw std::exception("Не вдалося підготувати вираз для вставки або заміни налаштувань");
    }

    sqlite3_bind_int(stmt, 1, settings.fill_field_with_candidates_at_start);
    sqlite3_bind_int(stmt, 2, settings.remove_invalid_candidates);
    sqlite3_bind_text(stmt, 3, settings.theme_name.toUtf8().constData(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);

    if (result != SQLITE_DONE) 
    {
        sqlite3_finalize(stmt);
        throw std::exception("Не вдалося виконати вираз для вставки або заміни налаштувань");
    }

    sqlite3_finalize(stmt);
}

Settings GameInfo::get_settings()
{
    QString load_settings("SELECT FILL_CANDIDATES_AT_START, AUTO_REMOVE_CANDIDATES, THEME FROM SETTINGS WHERE ID = 1;");

    sqlite3_stmt * statement;

    int result = sqlite3_prepare_v2(database, load_settings.toUtf8().constData(), -1, &statement, 0);

    if (result != SQLITE_OK)
        throw std::exception("Не вдалось завантажити налаштування");

    result = sqlite3_step(statement);

	if (result != SQLITE_ROW)
	{
		sqlite3_finalize(statement);
		throw std::exception("Не вдалось отримати налаштування");
	}

    Settings settings;

    settings.fill_field_with_candidates_at_start = sqlite3_column_int(statement, 0);
    settings.remove_invalid_candidates = sqlite3_column_int(statement, 1);

    settings.theme_name = QString::fromUtf8(reinterpret_cast<const char *>(sqlite3_column_text(statement, 2)));

    sqlite3_finalize(statement);

    return settings;
}

void GameInfo::save_game(const QList<QList<int>>& numbers, const QList<QList<QList<int>>>& notes, const QList<QList<int>>& solution, const Record& record)
{
    QString numbers_str;
    QString notes_str;
    QString solution_str;

    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            numbers_str += QString::number(numbers[row][col]);
        }
    }

    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            for (int number : notes[row][col])
                notes_str += QString::number(number);
            notes_str += "|";
        }
    }

    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            solution_str += QString::number(solution[row][col]);
        }
    }


    QString save_game_query("INSERT INTO SAVED_GAME (ID, NUMBERS, NOTES, SOLUTION, ELAPSED_TIME, ERRORS, HINTS, DIFFICULTY) VALUES (1, ?, ?, ?, ?, ?, ?, ?) "
                            "ON CONFLICT(ID) DO UPDATE SET NUMBERS = excluded.NUMBERS, NOTES = excluded.NOTES, SOLUTION = excluded.SOLUTION,"
                            "ELAPSED_TIME = excluded.ELAPSED_TIME, ERRORS = excluded.ERRORS, HINTS = excluded.HINTS, DIFFICULTY = excluded.DIFFICULTY;");

    sqlite3_stmt * statement;

    int result = sqlite3_prepare_v2(database, save_game_query.toUtf8().constData(), -1, &statement, nullptr);

    if (result != SQLITE_OK) 
    {
        throw std::exception("Не вдалося підготувати вираз для вставки або заміни налаштувань");
    }

    sqlite3_bind_text(statement, 1, numbers_str.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 2, notes_str.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 3, solution_str.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(statement,  4, record.time);
    sqlite3_bind_int(statement,  5, record.errors);
    sqlite3_bind_int(statement,  6, record.hints);
    sqlite3_bind_int(statement,  7, record.difficulty);

    result = sqlite3_step(statement);

    if (result != SQLITE_DONE) 
    {
        sqlite3_finalize(statement);
        throw std::exception("Не вдалося виконати вираз для вставки або заміни збереженої гри");
    }

    sqlite3_finalize(statement);
}

void GameInfo::load_game(QList<QList<int>>& numbers, QList<QList<QList<int>>>& notes, QList<QList<int>>& solution, Record& record)
{
    QString load_saved_game("SELECT NUMBERS, NOTES, SOLUTION, ELAPSED_TIME, ERRORS, HINTS, DIFFICULTY FROM SAVED_GAME WHERE ID = 1;");

    sqlite3_stmt * statement;

    int result = sqlite3_prepare_v2(database, load_saved_game.toUtf8().constData(), -1, &statement, 0);

    if (result != SQLITE_OK)
        throw std::exception("Не вдалось завантажити збережену гру");

    result = sqlite3_step(statement);

	if (result != SQLITE_ROW)
	{
		sqlite3_finalize(statement);
		throw std::exception("Не вдалось отримати збережену гру");
	}

    QString numbers_str = reinterpret_cast<const char *>(sqlite3_column_text(statement, 0));
    QString notes_str = reinterpret_cast<const char *>(sqlite3_column_text(statement, 1));
    QString solution_str = reinterpret_cast<const char *>(sqlite3_column_text(statement, 2));

    record.time = sqlite3_column_int(statement, 3);
    record.errors = sqlite3_column_int(statement, 4);
    record.hints = sqlite3_column_int(statement, 5);
    record.difficulty = static_cast<Difficulties>(sqlite3_column_int(statement, 6));

    sqlite3_finalize(statement);

    for (int index = 0; index < 81; index++)
    {
        int row = index / 9;
        int col = index % 9;

        numbers[row][col] = numbers_str[index].digitValue();
    }

    for (int index = 0; index < 81; index++)
    {
        int row = index / 9;
        int col = index % 9;

        solution[row][col] = solution_str[index].digitValue();
    }

    int row = 0;
    int col = 0;

    QList<int> cell;

    for (QChar ch : notes_str)
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

}

void GameInfo::save_records(const QList<QList<QList<Record>>>& records)
{
    clear_records();

    for (const auto& difficulty : records)
    {
        for (const auto& category : difficulty)
        {
            for (const auto& record : category)
                save_record(record);
        }
    }
}

void GameInfo::save_record(const Record& record)
{
    QString save_records_query("INSERT INTO RECORDS (ELAPSED_TIME, ERRORS, HINTS, DIFFICULTY) VALUES (?,?,?,?)");

    sqlite3_stmt * statement;

    int result = sqlite3_prepare_v2(database, save_records_query.toUtf8().constData(), -1, &statement, nullptr);

    if (result != SQLITE_OK)
        throw std::exception("Не вдалось підготувати збереження рекорду");

    sqlite3_bind_int(statement, 1, record.time);
    sqlite3_bind_int(statement, 2, record.errors);
    sqlite3_bind_int(statement, 3, record.hints);
    sqlite3_bind_int(statement, 4, record.difficulty);

    result = sqlite3_step(statement);

    sqlite3_finalize(statement);

    if (result != SQLITE_DONE) 
    {
        throw std::exception("Не вдалося виконати вираз для вставки рекорду");
    }

}

QList<Record> GameInfo::get_records()
{
    QString get_records_query("SELECT * FROM RECORDS");

    sqlite3_stmt * statement;

    QList<Record> records;

    int result = sqlite3_prepare_v2(database, get_records_query.toUtf8().constData(), -1, &statement, nullptr);
    
    if (result != SQLITE_OK)
        throw std::exception("Не вдалося підготувати запит для отримання рекордів");

    while (sqlite3_step(statement) == SQLITE_ROW) 
    {
        Record record;

        record.time = sqlite3_column_int(statement, 0);       
        record.errors = sqlite3_column_int(statement, 1);     
        record.hints = sqlite3_column_int(statement, 2);      
        record.difficulty = static_cast<Difficulties>(sqlite3_column_int(statement, 3)); 

        records.push_back(record);
    }

    sqlite3_finalize(statement);

    return records;
}

void GameInfo::clear_saved_game()
{
    QString clear_saved_game_query("DELETE FROM SAVED_GAME");

    char * error_message;
    int result = sqlite3_exec(database, clear_saved_game_query.toUtf8().constData(), nullptr, nullptr, &error_message);

    if (result != SQLITE_OK)
        throw std::exception(error_message);
}

bool GameInfo::has_saved_game()
{
    const char * query = "SELECT * FROM SAVED_GAME";

    sqlite3_stmt * statement;
    int result = sqlite3_prepare_v2(database, query, -1, &statement, nullptr);
    
    if (result != SQLITE_OK) 
    {
        throw std::runtime_error("Не вдалося підготувати запит для перевірки таблиці");
    }
    
    result = sqlite3_step(statement);
    sqlite3_finalize(statement);
    
    return (result == SQLITE_ROW);
}

bool GameInfo::has_settings()
{
     const char * query = "SELECT COUNT(*) FROM SETTINGS;";

    sqlite3_stmt * statement;

    int result = sqlite3_prepare_v2(database, query, -1, &statement, nullptr);

    if (result != SQLITE_OK) 
    {
        throw std::exception("Не вдалося підготувати запит для перевірки таблиці");
    }

    result = sqlite3_step(statement);

    int count = sqlite3_column_int(statement, 0);
    
    sqlite3_finalize(statement);

    return count != 0;
}


QPair<QList<QList<int>>,QList<QList<int>>> GameInfo::get_random_hard_sudoku_and_solution()
{
    int max_id = get_max_id();

    int random_index = QRandomGenerator::global()->bounded(max_id);

    QString get_sudoku_and_solution_query("SELECT SUDOKU, SOLUTION FROM HARD_SUDOKUS WHERE ID = " + QString::number(random_index) + ";");

    sqlite3_stmt * statement;

    int result = sqlite3_prepare_v2(database, get_sudoku_and_solution_query.toUtf8().constData(), -1, &statement, nullptr);

    if (result != SQLITE_OK)
        throw std::exception("Не вдалось підготувати запит отримання випадкового судоку та його рішення");

    sqlite3_step(statement);

    QString sudoku_str = QString::fromUtf8(reinterpret_cast<const char *>(sqlite3_column_text(statement, 0)));
    QString solution_str = QString::fromUtf8(reinterpret_cast<const char *>(sqlite3_column_text(statement, 1)));

    sqlite3_finalize(statement);

    auto sudoku = get_array_from_string(sudoku_str);
    auto solution = get_array_from_string(solution_str);

    return { solution, sudoku };
}

int GameInfo::get_max_id()
{
    QString get_range("SELECT MAX(ID) FROM HARD_SUDOKUS;");

    sqlite3_stmt * statement;

    int result = sqlite3_prepare_v2(database, get_range.toUtf8().constData(), -1 ,&statement, nullptr);

    if (result != SQLITE_OK)
    {
        throw std::exception("Не вдалось підготувати запит отримання максимального ID");
    }

    sqlite3_step(statement);

    int max_id = sqlite3_column_int(statement, 0);

    sqlite3_finalize(statement);

    return max_id;
}

QList<QList<int>> GameInfo::get_array_from_string(QString& string)
{
    QList<QList<int>> array(9, QList<int>(9,0));

    for (int index = 0; index < 81; index++)
    {
        int row = index / 9;
        int col = index % 9;

        array[row][col] = string[index].digitValue();
    }

    return array;
}

void GameInfo::clear_records()
{
    QString clear("DELETE FROM RECORDS;");

    int result = sqlite3_exec(database, clear.toUtf8().constData(), nullptr, nullptr, nullptr);

    if (result != SQLITE_OK)
        throw std::exception("Не вдалось очистити рекорди");
}