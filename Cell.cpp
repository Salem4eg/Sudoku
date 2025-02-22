#include "Cell.h"
#include <QVBoxLayout>

Cell::Cell(QWidget* parent, int row, int col)
	: QPushButton(parent), notepad(9, false), coords(row,col), wrong_number(-1),highlighted_number(-1),finished_number(-1)
{
	int distance_to_border = 2;
	int distance_between_rectangles = 3;

	setFixedSize(70,70);

	for (int number = 0; number < 9; number++)
    {
        int row = number % 3;
        int col = number / 3;

        int x = row * 20 + distance_between_rectangles * row + distance_to_border;
        int y = col * 20 + distance_between_rectangles * col + distance_to_border;

		number_rectangles.emplace_back(x, y);
    }

	connect(this, &QPushButton::pressed, [=]()
		{
			emit set_active(this);
		});
}

Cell::~Cell()
{}

void Cell::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);

	switch (cell_state)
	{
	case Cell::Wrong:
		painter.setBrush(QBrush(current_theme.wrong_cell));
		break;
	case Cell::Highlighted:
		painter.setBrush(QBrush(current_theme.highlighted_cell));
		break;
	case Cell::Highlighted_active:
		painter.setBrush(QBrush(current_theme.highlighted_active_cell));
		break;
	default:
		painter.setBrush(QBrush(current_theme.regular_cell));
		break;
	}

	painter.setPen({current_theme.field_border, 1});

	painter.drawRect(0,0,70,70);

	painter.setPen(Qt::NoPen);


	// Підсвічення кандидата
	if (highlighted_number != -1)
	{
		painter.setBrush(QBrush(current_theme.highlighted_candidate));

		auto& coords = number_rectangles[highlighted_number];

		painter.drawRect(coords.x(), coords.y(), 20, 20);
	}

	painter.setPen(current_theme.regular_text);
	painter.setBrush(Qt::NoBrush);

	// Відмалювання не правильного числа
	if (is_wrong())
	{
		painter.setFont(QFont("Comic Sans Ms", 26));
		
		painter.drawText(0,0,70,70, Qt::AlignCenter, QString::number(wrong_number));
		return;
	}

	// Відмалювання кінцевого числа
	if (is_finished())
	{
		painter.setFont(QFont("Comic Sans Ms", 26));

		painter.drawText(0,0,70,70, Qt::AlignCenter, QString::number(finished_number));
		return;
	}

	painter.setFont(QFont("Comic Sans Ms", 12));
	// Відмалювання кандидатів
	for (int number = 0; number < 9; number++)
	{
		if (!notepad[number])
			continue;

		auto& coords = number_rectangles[number];

		painter.drawText(coords.x(), coords.y(), 20, 20, Qt::AlignCenter, QString::number(number + 1));
	}
}

void Cell::change_theme(Theme theme)
{
	current_theme = theme;
	update();
}


void Cell::add_candidate(int number)
{
	notepad[number - 1] = true;
	update();
}

void Cell::remove_candidate(int number)
{
	notepad[number - 1] = false;
	update();
}

void Cell::set_number(int number)
{
	finished_number = number;
	finished = true;

	update();
}

void Cell::toggle_wrong_state(int number)
{
	if (number == -1)
	{
		cell_state = Highlighted;
		wrong_number = -1;
	}
	else
	{
		cell_state = Wrong;
		wrong_number = number;
	}

	update();
}


void Cell::highlight_cell()
{
	cell_state = Highlighted;
	update();
}

void Cell::set_default_state()
{
	cell_state = Default;
	update();
}

void Cell::highlight_as_active_cell()
{
	cell_state = Highlighted_active;
	update();
}

void Cell::highlight_number(int number)
{
	if (number == -1)
	{
		highlighted_number = -1;
		update();

		return;
	}

	highlighted_number = number - 1;
	update();
}

bool Cell::is_finished()
{
	return finished;
}

bool Cell::is_wrong()
{
	return cell_state == Wrong;
}

bool Cell::is_candidate_noted(int number)
{
	return notepad[number - 1];
}

void Cell::clear()
{
	cell_state = Default;
	
	for (auto& candidate : notepad)
		candidate = false;

	finished = false;
	finished_number = -1;
	update();
}

int Cell::get_number()
{
	return finished_number;
}

QList<int> Cell::get_candidates()
{
	QList<int> result;

	for (int number = 0; number < 9; number++)
	{
		if (notepad[number])
			result.push_back(number + 1);
	}

	return result;
}


QPoint Cell::get_coords()
{
	return coords;
}