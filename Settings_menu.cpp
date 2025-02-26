#include "Settings_menu.h"

Settings_menu::Settings_menu(QWidget *parent)
	: QWidget(parent), fill_candidates(false), remove_candidates (true)
{
	auto * main_layout = new QVBoxLayout(this);
	

	auto * top_layout = new QHBoxLayout;

	auto * back_button = new QPushButton("Вийти");
	back_button->setStyleSheet("font-size: 24px");

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

	fill_candidates_button = new QPushButton("Заповнення кандидатів на початку гри - Вимкнено");
	fill_candidates_button->setStyleSheet("font-size: 18px;");

	remove_invalid_candidates_button = new QPushButton("Стирання кандидатів, що вже не підходять - Увімкнено");
	remove_invalid_candidates_button->setStyleSheet("font-size: 18px");

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

	connect(fill_candidates_button, &QPushButton::pressed, this, &Settings_menu::toggle_fill_candidates_button);
	connect(remove_invalid_candidates_button, &QPushButton::pressed, this, &Settings_menu::toggle_remove_candidates_button);


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

	load_settings();
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
	white.name = "white";
	white.background_color = "#ffffff";
	white.background = "Зображення/Білий фон.png";

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
	black.name = "black";
	black.background_color = "#2a2f42";
	black.background = "Зображення/Чорний фон.png";

	black.field = "white";
	black.field_border = "black";

	black.regular_text = "#aeb4d5";
	black.hovered_text = "#555555";
	black.inactive_text = "gray";

	black.regular_cell = "#2a2f42";
	black.wrong_cell = "#fd8989";
	black.highlighted_cell = "#1f212e";
	black.highlighted_active_cell = "#242f6d";
	black.highlighted_candidate = "#273376";



	Theme pink;
	pink.name = "pink";
	pink.background_color = "#ffe0f0";
	pink.background = "Зображення/Рожевий фон.png";

	pink.field = "#ffe0f0";
	pink.field_border = "#c64886";

	pink.regular_text = "#a34473";
	pink.hovered_text = "#805a6f";
	pink.inactive_text = "#8c687a";

	pink.regular_cell = "#ffe0f0";
	pink.wrong_cell = "#e02727";
	pink.highlighted_cell = "#ffbbde";
	pink.highlighted_active_cell = "#ff70c1";
	pink.highlighted_candidate = "#ffb3da";


	themes.push_back(white);
	themes.push_back(black);
	themes.push_back(pink);
}

Theme Settings_menu::get_current_theme()
{
	return current_theme;
}

void Settings_menu::toggle_fill_candidates_button()
{
	fill_candidates = !fill_candidates;

	if (fill_candidates)
		fill_candidates_button->setText("Заповнення кандидатів на початку гри - Увімкнено");
	else
		fill_candidates_button->setText("Заповнення кандидатів на початку гри - Вимкнено");
	
	emit fill_candidates_at_start(fill_candidates);
}

void Settings_menu::toggle_remove_candidates_button()
{
	remove_candidates = !remove_candidates;

	if (remove_candidates )
		remove_invalid_candidates_button->setText("Стирання кандидатів, що вже не підходять - Увімкнено");
	else
		remove_invalid_candidates_button->setText("Стирання кандидатів, що вже не підходять - Вимкнено");

	emit remove_invalid_candidates(remove_candidates);
}

void Settings_menu::load_settings()
{
	if (game_info.is_settings_file_empty())
	{
		current_theme = themes[0];
		change_theme();
		return;
	}

	QString theme_name;
	bool fill;
	bool remove;

	game_info.load_settings(fill, remove, theme_name);

	if (theme_name == "black")
	{
		emit theme_changed(themes[1]);
		current_theme = themes[1];
	}
	else if (theme_name == "pink")
	{
		emit theme_changed(themes[2]);
		current_theme = themes[2];
	}
	else
	{
		emit theme_changed(themes[0]);
		current_theme = themes[0];
	}

	change_theme();

	if (fill != fill_candidates)
		toggle_fill_candidates_button();

	if (remove != remove_candidates)
		toggle_remove_candidates_button();

}

void Settings_menu::save_settings()
{
	game_info.save_settings(fill_candidates, remove_candidates, current_theme.name);
}

void Settings_menu::change_theme()
{
	setStyleSheet("Settings_menu { background-image: url(" + current_theme.background + "); }");
}