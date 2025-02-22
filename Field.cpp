#include "Field.h"
#include <QShortcut>

Field::Field(QWidget* parent)
	: QWidget(parent), notepad_mode(false), active_cell(nullptr), finished_cells(0), 
	placed_count(9,0), auto_fill_candidates_at_start(false), auto_remove_invalid_candidates(true), game_finished(false)
{
	auto* layout = new QVBoxLayout(this);

	auto * outline = new QWidget;

	outline->setStyleSheet("border: 2px solid black");

	auto * out_line_layout = new QVBoxLayout(outline);

	out_line_layout->setSpacing(0);
	out_line_layout->setContentsMargins(1,1,2,2);

	auto* field_widget = new QWidget;

	out_line_layout->addWidget(field_widget);

	auto* field_layout = new QGridLayout(field_widget);

	field_layout->setSpacing(0);
	field_layout->setContentsMargins(0, 0, 0, 0);

	QList<QGridLayout *> box_layouts;

	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			auto* box_widget = new QWidget;

			box_widget->setStyleSheet("border: 2px solid black;");

			connect(this, &Field::theme_changed, [=](Theme theme)
				{
					box_widget->setStyleSheet("border: 2px solid " + theme.field_border.name() + ";");
				});

			auto * box_layout = new QGridLayout(box_widget);
			
			box_layout->setSpacing(0);
			box_layout->setContentsMargins(2, 2, 2, 2);

			box_layouts.push_back(box_layout);
			field_layout->addWidget(box_widget, row, col);
		}
	}


	layout->addWidget(outline, 0, Qt::AlignCenter);
		
	for (int row = 0; row < 9; row++)
	{
		QList<Cell *> field_row;
		for (int col = 0; col < 9; col++)
		{
			auto* cell = new Cell(this, row, col);

			connect(cell, &Cell::set_active, this, &Field::set_active_cell);
			connect(this, &Field::theme_changed, cell, &Cell::change_theme);

			field_row.push_back(cell);
			box_layouts[(row / 3 * 3) + (col / 3)]->addWidget(cell, row % 3, col % 3);
		}
		field.push_back(field_row);
	}



	QShortcut * notepad_shortcut = new QShortcut(Qt::Key_N, parent);
	connect(notepad_shortcut, &QShortcut::activated, this, &Field::toggle_notepad_mode);
	connect(notepad_shortcut, &QShortcut::activated, this, [=]()
		{
			emit toggle_notepad();
		});
	

}

Field::~Field()
{}


void Field::set_active_cell(Cell* cell)
{
	if (active_cell)
	{
		set_cells_default_style();
	}

	active_cell = cell;

	set_cells_highlighting_style();
}

// Вимкнення підсвітки клітинок
void Field::set_cells_default_style()
{
	if (!active_cell)
		return;

	int cell_row = active_cell->get_coords().x();
	int cell_col = active_cell->get_coords().y();

	if (active_cell->is_finished())
	{
		int number = active_cell->get_number();

		unhighlight_number(number);
	}

	for (int row = 0; row < 9; row++)
	{
		if (row != cell_row)
			field[row][cell_col]->set_default_state();
	}

	for (int col = 0; col < 9; col++)
	{
		if (col != cell_col)
			field[cell_row][col]->set_default_state();
	}

	int box_row = cell_row / 3;
	int box_col = cell_col / 3;

	for (int row = box_row * 3; row < box_row * 3 + 3; row++)
	{
		for (int col = box_col * 3; col < box_col * 3 + 3; col++)
		{
			if (row != cell_row && col != cell_col)
				field[row][col]->set_default_state();
		}
	}

	active_cell->set_default_state();
}

// Підсвітка клітинок, які бачать активну клітинку
void Field::set_cells_highlighting_style()
{
	int cell_row = active_cell->get_coords().x();
	int cell_col = active_cell->get_coords().y();

	// По однаковому числу
	if (active_cell->is_finished() || highlighted_number != -1)
	{
		int number = active_cell->get_number();
		highlight_number(number);		
	}

	// Підсвітка клітинок на одному рядку з активною клітинкою
	for (int row = 0; row < 9; row++)
	{
		if (row != cell_row)
			field[row][cell_col]->highlight_cell();
	}

	// стовпчику
	for (int col = 0; col < 9; col++)
	{
		if (col != cell_col)
			field[cell_row][col]->highlight_cell();
	}

	int box_row = cell_row / 3;
	int box_col = cell_col / 3;

	// квадраті
	for (int row = box_row * 3; row < box_row * 3 + 3; row++)
	{
		for (int col = box_col * 3; col < box_col * 3 + 3; col++)
		{
			if (row != cell_row && col != cell_col)
				field[row][col]->highlight_cell();
		}
	}

	

	active_cell->highlight_as_active_cell();
}

void Field::fill_candidates()
{
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			if (field[row][col]->is_finished())
				continue;

			for (int number = 1; number <= 9; number++)
			{
				if (is_valid_candidate(number, row, col))
					field[row][col]->add_candidate(number);
			}
		}
	}
}

bool Field::is_valid_candidate(int number, int cell_row, int cell_col)
{
	for (int row = 0; row < 9; row++)
	{	
		if (row == cell_row)
			continue;

		if (field[row][cell_col]->get_number() == number)
			return false;
	}

	for (int col = 0; col < 9; col++)
	{	
		if (col == cell_col)
			continue;

		if (field[cell_row][col]->get_number() == number)
			return false;
	}

	int box_row = cell_row / 3;
	int box_col = cell_col / 3;

	for (int row_in_box = 0; row_in_box < 3; row_in_box++)
	{
		for (int col_in_box = 0; col_in_box < 3; col_in_box++)
		{
			int row = box_row * 3 + row_in_box;
			int col = box_col * 3 + col_in_box;

			if (row == cell_row && col == cell_col)
				continue;

			if (field[row][col]->get_number() == number)
				return false;
		}
	}

	return true;
}

void Field::keyPressEvent(QKeyEvent* event)
{
	if (active_cell->is_finished() || game_finished)
		return;

	
	// Обробка видалення не правильного рішення
	if (event->key() == Qt::Key::Key_Backspace || event->key() == Qt::Key::Key_Delete)
	{
		if (active_cell->is_wrong())
		{
			active_cell->toggle_wrong_state();
		}
		return;
	}

	// Ігноруємо натискання, що нам не потрібні
	if (event->key() < Qt::Key::Key_1 || event->key() > Qt::Key::Key_9)
		return;

	int number = event->key() - 48;

	if (active_cell->is_wrong())
	{
		active_cell->toggle_wrong_state();
		return;
	}

	if (notepad_mode)
	{
		if (!active_cell->is_candidate_noted(number))
		{
			int x = active_cell->get_coords().x();
			int y = active_cell->get_coords().y();

			if (is_valid_candidate(number,x,y))
			{
				active_cell->add_candidate(number);
				cells_by_candidate[number].push_back(active_cell);
			}
		}
		else
		{
			active_cell->remove_candidate(number);
			cells_by_candidate[number].removeOne(active_cell);
		}

		return;
	}

	if (is_correct_number(number, active_cell->get_coords()))
	{
		for (int candidate : active_cell->get_candidates())
			cells_by_candidate[candidate].removeOne(active_cell);

		active_cell->set_number(number);

		cells_by_number[number].push_back(active_cell);

		int row = active_cell->get_coords().x();
		int col = active_cell->get_coords().y();

		uncompleted_field[row][col] = number;

		if (auto_remove_invalid_candidates)
			clear_invalid_candidates();

		set_cells_highlighting_style();

		placed_count[number - 1]++;
		if (placed_count[number - 1] == 9)
			emit number_finished(number);

		finished_cells++;
		if (finished_cells == 81)
			emit finished_field();
	}
	else
	{
		int x = active_cell->get_coords().x();
		int y = active_cell->get_coords().y();

		active_cell->toggle_wrong_state(number);

		// Якщо гравець поставив число, яке вже є на рядку\стовпці\квадраті, а тобто не підходяще число, то помилка за неуважність не зараховується
		if (is_valid_candidate(number, x, y))
			emit wrong_number();
	}	
	
}

void Field::toggle_notepad_mode()
{
	notepad_mode = !notepad_mode;
}

bool Field::show_hint()
{
	if (active_cell->is_finished() || game_finished)
		return false;


	if (active_cell->is_wrong())
		active_cell->toggle_wrong_state();

	for (int candidate : active_cell->get_candidates())
		cells_by_candidate[candidate].removeOne(active_cell);

	int number = completed_field[active_cell->get_coords().x()][active_cell->get_coords().y()];

	active_cell->set_number(number);

	cells_by_number[number].push_back(active_cell);

	int row = active_cell->get_coords().x();
	int col = active_cell->get_coords().y();

	uncompleted_field[row][col] = number;

	if (auto_remove_invalid_candidates)
		clear_invalid_candidates();

	set_cells_highlighting_style();


	placed_count[number - 1]++;
	if (placed_count[number - 1] == 9)
		emit number_finished(number);

	finished_cells++;
	if (finished_cells == 81)
		emit finished_field();

	return true;
}

void Field::clear()
{
	for (int row = 0; row < 9; row++)
		for (int col = 0; col < 9; col++)
			field[row][col]->clear();

	cells_by_number.clear();
	cells_by_candidate.clear();

	for (int number = 0; number < 9; number++)
		placed_count[number] = 0;
}
void Field::set_numbers(QList<QList<int>> finished_field, QList<QList<int>> unfinished_field)
{
	clear();

	finished_cells = 0;
	
	completed_field = finished_field;
	uncompleted_field = unfinished_field;

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			if (unfinished_field[row][col] != 0)
			{
				int number = unfinished_field[row][col];

				field[row][col]->set_number(number);

				finished_cells++;

				placed_count[number - 1]++;
				if (placed_count[number - 1] == 9)
					emit number_finished(number);

				cells_by_number[number].push_back(field[row][col]);
			}
		}
	}

	if (auto_fill_candidates_at_start)
		fill_candidates();
}

void Field::unhighlight_number(int number)
{
	for (auto& cell : cells_by_number[number])
		cell->set_default_state();

	for (auto& cell : cells_by_candidate[number])
		cell->highlight_number(-1);
}

void Field::clear_invalid_candidates()
{
	int active_cell_row = active_cell->get_coords().x();
	int active_cell_col = active_cell->get_coords().y();

	int number = active_cell->get_number();

	for (int row = 0; row < 9; row++)
	{
		if (row == active_cell_row)
			continue;

		field[row][active_cell_col]->remove_candidate(number);

		cells_by_candidate[number].removeOne(field[row][active_cell_col]);
	}

	for (int col = 0; col < 9; col++)
	{
		if (col == active_cell_col)
			continue;

		field[active_cell_row][col]->remove_candidate(number);

		cells_by_candidate[number].removeOne(field[active_cell_row][col]);
	}

	int active_cell_box_row = active_cell_row / 3;
	int active_cell_box_col = active_cell_col / 3;

	for (int row_in_box = 0; row_in_box < 3; row_in_box++)
	{
		int row = active_cell_box_row * 3 + row_in_box;

		for (int col_in_box = 0; col_in_box < 3; col_in_box++)
		{
			int col = active_cell_box_col * 3 + col_in_box;

			if (row == active_cell_row && col == active_cell_col)
				continue;

			field[row][col]->remove_candidate(number);

			cells_by_candidate[number].removeOne(field[row][col]);
		}
	}
}

bool Field::is_correct_number(int number, QPoint coords)
{
	int row = coords.x();
	int col = coords.y();
	return completed_field[row][col] == number;
}

void Field::change_theme(Theme theme)
{
	setStyleSheet("Field { background-color: " + theme.field.name() + ";"
						    "border: 2px solid " + theme.field_border.name() + ";}");

	emit theme_changed(theme);
}

void Field::highlight_number(int number)
{
	unhighlight_number(highlighted_number);

	highlighted_number = number;

	for (auto& cell : cells_by_number[number])
			cell->highlight_cell();

	for (auto& cell : cells_by_candidate[number])
		cell->highlight_number(number);
}

void Field::fill_candidates_at_start(bool fill)
{
	auto_fill_candidates_at_start = fill;
}

void Field::remove_invalid_candidates(bool remove)
{
	auto_remove_invalid_candidates = remove;
}

void Field::lock_field(bool lock)
{
	game_finished = lock;
}

QList<QList<Cell *>> Field::get_cells()
{
	return field;
}

void Field::load_field(QList<QList<int>> numbers, QList<QList<QList<int>>> notes, QList<QList<int>> completed)
{
	clear();

	finished_cells = 0;
	uncompleted_field = numbers;
	completed_field = completed;

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			if (numbers[row][col] != 0)
			{
				int number = numbers[row][col];
				
				field[row][col]->set_number(number);

				finished_cells++;

				placed_count[number - 1]++;
				if (placed_count[number - 1] == 9)
					emit number_finished(number);

				cells_by_number[number].push_back(field[row][col]);
			}
			else
				for (int note : notes[row][col])
				{
					field[row][col]->add_candidate(note);

					cells_by_candidate[note].push_back(field[row][col]);
				}
		}
	}
}


QPair<QList<QList<int>>, QList<QList<QList<int>>>> Field::get_field()
{
	QList<QList<QList<int>>> notes {9, QList<QList<int>>(9, QList<int>(9, 0))};

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			notes[row][col] = field[row][col]->get_candidates();
		}
	}

	return { uncompleted_field, notes };
}

QList<QList<int>> Field::get_completed_field()
{
	return completed_field;
}