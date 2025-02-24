#include "Main_menu.h"
#include <QVBoxLayout>
#include <QLabel>

Main_menu::Main_menu(QWidget* parent)
	: QWidget(parent)
{
	auto* main_menu_layout = new QVBoxLayout(this);

	game_name = new QLabel("Судоку");
	play_button = new QPushButton("Грати");
	continue_game_button = new QPushButton("Продовжити гру");
	personal_best_button = new QPushButton("Особисті рекорди");
	settings_button = new QPushButton("Налаштування");
	exit_button = new QPushButton("Вийти");

	connect(play_button, &QPushButton::pressed, [=]() { emit play_new_game(); });
	connect(continue_game_button, &QPushButton::pressed, [=]() { emit continue_game(); });
	connect(personal_best_button, &QPushButton::pressed, [=]() { emit show_personal_recods(); });
	connect(settings_button, &QPushButton::pressed, [=]() { emit show_settings_menu(); });
	connect(exit_button, &QPushButton::pressed, [=]() { emit close_program(); });


	game_name->setStyleSheet("QLabel { font-size: 96pt; }");


	main_menu_layout->addStretch(1);
	main_menu_layout->addWidget(game_name, 0, Qt::AlignHCenter);
	main_menu_layout->addStretch(1);
	main_menu_layout->addWidget(play_button, 0, Qt::AlignHCenter);
	main_menu_layout->addWidget(continue_game_button, 0,Qt::AlignHCenter);
	main_menu_layout->addWidget(personal_best_button, 0,Qt::AlignHCenter);
	main_menu_layout->addWidget(settings_button, 0,Qt::AlignHCenter);
	main_menu_layout->addWidget(exit_button, 0,Qt::AlignHCenter);
	main_menu_layout->addStretch(4);
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


void Main_menu::show_continue_option(bool show)
{
	if (show)
		continue_game_button->show();
	else
		continue_game_button->hide();
}

void Main_menu::change_background(QString& filename)
{
	setStyleSheet("Main_menu { background-image: url( " + filename + "); }");
}