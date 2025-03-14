#include "GUI.h"

GUI::GUI(QWidget* parent)
	: QMainWindow(parent), text_color("#f4a6e0"), background_color("#ffffff"), text_size(medium)
{
	setGeometry(0, 0, 1920, 1000);
	QApplication::setFont(QFont("Comic Sans Ms"));

	auto* widget = new QWidget(parent);

	setCentralWidget(widget);

	layout = new QGridLayout(widget);

	menus = new QStackedWidget;

	layout->addWidget(menus);

	layout->setContentsMargins(0,0,0,0);


	personal_records = new Records;
	main_menu = new Main_menu;
	play_menu = new Play_menu;
	settings_menu = new Settings_menu;
	

	set_difficulty_dialog = new QDialog(this);

	set_difficulty_dialog->setWindowTitle("Оберіть складність");
	set_difficulty_dialog->resize(300, 100);

	auto* dialog_layout = new QHBoxLayout(set_difficulty_dialog);

	auto * easy = new QPushButton("Легка");
	auto * normal = new QPushButton("Звичайна");
	auto* hard = new QPushButton("Складна");

	dialog_layout->addWidget(easy);
	dialog_layout->addWidget(normal);
	dialog_layout->addWidget(hard);

	connect(easy, &QPushButton::pressed, [=]() 
		{ 
			play_menu->start_game(Difficulties::easy);
			set_difficulty_dialog->close();
		});

	connect(normal, &QPushButton::pressed, [=]() 
		{ 
			play_menu->start_game(Difficulties::normal); 
			set_difficulty_dialog->close();
		});

	connect(hard, &QPushButton::pressed, [=]() 
		{ 
			play_menu->start_game(Difficulties::hard); 
			set_difficulty_dialog->close();
		});


	connect(personal_records, &Records::leave, this, &GUI::show_main_menu);


	connect(main_menu, &Main_menu::play_new_game, this, &GUI::play_new_game);
	connect(main_menu, &Main_menu::continue_game, this, &GUI::continue_game);
	connect(main_menu, &Main_menu::show_personal_recods, this, &GUI::show_records_menu);
	connect(main_menu, &Main_menu::show_settings_menu, this, &GUI::show_settings_menu);
	connect(main_menu, &Main_menu::close_program, this, &GUI::close);


	connect(settings_menu, &Settings_menu::theme_changed, this, &GUI::change_theme);
	connect(settings_menu, &Settings_menu::fill_candidates_at_start, play_menu, &Play_menu::fill_candidates_at_start);
	connect(settings_menu, &Settings_menu::remove_invalid_candidates, play_menu, &Play_menu::remove_invalid_candidates);
	connect(settings_menu, &Settings_menu::leave, this, &GUI::show_main_menu);


	connect(play_menu, &Play_menu::game_finished, [=](bool game_finished)
		{
			 main_menu->show_continue_option(!game_finished);
		});
	connect(play_menu, &Play_menu::leave, this, &GUI::show_main_menu);
	// Це заставляє показувати поле для гри лише тоді, коли поле підготовлене
	connect(play_menu, &Play_menu::field_ready, this, [=](){ menus->setCurrentIndex(1); });
	connect(play_menu, &Play_menu::new_record, personal_records, &Records::get_new_record);
	connect(play_menu, &Play_menu::request_new_hard_sudoku, [=]()
		{
			play_menu->fill_field(game_info.get_random_hard_sudoku_and_solution());
		});
	connect(play_menu, &Play_menu::need_to_save_game, [=](bool need_to_save)
		{
			has_saved_game = need_to_save;
		});


	if (game_info.has_saved_game())
	{
		main_menu->show_continue_option(true);

		QList<QList<int>> numbers(9, QList<int>(9,0));
		QList<QList<QList<int>>> notes(9, QList<QList<int>>(9, QList<int>(9, 0)));
		QList<QList<int>> solution(9, QList<int>(9,0));
		Record record;

		game_info.load_game(numbers, notes, solution, record);
		play_menu->load_game(numbers, notes, solution, record);
	}
	else
		main_menu->show_continue_option(false);
	
	if (game_info.has_settings())
		settings_menu->load_settings(game_info.get_settings());
	else
		settings_menu->set_default_theme();

	personal_records->load_records(game_info.get_records());

	menus->addWidget(main_menu);
	menus->addWidget(play_menu);
	menus->addWidget(personal_records);
	menus->addWidget(settings_menu);
	
	
	change_theme(settings_menu->get_current_theme());
	showMaximized();
	
	show_main_menu();
}

GUI::~GUI()
{}

#include <QMessageBox>

void GUI::closeEvent(QCloseEvent * event)
{
	try
	{
		QList<QList<int>> numbers;
		QList<QList<QList<int>>> notes;
		QList<QList<int>> completed_field;
		Record stats;

		if (has_saved_game)
		{
			play_menu->get_saved_game(numbers, notes, completed_field, stats);
			game_info.save_game(numbers, notes, completed_field, stats);
		}

		game_info.save_records(personal_records->get_records());

		game_info.save_settings(settings_menu->get_settings());

		event->accept();
	}
	catch (std::exception error)
	{
	    QMessageBox box;
	    box.information(nullptr, "Помилка", error.what());
	}
	
}

void GUI::show_main_menu()
{
	menus->setCurrentIndex(0);
}

void GUI::play_new_game()
{
	set_difficulty_dialog->exec();
}

void GUI::continue_game()
{
	play_menu->continue_game();
}


void GUI::show_records_menu()
{
	menus->setCurrentIndex(2);
}

void GUI::show_settings_menu()
{
	menus->setCurrentIndex(3);
}

void GUI::change_theme(Theme theme)
{
	
	setStyleSheet("QPushButton:pressed { background-color: none; border: none; }"

				  "QPushButton:hover { color: " + theme.hovered_text.name() + "; }"
					
				  "QPushButton:disabled { color: " + theme.inactive_text.name() + "; }"

				  "QPushButton { border: none; "
								 "color: " + theme.regular_text.name() + ";"
				               " font-size: " + QString::number(text_size) + "px; }"

				  "QLabel { color: " + theme.regular_text.name() + ";"
				               " font-size: " + QString::number(text_size) + "px; }"
				  "GUI { background-color: " + theme.background_color.name() + "}");

	

	main_menu->change_background(theme.background);
	personal_records->change_background(theme.background);
	play_menu->change_theme(theme);
	set_difficulty_dialog->setStyleSheet("background-color: " + theme.background_color.name() + ";");

	QIcon icon;
	if (theme.name == "white")
		icon.addFile("Зображення/Біла іконка.png");
	else if (theme.name == "black")
		icon.addFile("Зображення/Чорна іконка.png");
	else if (theme.name == "pink")
		icon.addFile("Зображення/Рожева іконка.png");

	setWindowIcon(icon);

	current_theme = theme;
}
