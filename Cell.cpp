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
		painter.setBrush(QBrush(QColor("crimson")));
		break;
	case Cell::Highlighted:
		painter.setBrush(QBrush(QColor("darkgray")));
		break;
	default:
		painter.setBrush(QBrush(QColor("white")));
		break;
	}

	painter.setPen({Qt::black, 1});

	painter.drawRect(0,0,70,70);

	painter.setPen(Qt::NoPen);


	// Підсвічення кандидата
	if (highlighted_number != -1)
	{
		painter.setBrush(QBrush(QColor("cyan")));

		auto& coords = number_rectangles[highlighted_number];

		painter.drawRect(coords.x(), coords.y(), 20, 20);
	}

	painter.setPen(Qt::black);

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


void Cell::set_highlighted_state()
{
	cell_state = Highlighted;
	update();
}

void Cell::set_default_state()
{
	cell_state = Default;
	update();
}

void Cell::set_highlighted_number(int number)
{
	highlighted_number = number - 1;
	cell_state = Highlighted;
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