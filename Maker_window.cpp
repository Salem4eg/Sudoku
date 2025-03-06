#include "Maker_window.h"

#include <QFile>

Maker_window::Maker_window() : finished_threads(0), tries(1000), completed_numbers(40), time(0)
{
	resize(800, 600);

	auto * central_widget = new QWidget(this);

	setCentralWidget(central_widget);

	QVBoxLayout * layout = new QVBoxLayout(central_widget);

	sudoku_count_label = new QLabel("Кількість створених судоку: 0");
	time_label = new QLabel("Часу витрачено: 0 секунд");
	data_label = new QLabel();

	layout->addStretch(2);
	layout->addWidget(sudoku_count_label, 0, Qt::AlignCenter);
	layout->addWidget(data_label, 0, Qt::AlignCenter);
	layout->addWidget(time_label, 0, Qt::AlignCenter);
	layout->addStretch(4);
	sudoku_count_label->setFont(QFont("Comic Sans Ms", 24));

	show();

	timer = new QTimer(this);

	connect(timer, &QTimer::timeout, this, &Maker_window::change_time_label);

	timer->start(1000);

	required_difficulty = Difficulties::normal;

	generate_sudokus();
}

Maker_window::~Maker_window()
{

}

void Maker_window::generate_sudokus()
{
	change_data_label();

	for (int i = 0; i < 6; i++)
	{
		auto * thread = new QThread();
		threads.push_back(thread);


		Sudoku_maker * maker = new Sudoku_maker;

		maker->moveToThread(thread);

		connect(thread, &QThread::started, [=]()
			{
				if (required_difficulty == Difficulties::hard)
					maker->create_hard_sudoku(tries, completed_numbers);
				else if (required_difficulty == Difficulties::normal)
					maker->create_normal_sudoku(tries, completed_numbers);
				else if (required_difficulty == Difficulties::easy)
					maker->create_easy_sudoku(tries, completed_numbers);
			});
		connect(maker, &Sudoku_maker::finished, thread, &QThread::quit);
		connect(maker, &Sudoku_maker::finished, maker, &Sudoku_maker::deleteLater);
		connect(thread, &QThread::finished, thread, &QThread::deleteLater);

		if (required_difficulty == Difficulties::hard)
		{
			connect(maker, &Sudoku_maker::hard_sudoku_created, this, &Maker_window::add_new_sudoku);
			connect(maker, &Sudoku_maker::hard_sudoku_created, this, &Maker_window::change_count_label);
		}
		else if (required_difficulty == Difficulties::normal)
		{
			connect(maker, &Sudoku_maker::normal_sudoku_created, this, &Maker_window::add_new_sudoku);
			connect(maker, &Sudoku_maker::normal_sudoku_created, this, &Maker_window::change_count_label);
		}
		else if (required_difficulty == Difficulties::easy)
		{
			connect(maker, &Sudoku_maker::easy_sudoku_created, this, &Maker_window::add_new_sudoku);
			connect(maker, &Sudoku_maker::easy_sudoku_created, this, &Maker_window::change_count_label);
		}

		connect(maker, &Sudoku_maker::finished, [=]()
			{
				finished_threads++;
				if (finished_threads == threads.size())
				{
					finished_threads = 0;
					threads.clear();
					save_sudokus();

					completed_numbers--;
					if (completed_numbers > 28)
						generate_sudokus();
					else
						timer->stop();
				}
			});
	}

	for (auto thread : threads)
		thread->start();
}

void Maker_window::change_count_label()
{
	sudoku_count_label->setText("Кількість створених судоку: " + QString::number(sudokus_and_solutions.size()));
}

void Maker_window::change_time_label()
{
	time++;
	QString time_elapsed_str = "Часу витрачено: " + QString::number(time) + " секунд";
	time_label->setText(time_elapsed_str);
}

void Maker_window::change_data_label()
{
	if (required_difficulty == Difficulties::hard)
		data_label->setText("Зараз створюються складні судоку з " + QString::number(completed_numbers) + " числами на початку: ");
	else if (required_difficulty == Difficulties::normal)
		data_label->setText("Зараз створюються звичайні судоку з " + QString::number(completed_numbers) + " числами на початку: ");
	else if (required_difficulty == Difficulties::easy)
		data_label->setText("Зараз створюються легкі судоку з " + QString::number(completed_numbers) + " числами на початку: ");
}

void Maker_window::add_new_sudoku(QList<QList<int>> sudoku, QList<QList<int>> solution)
{
	sudokus_and_solutions.push_back({sudoku, solution});
}

void Maker_window::save_sudokus()
{
	QFile file;

	if (required_difficulty == Difficulties::hard)
		file.setFileName("hard_sudoku.txt");
	else if (required_difficulty == Difficulties::normal)
		file.setFileName("normal_sudoku.txt");
	else if (required_difficulty == Difficulties::easy)
		file.setFileName("easy_sudoku.txt");

	file.open(QIODevice::WriteOnly | QIODevice::Append);

	QTextStream out(&file);

	for (auto sudoku_and_solution : sudokus_and_solutions)
	{
		QString sudoku_str;
		QString solution_str;


		for (auto row : sudoku_and_solution.first)
		{
			for (int number : row)
			{
				sudoku_str += QString::number(number);
			}
		}

		for (auto row : sudoku_and_solution.second)
		{
			for (int number : row)
			{
				solution_str += QString::number(number);
			}
		}

		out << sudoku_str << " | " << solution_str <<"\n";
	}
	file.close();
}