#include "Field.h"
#include <QShortcut>

Field::Field(QWidget* parent)
	: QWidget(parent), notepad_mode(false), active_cell(nullptr), finished_cells(0)
{
	setStyleSheet("background-color: gray");

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
	active_cell->set_default_state();
	int cell_row = active_cell->get_coords().x();
	int cell_col = active_cell->get_coords().y();

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
}

// Підсвітка клітинок, які бачать активну клітинку
void Field::set_cells_highlighting_style()
{
	active_cell->set_highlighted_state();
	int cell_row = active_cell->get_coords().x();
	int cell_col = active_cell->get_coords().y();

	for (int row = 0; row < 9; row++)
	{
		if (row != cell_row)
			field[row][cell_col]->set_highlighted_state();
	}

	for (int col = 0; col < 9; col++)
	{
		if (col != cell_col)
			field[cell_row][col]->set_highlighted_state();
	}

	int box_row = cell_row / 3;
	int box_col = cell_col / 3;

	for (int row = box_row * 3; row < box_row * 3 + 3; row++)
	{
		for (int col = box_col * 3; col < box_col * 3 + 3; col++)
		{
			if (row != cell_row && col != cell_col)
				field[row][col]->set_highlighted_state();
		}
	}
}

// Потрібно 4 види підсвітки клітинки:
// Підсвітка активної клітинки
// Підсвітка клітинок, що бачать активну клітинку
// Підсвітка клітинок, що мають вибране кінцеве число
// Підсвітка кандидатів вибраного числа


void Field::keyPressEvent(QKeyEvent* event)
{
	if (active_cell->is_finished())
		return;

	
	// Обробка натискання чисел
	if (event->key() >= Qt::Key::Key_1 && event->key() <= Qt::Key::Key_9)
	{
		int number = event->key() - 48;

		if (active_cell->is_wrong())
		{
			active_cell->toggle_wrong_state();
			return;
		}

		if (notepad_mode)
		{
			if (!active_cell->is_candidate_noted(number))
				active_cell->add_candidate(number);
			else
				active_cell->remove_candidate(number);

			return;
		}

		if (is_correct_number(number, active_cell->get_coords()))
		{
			active_cell->set_number(number);
			
			finished_cells++;
			if (finished_cells == 81)
				emit finished_field();
		}
		else
		{
			active_cell->toggle_wrong_state(number);
			emit wrong_number();
		}
		
	}

	// Обробка видалення не правильного рішення
	if (event->key() == Qt::Key::Key_Backspace || event->key() == Qt::Key::Key_Delete)
	{
		if (active_cell->is_wrong())
		{
			active_cell->toggle_wrong_state();
		}
	}
}

void Field::toggle_notepad_mode()
{
	notepad_mode = !notepad_mode;
}

bool Field::show_hint()
{
	if (active_cell->is_finished())
		return false;


	if (active_cell->is_wrong())
		active_cell->toggle_wrong_state();

	active_cell->set_number(completed_field[active_cell->get_coords().x()][active_cell->get_coords().y()]);

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
				field[row][col]->set_number(unfinished_field[row][col]);
				finished_cells++;
			}
		}
	}
}

bool Field::is_correct_number(int number, QPoint coords)
{
	int row = coords.x();
	int col = coords.y();
	return completed_field[row][col] == number;
}

void Field::change_field_theme(QColor field, QColor field_border)
{
	setStyleSheet("QWigdet { background-color: " + field.name() + ";"
						    "border: 2px solid " + field_border.name() + ";}");
}

QList<QList<Cell *>> Field::get_cells()
{
	return field;
}

void Field::load_field(QList<QList<int>> numbers, QList<QList<QList<int>>> notes)
{
	clear();

	finished_cells = 0;

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			if (numbers[row][col] != 0)
			{
				field[row][col]->set_number(numbers[row][col]);
				finished_cells++;
			}
			else
				for (int note : notes[row][col])
					field[row][col]->add_candidate(note);
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