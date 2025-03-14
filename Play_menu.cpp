#include "Play_menu.h"

Play_menu::Play_menu(QWidget *parent)
	: QWidget(parent)
{
	setFont(QFont("Comic Sans Ms"));

	auto * play_menu_layout = new QVBoxLayout(this);

	// верхня частина екрану
	auto * upper_menu_part = new QWidget(this);
	auto * upper_menu_part_layout = new QHBoxLayout(upper_menu_part);


	// Тут кнопка переходу до головного меню
	auto * upper_menu_left_part_widget = new QWidget;
	auto * upper_menu_left_part_layout = new QHBoxLayout(upper_menu_left_part_widget);

	auto * back_button = new QPushButton("Вийти");
	back_button->setStyleSheet("font-size: 24px");
	upper_menu_left_part_layout->addWidget(back_button, 0, Qt::AlignHCenter);



	// Тут час, складність, помилки
	auto * upper_menu_center_part_widget = new QWidget;
	auto * upper_menu_center_part_layout = new QHBoxLayout(upper_menu_center_part_widget);



	difficulty_label = new QLabel("Складність: складна");
	time_label = new QLabel("Час: 0:00");
	errors_label = new QLabel("Помилок: 0 / 3");
	hints_label = new QLabel("Підказок: 0 / 3");



	upper_menu_center_part_layout->addStretch(3);
	upper_menu_center_part_layout->addWidget(difficulty_label, 1, Qt::AlignHCenter);
	upper_menu_center_part_layout->addWidget(time_label, 1, Qt::AlignHCenter);
	upper_menu_center_part_layout->addWidget(errors_label, 1, Qt::AlignHCenter);
	upper_menu_center_part_layout->addWidget(hints_label, 1, Qt::AlignHCenter);
	upper_menu_center_part_layout->addStretch(3);




	upper_menu_part_layout->addWidget(upper_menu_left_part_widget, 1);
	upper_menu_part_layout->addWidget(upper_menu_center_part_widget, 7);
	upper_menu_part_layout->addStretch(1);





	// центральна частина
	auto * center_menu_part = new QWidget;
	auto * center_menu_part_layout = new QVBoxLayout(center_menu_part);

	field = new Field(center_menu_part);


	auto * numbers_widget = new QWidget;
	numbers_widget->setFixedSize(650,70);

	auto * numbers_layout = new QHBoxLayout(numbers_widget);

	for (int number = 1; number <= 9; number++)
	{
		auto * number_button = new QPushButton(QString::number(number));

		number_button->setStyleSheet("font-size: 36px");

		connect(number_button, &QPushButton::pressed, [=]()
			{
				field->set_cells_default_style();
				field->highlight_number(number);
			});


		numbers_layout->addWidget(number_button, 0, Qt::AlignCenter);
		numbers_button.push_back(number_button);
	}


	center_menu_part_layout->addWidget(field, 0, Qt::AlignCenter);
	center_menu_part_layout->addWidget(numbers_widget, 0, Qt::AlignCenter);
	

	// нижня частина
	auto * lower_menu_part = new QWidget;
	auto * lower_menu_part_layout = new QHBoxLayout(lower_menu_part);

	auto * notepad_mode_button = new QPushButton("Блокнот вимкнено");
	auto * hint_button = new QPushButton("Підказки");


	lower_menu_part_layout->addWidget(notepad_mode_button, 0, Qt::AlignRight);
	lower_menu_part_layout->addWidget(hint_button, 0, Qt::AlignLeft);
	lower_menu_part_layout->setSpacing(150);


	play_menu_layout->addWidget(upper_menu_part);
	play_menu_layout->addWidget(center_menu_part);
	play_menu_layout->addWidget(lower_menu_part);



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


	connect(back_button, &QPushButton::pressed, [=]() 
		{ 
			if (has_unfinished_game)
				emit need_to_save_game(true);
			else
				emit need_to_save_game(false);

			emit leave();
		});

	connect(hint_button, &QPushButton::pressed, [=]()
		{
			if (hints >= 3)
				return;

			if (!field->show_hint())
				return;

			increase_hints();
		});

	connect(field, &Field::wrong_number, [=]()
		{
			increase_errors();

			if (errors > 3)
				game_lost();
		});

	connect(field, &Field::finished_field, [=]()
		{
			game_won();
		});

	timer = new QTimer(this);

	connect(timer, &QTimer::timeout, this, &Play_menu::increase_time);

	result_window = new Result(this);
	
	connect(result_window, &Result::back_to_menu, [=]() 
		{
			emit leave(); 
		});

	connect(result_window, &Result::play_again, [=]() { start_game(); });

	connect(field, &Field::number_finished, this, &Play_menu::set_inactive_number_button);
	connect(this, &Play_menu::game_finished, field, &Field::lock_field);

}

Play_menu::~Play_menu()
{}




void Play_menu::start_game(Difficulties game_difficulty)
{
	time_elapsed = 0;
	update_time_label();

	errors = 0;
	update_errors_label();

	hints = 0;
	update_hints_label();

	difficulty = game_difficulty;
	update_difficulty_label();

	if (game_difficulty == Difficulties::easy || game_difficulty == Difficulties::normal)
		fill_field(generate_sudoku());
	else if (game_difficulty == Difficulties::hard)
		emit request_new_hard_sudoku();

	timer->start(1000);

	notepad_mode = false;
	has_unfinished_game = true;

	set_default_number_button();

	emit game_finished(false);
	emit field_ready();
}

void Play_menu::continue_game()
{
	update_time_label();
	update_errors_label();
	update_hints_label();
	update_difficulty_label();

	timer->start(1000);

	notepad_mode = false;
	has_unfinished_game = true;

	emit game_finished(false);
	emit field_ready();
}

void Play_menu::increase_errors()
{
	errors++;

	update_errors_label();
}

void Play_menu::update_errors_label()
{
	errors_label->setText("Помилок: " + QString::number(errors) + " / 3");
}

void Play_menu::increase_time()
{
	time_elapsed++;

	update_time_label();
}

void Play_menu::update_time_label()
{
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

void Play_menu::increase_hints()
{
	hints++;

	update_hints_label();
}

void Play_menu::update_hints_label()
{
	hints_label->setText("Підказок: " + QString::number(hints) + " / 3");
}

void Play_menu::update_difficulty_label()
{
	switch (difficulty)
	{
	case Difficulties::easy:
		difficulty_label->setText("Складність: легка");
		break;
	case Difficulties::normal:
		difficulty_label->setText("Складність: звичайна");
		break;
	case Difficulties::hard:
		difficulty_label->setText("Складність: складна");
		break;
	case Difficulties::ultra_hard:
		difficulty_label->setText("Складність: надскладна");
		break;
	}
}

void Play_menu::game_lost()
{
	timer->stop();

	Record result(time_elapsed, errors, hints, difficulty);

	emit need_to_save_game(false);
	emit game_finished(true);
	
	has_unfinished_game = false;

	result_window->show_lost();
}

void Play_menu::game_won()
{
	timer->stop();

	Record result(time_elapsed, errors, hints, difficulty);

	emit new_record(result);
	emit need_to_save_game(false);
	emit game_finished(true);

	has_unfinished_game = false;

	result_window->show_won(result);
}

void Play_menu::fill_field(QPair<QList<QList<int>>, QList<QList<int>>> solution_and_sudoku)
{
	field->set_numbers(solution_and_sudoku.first, solution_and_sudoku.second);
}

QPair<QList<QList<int>>, QList<QList<int>>> Play_menu::generate_sudoku()
{
	Sudoku_generator generator;
	Solver solver;

	generator.generate();

	int completed_numbers;

	switch (difficulty)
	{
	case easy:
		completed_numbers = 34;
		break;
	case normal:
		completed_numbers = 31;
		break;
	default:
		break;
	}

	int tries = 0;
	do
	{
		if (tries > 1000)
			throw std::runtime_error("Too many tries to generate a sudoku");

		generator.createPuzzle(completed_numbers);

		solver.load_puzzle(generator.get_uncompleted_sudoku());

		if (difficulty == Difficulties::easy)
			solver.solve_with_easy_techniques();
		else if (difficulty == Difficulties::normal)
			solver.solve_with_normal_techniques();

		tries++;
	}while(!solver.isSolved());

	return { generator.get_completed_sudoku(), generator.get_uncompleted_sudoku() };
}

void Play_menu::change_theme(Theme theme)
{
	field->change_theme(theme);
	result_window->change_theme(theme);
}

void Play_menu::get_saved_game(QList<QList<int>>& numbers, QList<QList<QList<int>>>& notes, QList<QList<int>>& completed_field, Record& stats)
{
	// has_unfinished_game залежить від того, чи гравець вже пройшов судоку
	if (!has_unfinished_game)
		return;

	auto numbers_and_notes = field->get_field();
	completed_field = field->get_completed_field();

	numbers = numbers_and_notes.first;
	notes = numbers_and_notes.second;

	stats.time = time_elapsed;
	stats.errors = errors;
	stats.hints = hints;
	stats.difficulty = difficulty;
}

void Play_menu::fill_candidates_at_start(bool fill)
{
	field->toggle_fill_candidates_at_start(fill);
}

void Play_menu::remove_invalid_candidates(bool remove)
{
	field->remove_invalid_candidates(remove);
}


void Play_menu::load_game(QList<QList<int>> numbers, QList<QList<QList<int>>> notes, QList<QList<int>> completed_field, Record stats)
{
	set_default_number_button();

	field->load_field(numbers, notes, completed_field);

	time_elapsed = stats.time;
	errors = stats.errors;
	hints = stats.hints;
	difficulty = stats.difficulty;
}

void Play_menu::set_inactive_number_button(int number)
{
	numbers_button[number - 1]->setEnabled(false);	
}

void Play_menu::set_default_number_button()
{
	for (int number = 0; number < 9; number++)
	{
		numbers_button[number]->setEnabled(true);
	}
}