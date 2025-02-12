#include "Main_menu.h"
#include <QVBoxLayout>
#include <QLabel>

Main_menu::Main_menu(QWidget* parent)
	: QWidget(parent), text_color("#f4a6e0"), background_color("#ffffff")
{
	setStyleSheet("Main_menu { background-image: url(Зображення/Шпалери головного меню.png); }");

	auto* main_menu_layout = new QVBoxLayout(this);

	game_name = new QLabel("Судоку");
	play_button = new QPushButton("Грати");
	continue_game_button = new QPushButton("Продовжити гру");
	personal_best_button = new QPushButton("Особисті рекорди");
	appearance_button = new QPushButton("Кольора палітра");
	guide_button = new QPushButton("Посібник");
	exit_button = new QPushButton("Вийти");

	connect(play_button, &QPushButton::pressed, [=]() { emit play_new_game(); });
	connect(continue_game_button, &QPushButton::pressed, [=]() { emit continue_game(); });
	connect(personal_best_button, &QPushButton::pressed, [=]() { emit show_personal_recods(); });
	connect(appearance_button, &QPushButton::pressed, [=]() { emit show_appearance_menu(); });
	connect(guide_button, &QPushButton::pressed, [=]() { emit show_guide(); });
	connect(exit_button, &QPushButton::pressed, [=]() { emit close_program(); });

	change_buttons_style();

	game_name->setStyleSheet("QLabel { color: " + text_color.name() + ";  font-size: 96pt; }");

	game_name->setAlignment(Qt::AlignCenter);




	auto* game_name_layout = new QVBoxLayout;
	auto* buttons_layout = new QVBoxLayout;
	auto* space_layout = new QVBoxLayout;

	main_menu_layout->addWidget(game_name);

	buttons_layout->addWidget(play_button, 0, Qt::AlignHCenter);
	buttons_layout->addWidget(continue_game_button, 0,Qt::AlignHCenter);
	buttons_layout->addWidget(personal_best_button, 0,Qt::AlignHCenter);
	buttons_layout->addWidget(appearance_button, 0,Qt::AlignHCenter);
	buttons_layout->addWidget(guide_button, 0,Qt::AlignHCenter);
	buttons_layout->addWidget(exit_button, 0,Qt::AlignHCenter);

	auto* empty_widget = new QWidget;
	space_layout->addWidget(empty_widget);

	game_name_layout->setStretch(0, 1);
	buttons_layout->setStretch(1, 1);
	space_layout->setStretch(2, 1);

	main_menu_layout->addLayout(game_name_layout);
	main_menu_layout->addLayout(buttons_layout);
	main_menu_layout->addLayout(space_layout);


}

Main_menu::~Main_menu()
{}

void Main_menu::paintEvent(QPaintEvent*)
{
	QStyleOption opt;

	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Main_menu::set_button_style(QPushButton* button)
{
	/*button->setFlat(true);*/
}

void Main_menu::change_appearance()
{
	// Додати зміну стилю назви гри
	// Додати зміну заднього фону
	change_buttons_style();
}

void Main_menu::show_continue_option(bool show)
{
	if (show)
		continue_game_button->show();
	else
		continue_game_button->hide();
}

void Main_menu::change_buttons_style()
{
	set_button_style(play_button);
	set_button_style(continue_game_button);
	set_button_style(personal_best_button);
	set_button_style(appearance_button);
	set_button_style(guide_button);
	set_button_style(exit_button);
}

void Main_menu::change_main_menu_background(QString filename)
{
	setStyleSheet("Main_menu { background-image: url( " + filename + "); }");
}