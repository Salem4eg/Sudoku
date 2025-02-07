#include "Play_menu.h"

Play_menu::Play_menu(QWidget *parent)
	: QWidget(parent)
{
	auto * play_menu_layout = new QVBoxLayout(this);




	// верхня частина екрану
	auto * upper_menu_part = new QWidget;

	auto * upper_menu_part_layout = new QHBoxLayout(upper_menu_part);


	// Тут кнопка переходу до головного меню
	auto * upper_menu_left_part_widget = new QWidget(upper_menu_part);
	upper_menu_left_part_widget->setStyleSheet("background-color: yellow");

	auto * upper_menu_left_part_layout = new QHBoxLayout(upper_menu_left_part_widget);

	auto * back_button = new QPushButton("Вийти", upper_menu_left_part_widget);
	upper_menu_left_part_layout->addWidget(back_button, 0, Qt::AlignLeft);



	// Тут час, складність, помилки
	auto * upper_menu_center_part_widget = new QWidget(upper_menu_part);
	upper_menu_center_part_widget->setStyleSheet("background-color: yellow");

	auto * upper_menu_center_part_layout = new QHBoxLayout(upper_menu_center_part_widget);

	difficulty_label = new QLabel("Складність: складна", upper_menu_center_part_widget);
	difficulty_label->setStyleSheet("background-color: white");


	time_label = new QLabel("Час: 0:00", upper_menu_center_part_widget);
	errors_label = new QLabel("Помилок: 0 / 3", upper_menu_center_part_widget);
	hints_label = new QLabel("Підказок: 0 / 3", upper_menu_center_part_widget);

	time_label->setStyleSheet("background-color: white");
	errors_label->setStyleSheet("background-color: white");
	hints_label->setStyleSheet("background-color: white");

	upper_menu_center_part_layout->addWidget(difficulty_label, 5, Qt::AlignRight);
	upper_menu_center_part_layout->addWidget(time_label, 1, Qt::AlignCenter);
	upper_menu_center_part_layout->addWidget(errors_label, 1, Qt::AlignLeft);
	upper_menu_center_part_layout->addWidget(hints_label, 4, Qt::AlignLeft);


	// Це пустота
	auto * upper_menu_right_part_widget = new QWidget(upper_menu_part);
	upper_menu_right_part_widget->setStyleSheet("background-color: yellow");


	upper_menu_part_layout->addWidget(upper_menu_left_part_widget, 1);
	upper_menu_part_layout->addWidget(upper_menu_center_part_widget, 7);
	upper_menu_part_layout->addWidget(upper_menu_right_part_widget, 1);








	// центральна частина
	auto * center_menu_part = new QWidget;
	center_menu_part->setStyleSheet("background-color: gray");

	auto * center_menu_part_layout = new QHBoxLayout(center_menu_part);

	field = new Field(center_menu_part);
	center_menu_part_layout->addWidget(field, 0, Qt::AlignCenter);

	

	// нижня частина
	auto * lower_menu_part = new QWidget;
	lower_menu_part->setStyleSheet("background-color: gray");

	auto * lower_menu_part_layout = new QHBoxLayout(lower_menu_part);

	auto * notepad_mode_button = new QPushButton("Блокнот вимкнено", lower_menu_part);
	auto * hint_button = new QPushButton("Підказки", lower_menu_part);

	lower_menu_part_layout->addWidget(notepad_mode_button, 0, Qt::AlignRight);
	lower_menu_part_layout->addWidget(hint_button, 0, Qt::AlignLeft);
	lower_menu_part_layout->setSpacing(150);


	play_menu_layout->addWidget(upper_menu_part);
	play_menu_layout->addWidget(center_menu_part);
	play_menu_layout->addWidget(lower_menu_part);


	// тут зв'язки.
	connect(notepad_mode_button, &QPushButton::pressed, [=]()
		{
			field->toggle_notepad_mode();

			notepad_mode = !notepad_mode;

			if (notepad_mode)
				notepad_mode_button->setText("Блокнот увімкнено");
			else
				notepad_mode_button->setText("Блокнот вимкнено");
		});

	connect(field, &Field::toggle_notepad, [=]()
		{
			notepad_mode = !notepad_mode;

			if (notepad_mode)
				notepad_mode_button->setText("Блокнот увімкнено");
			else
				notepad_mode_button->setText("Блокнот вимкнено");
		});

	connect(back_button, &QPushButton::pressed, [=]() { emit leave(); });

	connect(hint_button, &QPushButton::pressed, [=]()
		{
			if (hints >= 3)
				return;

			if (!field->show_hint())
				return;

			hints++;

			hints_label->setText("Підказок: " + QString::number(hints) + " / 3");
		});

	connect(field, &Field::wrong_number, [=]()
		{
			update_errors();

			if (errors > 3)
				game_lost();
		});

	connect(field, &Field::finished_field, [=]()
		{
			game_won();
		});

	timer = new QTimer(this);

	connect(timer, &QTimer::timeout, this, &Play_menu::update_time);

	result_window = new Result(this);
	
	connect(result_window, &Result::back_to_menu, [=]() { emit leave(); });

	connect(result_window, &Result::play_again, [=]() { start_game(); });

}

Play_menu::~Play_menu()
{}




void Play_menu::start_game(Difficulties game_difficulty)
{
	fill_field();

	time_elapsed = 0;
	time_label->setText("Час: 00:00");

	errors = 0;
	errors_label->setText("Помилок: 0 / 3");

	hints = 0;
	hints_label->setText("Підказок: 0 / 3");

	difficulty = game_difficulty;

	switch (game_difficulty)
	{
	case Difficulties::easy:
		difficulty_label->setText("Складність: легка");
		break;
	case Difficulties::normal:
		difficulty_label->setText("Складність: нормальна");
		break;
	case Difficulties::hard:
		difficulty_label->setText("Складність: складна");
		break;
	case Difficulties::ultra_hard:
		difficulty_label->setText("Складність: надскладна");
		break;
	}

	timer->start(1000);

	emit field_ready();

	notepad_mode = false;
}

void Play_menu::update_errors()
{
	errors++;

	errors_label->setText("Помилок: " + QString::number(errors) + " / 3");
}

void Play_menu::update_time()
{
	time_elapsed++;

	QString time;
	// Більше години мучається
	if (time_elapsed >= 3600)
	{
		QString seconds = QString::number(time_elapsed % 60);
		QString minutes = QString::number((time_elapsed / 60) % 60);
		QString hours = QString::number(time_elapsed / 60 / 60);

		// 6 -> 06
		if (time_elapsed % 60 < 10)
			seconds = "0" + seconds;

		if ((time_elapsed / 60) % 60 < 10)
			minutes = "0" + minutes;

		time = hours + ":" + minutes + ":" + seconds;
	}
	else
	{
		QString minutes = QString::number(time_elapsed / 60);
		QString seconds = QString::number(time_elapsed % 60);

		
		if (time_elapsed % 60 < 10)
			seconds = "0" + seconds;

		if (time_elapsed / 60 < 10)
			minutes = "0" + minutes;

		time = minutes + ":" + seconds;
	}

	time_label->setText("Час: " + time);
}

void Play_menu::game_lost()
{
	timer->stop();

	Record result(time_elapsed, errors, hints, difficulty);

	save_record(result);

	result_window->show_lost();
}

void Play_menu::game_won()
{
	timer->stop();

	Record result(time_elapsed, errors, hints, difficulty);

	save_record(result);

	result_window->show_won(result);
}

void Play_menu::fill_field()
{
	Sudoku_generator generator;
	Solver solver;

	generator.generate();

	int tries = 0;
	do
	{
		if (tries > 250)
			break;

		generator.createPuzzle(25);
		solver.load_puzzle(generator.get_uncompleted_sudoku());
		solver.solve();
	}while(!solver.isSolved());

	field->set_numbers(generator.get_completed_sudoku(), generator.get_uncompleted_sudoku());
}

void Play_menu::change_field_colors(QColor field_color, QColor field_border)
{
	field->change_field_theme(field_color, field_border);
}

void Play_menu::save_record(Record& record)
{
	// Зберігання рекорду
}