#include "Settings_menu.h"

Settings_menu::Settings_menu(QWidget *parent)
	: QWidget(parent), fill_candidates(false), remove_candidates (true)
{
	setStyleSheet("Settings_menu { background-image: url(Зображення/Шпалери головного меню.png); }");
	auto * main_layout = new QVBoxLayout(this);
	

	auto * top_layout = new QHBoxLayout;

	auto * back_button = new QPushButton("Вийти");
	back_button->setStyleSheet("font-size: 24px");
	buttons.push_back(back_button);

	auto * settings_label = new QLabel("Налаштування");
	settings_label->setStyleSheet("font-size: 30px");
	labels.push_back(settings_label);

	top_layout->addWidget(back_button, 1, Qt::AlignHCenter);
	top_layout->addWidget(settings_label, 7, Qt::AlignHCenter);
	top_layout->addStretch(1);
	
	auto * center_layout = new QVBoxLayout;

	auto * help_label = new QLabel("Допомога по грі");
	help_label->setStyleSheet("font-size: 24px");
	labels.push_back(help_label);

	auto * fill_candidates_button = new QPushButton("Заповнення кандидатів на початку гри - Вимкнено");
	fill_candidates_button->setStyleSheet("font-size: 18px;");
	buttons.push_back(fill_candidates_button);

	auto * remove_invalid_candidates_button = new QPushButton("Стирання кандидатів, що вже не підходять - Увімкнено");
	remove_invalid_candidates_button->setStyleSheet("font-size: 18px");
	buttons.push_back(remove_invalid_candidates_button);

	fill_candidates_button->setFlat(true);
	remove_invalid_candidates_button->setFlat(true);
	
	auto * appearance_label = new QLabel("Вигляд гри");
	appearance_label->setStyleSheet("font-size: 24px");
	labels.push_back(appearance_label);

	auto * themes_widget = new QWidget;

	auto * themes_layout = new QHBoxLayout(themes_widget);

	auto * white_theme = new QPushButton("Біла тема");
	auto * black_theme = new QPushButton("Чорна тема");
	auto * pink_theme = new QPushButton("Рожева тема");
	white_theme->setStyleSheet("font-size: 18px");
	black_theme->setStyleSheet("font-size: 18px");
	pink_theme->setStyleSheet("font-size: 18px");

	buttons.push_back(white_theme);
	buttons.push_back(black_theme);
	buttons.push_back(pink_theme);

	center_layout->addWidget(help_label, 1, Qt::AlignHCenter);
	center_layout->addWidget(fill_candidates_button, 1, Qt::AlignHCenter);
	center_layout->addWidget(remove_invalid_candidates_button, 1, Qt::AlignHCenter);
	center_layout->addStretch(1);
	center_layout->addWidget(appearance_label, 1, Qt::AlignHCenter);
	center_layout->addWidget(themes_widget, 1, Qt::AlignHCenter);
	center_layout->addStretch(4);

	themes_layout->addWidget(white_theme, 1, Qt::AlignHCenter);
	themes_layout->addWidget(black_theme, 1, Qt::AlignHCenter);
	themes_layout->addWidget(pink_theme, 1, Qt::AlignHCenter);

	main_layout->addLayout(top_layout, 1);
	main_layout->addStretch(1);
	main_layout->addLayout(center_layout, 9);

	connect(fill_candidates_button, &QPushButton::pressed, this, [=]()
		{
			fill_candidates = !fill_candidates;

			if (fill_candidates)
				fill_candidates_button->setText("Заповнення кандидатів на початку гри - Увімкнено");
			else
				fill_candidates_button->setText("Заповнення кандидатів на початку гри - Вимкнено");
			
			emit fill_candidates_at_start(fill_candidates);
		});

	connect(remove_invalid_candidates_button, &QPushButton::pressed, this, [=]()
		{
			remove_candidates = !remove_candidates ;

			if (remove_candidates )
				remove_invalid_candidates_button->setText("Стирання кандидатів, що вже не підходять - Увімкнено");
			else
				remove_invalid_candidates_button->setText("Стирання кандидатів, що вже не підходять - Вимкнено");

			emit remove_invalid_candidates(remove_candidates);
		});


	connect(white_theme, &QPushButton::pressed, [=]()
		{
			emit theme_changed(themes[0]);
			current_theme = themes[0];
			change_theme();
		});

	connect(black_theme, &QPushButton::pressed, [=]()
		{
			emit theme_changed(themes[1]);
			current_theme = themes[1];
			change_theme();
		});

	connect(pink_theme, &QPushButton::pressed, [=]()
		{
			emit theme_changed(themes[2]);
			current_theme = themes[2];
			change_theme();
		});
	
	connect(back_button, &QPushButton::pressed, this, &Settings_menu::leave);

	create_themes();

	current_theme = themes[0];
}

Settings_menu::~Settings_menu()
{}

void Settings_menu::paintEvent(QPaintEvent * event)
{
	QStyleOption opt;

	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Settings_menu::create_themes()
{
	Theme white;
	white.background_color = "#ffffff";
	white.background = "Зображення/Шпалери головного меню.png";

	white.field = "#ffffff";
	white.field_border = "#000000";

	white.regular_text = "#000000";
	white.hovered_text = "#555555";
	white.inactive_text = "gray";

	white.regular_cell = "white";
	white.wrong_cell = "#fd8989";
	white.highlighted_cell = "#ebebeb";
	white.highlighted_active_cell = "#c8c8c8";
	white.highlighted_candidate = "#88ffff";



	Theme black;
	black.background_color = "#000000";
	black.background = "Зображення/Чорний фон.png";

	black.field = "white";
	black.field_border = "white";

	black.regular_text = "white";
	black.hovered_text = "#555555";
	black.inactive_text = "gray";

	black.regular_cell = "#171717";
	black.wrong_cell = "#fd8989";
	black.highlighted_cell = "#262424";
	black.highlighted_active_cell = "#c8c8c8";
	black.highlighted_candidate = "#88ffff";



	Theme pink;
	pink.background_color = "#ffffff";
	pink.background = "Зображення/Шпалери головного меню.png";

	pink.field = "#ffffff";
	pink.field_border = "#ffffff";

	pink.regular_text = "#000000";
	pink.hovered_text = "#555555";
	pink.inactive_text = "gray";

	themes.push_back(white);
	themes.push_back(black);
	themes.push_back(pink);
}

Theme Settings_menu::get_current_theme()
{
	return current_theme;
}

void Settings_menu::change_theme()
{
	setStyleSheet("Settings_menu { background-image: url(" + current_theme.background + "); }");
}