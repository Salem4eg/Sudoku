#include "Appearance_menu.h"


Appearance_menu::Appearance_menu(QList<Theme> saved_themes, int active_theme, QWidget* parent)
	: QWidget(parent), themes(saved_themes), active_theme_index(active_theme)
{
	

	auto* layout = new QVBoxLayout(this);

	auto* header = new QWidget;
	set_header(header);

	if (themes.isEmpty())
	{
		// Додати теми
		set_default_themes();
	}
	


	auto* themes_widget = new QWidget;
	set_themes(themes_widget);
	
	auto* options = new QWidget;
	set_theme_options(options);

	auto* general_options = new QWidget;
	set_general_options(general_options);

	auto * theme_control = new QWidget;
	set_theme_control(theme_control);

	layout->addWidget(header, 1);
	layout->addWidget(themes_widget, 1);
	layout->addWidget(options, 1);
	layout->addWidget(general_options, 1);
	layout->addWidget(theme_control, 1);
	
}

Appearance_menu::~Appearance_menu()
{}

void Appearance_menu::init()
{
	emit theme_changed(themes[active_theme_index]);
}

void Appearance_menu::set_header(QWidget * header)
{

	header->setStyleSheet("background-color: gray");
	auto* header_layout = new QHBoxLayout(header);

	auto* leave_button = new QPushButton("Вийти");
	auto* menu_name = new QLabel("Вигляд");

	header_layout->addWidget(leave_button, 1, Qt::AlignHCenter);
	header_layout->addWidget(menu_name, 6, Qt::AlignHCenter);
	header_layout->addStretch(1);

	connect(leave_button, &QPushButton::pressed, [=]()
		{
			emit leave();
		});
}


// Переробити віджет вигляду, має приймати масив тем

// 

// Теми зберігатимуться у масиві
// При додані теми створююється кнопка з відповідною назвою
// при натиску на кнопку вона виставляє тему з індексом цієї кнопки



void Appearance_menu::set_themes(QWidget * themes_widget)
{

	themes_widget->setStyleSheet("background-color: gray");
	auto* themes_layout = new QHBoxLayout(themes_widget);

	themes_layout->addStretch(1);
	for (int i = 0; i < themes.size(); i++)
	{
		auto * theme_button = new QPushButton(themes[i].name);
		themes_layout->addWidget(theme_button, 1, Qt::AlignHCenter);

		
		/*if (i == active_theme_index)
			theme_button->toggle();*/
	}
	themes_layout->addStretch(1);

}
void Appearance_menu::set_theme_options(QWidget * options)
{

	options->setStyleSheet("background-color: gray");
	auto* options_layout = new QHBoxLayout(options);

	// Колір тексту
	auto* text_options_widget = new QWidget;
	set_text_color_options(text_options_widget);

	//Кольори поля та рамок
	auto* field_options_widget = new QWidget;
	set_field_and_border_color_options(field_options_widget);

	// Задній фон
	auto* background_options_widget = new QWidget;
	set_background_options(background_options_widget);

	options_layout->addStretch(1);
	options_layout->addWidget(text_options_widget, 1, Qt::AlignHCenter);
	options_layout->addWidget(field_options_widget, 1, Qt::AlignHCenter);
	options_layout->addWidget(background_options_widget, 1, Qt::AlignHCenter);
	options_layout->addStretch(1);

}

// ЗАМІНИТИ НА АКТУАЛЬНІ КОЛЬОРИ ТЕМИ
void Appearance_menu::set_text_color_options(QWidget * text_options)
{
	Theme current_theme = themes[active_theme_index];

	auto* text_options_layout = new QVBoxLayout(text_options);

	// Колір звичайного тексту
	auto* regular_text_option = new QWidget;
	auto* regular_text_option_layout = new QHBoxLayout(regular_text_option);

	auto* regular_text_option_label = new QLabel("Текст");

	auto* regular_text_color_code = new QLineEdit;
	regular_text_color_code->setPlaceholderText(current_theme.regular_text.name());
	regular_text_color_code->setMaxLength(7);
	regular_text_color_code->setFixedWidth(50);

	auto* regular_text_show_color = new QLabel;
	regular_text_show_color->setStyleSheet("background-color: " + current_theme.regular_text.name());
	regular_text_show_color->setFixedSize(30, 30);

	regular_text_option_layout->addWidget(regular_text_option_label, 2, Qt::AlignRight);
	regular_text_option_layout->addWidget(regular_text_color_code, 1);
	regular_text_option_layout->addWidget(regular_text_show_color, 1);

	// Колір наведеного тексту
	auto* hovered_text_option = new QWidget;
	auto* hovered_text_option_layout = new QHBoxLayout(hovered_text_option);

	
	auto * hovered_text_option_label = new QLabel("Наведений текст");

	auto * hovered_text_color_code = new QLineEdit;
	hovered_text_color_code->setPlaceholderText(current_theme.hovered_text.name());
	hovered_text_color_code->setMaxLength(7);
	hovered_text_color_code->setFixedWidth(50);


	auto * hovered_text_show_color = new QLabel;
	hovered_text_show_color->setStyleSheet("background-color: " + current_theme.hovered_text.name());
	hovered_text_show_color->setFixedSize(30, 30);

	hovered_text_option_layout->addWidget(hovered_text_option_label, 2, Qt::AlignRight);
	hovered_text_option_layout->addWidget(hovered_text_color_code, 1);
	hovered_text_option_layout->addWidget(hovered_text_show_color, 1);


	text_options_layout->addStretch(1);
	text_options_layout->addWidget(regular_text_option, 0);
	text_options_layout->addWidget(hovered_text_option, 0);
	text_options_layout->addStretch(1);

	connect(regular_text_color_code, &QLineEdit::editingFinished, [=]()
	{
		if (QColor::isValidColorName(regular_text_color_code->text()))
		{
			regular_text_show_color->setStyleSheet("background-color: " + regular_text_color_code->text());

			themes[active_theme_index].regular_text = regular_text_color_code->text();
			emit theme_changed(themes[active_theme_index]);
		}
	});

	connect(hovered_text_color_code, &QLineEdit::editingFinished, [=]()
	{
		if (QColor::isValidColorName(hovered_text_color_code->text()))
		{
			hovered_text_show_color->setStyleSheet("background-color: " + hovered_text_color_code->text());

			themes[active_theme_index].hovered_text = hovered_text_color_code->text();
			emit theme_changed(themes[active_theme_index]);
		}
	});

}


void Appearance_menu::set_field_and_border_color_options(QWidget * field_and_border_options)
{
	Theme current_theme = themes[active_theme_index];

	auto* field_options_layout = new QVBoxLayout(field_and_border_options);


	auto* field_color_option = new QWidget;
	auto* field_color_option_layout = new QHBoxLayout(field_color_option);

	auto * field_color_label = new QLabel("Поле");

	auto * field_color_code = new QLineEdit;
	field_color_code->setPlaceholderText(current_theme.field.name());
	field_color_code->setMaxLength(7);
	field_color_code->setFixedWidth(50);

	auto * field_show_color = new QLabel;
	field_show_color->setStyleSheet("background-color: " + current_theme.field.name());
	field_show_color->setFixedSize(30,30);

	field_color_option_layout->addWidget(field_color_label, 2, Qt::AlignRight);
	field_color_option_layout->addWidget(field_color_code, 1);
	field_color_option_layout->addWidget(field_show_color, 1);





	auto* field_border_color_option = new QWidget;
	auto* field_border_color_option_layout = new QHBoxLayout(field_border_color_option);

	auto * field_border_color_label = new QLabel("Рамки поля");
	auto * field_border_color_code = new QLineEdit;
	field_border_color_code->setPlaceholderText(current_theme.field_border.name());
	field_border_color_code->setMaxLength(7);
	field_border_color_code->setFixedWidth(50);
	auto * field_border_show_color = new QLabel;
	field_border_show_color->setStyleSheet("background-color: " + current_theme.field_border.name());
	field_border_show_color->setFixedSize(30,30);

	field_border_color_option_layout->addWidget(field_border_color_label, 2, Qt::AlignRight);
	field_border_color_option_layout->addWidget(field_border_color_code, 1);
	field_border_color_option_layout->addWidget(field_border_show_color, 1);


	field_options_layout->addStretch(1);
	field_options_layout->addWidget(field_color_option);
	field_options_layout->addWidget(field_border_color_option);
	field_options_layout->addStretch(1);


	connect(field_color_code, &QLineEdit::editingFinished, [=]()
	{
		if (QColor::isValidColorName(field_color_code->text()))
		{
			field_show_color->setStyleSheet("background-color: " + field_color_code->text());

			themes[active_theme_index].field = field_color_code->text();
			emit theme_changed(themes[active_theme_index]);
		}
	});

	connect(field_border_color_code, &QLineEdit::editingFinished, [=]()
	{
		if (QColor::isValidColorName(field_border_color_code->text()))
		{
			field_border_show_color->setStyleSheet("background-color: " + field_border_color_code->text());

			themes[active_theme_index].field_border = field_border_color_code->text();
			emit theme_changed(themes[active_theme_index]);
		}
	});

}



void Appearance_menu::set_background_options(QWidget * background_options)
{
	Theme current_theme = themes[active_theme_index];

	auto* background_options_layout = new QVBoxLayout(background_options);

	// Колір заднього фону
	auto* background_color_option = new QWidget;
	auto* background_color_option_layout = new QHBoxLayout(background_color_option);


	auto * background_color_label = new QLabel("Задній фон");

	auto * background_color_code = new QLineEdit;
	background_color_code->setPlaceholderText(current_theme.background.name());
	background_color_code->setMaxLength(7);
	background_color_code->setFixedWidth(50);

	auto * background_show_color = new QLabel;
	background_show_color->setStyleSheet("background-color: " + current_theme.background.name());
	background_show_color->setFixedSize(30,30);

	background_color_option_layout->addWidget(background_color_label, 2, Qt::AlignRight);
	background_color_option_layout->addWidget(background_color_code, 1);
	background_color_option_layout->addWidget(background_show_color, 1);

	
	// Фон головного меню
	auto* main_menu_background_option = new QWidget;
	auto* main_menu_background_option_layout = new QHBoxLayout(main_menu_background_option);

	auto * main_menu_background_label = new QLabel("Фон головного меню");
	auto * main_menu_background_file = new QPushButton(current_theme.main_menu_background);

	main_menu_background_option_layout->addWidget(main_menu_background_label);
	main_menu_background_option_layout->addWidget(main_menu_background_file);

	background_options_layout->addStretch(1);
	background_options_layout->addWidget(background_color_option);
	background_options_layout->addWidget(main_menu_background_option);
	background_options_layout->addStretch(1);



	connect(main_menu_background_file, &QPushButton::pressed, [=]()
		{
			QString filename = QFileDialog::getOpenFileName(this, "Виберіть зображення фону головного меню", "", "Зображення (*.png *jpg)");

			if (!filename.isEmpty())
			{
				main_menu_background_file->setText(filename);
				themes[active_theme_index].main_menu_background = filename;
				emit theme_changed(themes[active_theme_index]);
			}
		});

	connect(background_color_code, &QLineEdit::editingFinished, [=]()
		{
			if (QColor::isValidColorName(background_color_code->text()))
			{
				background_show_color->setStyleSheet("background-color: " + background_color_code->text());
				themes[active_theme_index].background = background_color_code->text();
				emit theme_changed(themes[active_theme_index]);
			}

		});
}



void Appearance_menu::set_general_options(QWidget * general_options)
{

	general_options->setStyleSheet("background-color: gray");
	auto* general_options_layout = new QHBoxLayout(general_options);


	auto* text_size = new QWidget;
	auto* text_size_layout = new QVBoxLayout(text_size);

	auto* text_size_label = new QLabel("Розмір тексту");
	
	auto* text_size_options = new QWidget;
	auto* text_size_options_layout = new QHBoxLayout(text_size_options);

	auto * small_text = new QPushButton("Дрібний");
	auto * medium_text = new QPushButton("Середній");
	auto * big_text = new QPushButton("Великий");



	text_size_options_layout->addWidget(small_text);
	text_size_options_layout->addWidget(medium_text);
	text_size_options_layout->addWidget(big_text);

	text_size_layout->addStretch(1);
	text_size_layout->addWidget(text_size_label, 1, Qt::AlignHCenter);
	text_size_layout->addWidget(text_size_options, 1, Qt::AlignHCenter);
	text_size_layout->addStretch(1);
	

	general_options_layout->addStretch(1);
	general_options_layout->addWidget(text_size, 1, Qt::AlignHCenter);
	general_options_layout->addStretch(1);

	connect(small_text, &QPushButton::pressed, [=]()
		{
			emit text_size_changed(small);
		});

	connect(medium_text, &QPushButton::pressed, [=]()
		{
			emit text_size_changed(medium);
		});

	connect(big_text, &QPushButton::pressed, [=]()
		{
			emit text_size_changed(big);
		});

}

void Appearance_menu::set_theme_control(QWidget * theme_control)
{

	auto * layout = new QHBoxLayout(theme_control);


	
	auto * rename_current_theme = new QPushButton("Перейменувати тему");
	auto * add_theme = new QPushButton("Додати тему");
	auto * delete_current_theme = new QPushButton("Видалити тему");

	layout->addStretch(1);
	layout->addWidget(rename_current_theme, 1, Qt::AlignHCenter);
	layout->addWidget(add_theme, 1, Qt::AlignHCenter);
	layout->addWidget(delete_current_theme, 1, Qt::AlignHCenter);
	layout->addStretch(1);
}

void Appearance_menu::set_default_themes()
{
	Theme white;
	white.name = "Біла тема";
	white.background = "#ffffff";
	white.main_menu_background = "/Зображення/Шпалери головного меню.png";
	white.field = "#ffffff";
	white.field_border = "#ffffff";
	white.regular_text = "#000000";
	white.hovered_text = "#555555";
	

	Theme black;
	black.name = "Чорна тема";
	black.background = "#ffffff";
	black.main_menu_background = "/Зображення/Шпалери головного меню.png";
	black.field = "#ffffff";
	black.field_border = "#ffffff";
	black.regular_text = "#000000";
	black.hovered_text = "#555555";


	Theme pink;
	pink.name = "Рожева тема";
	pink.background = "#ffffff";
	pink.main_menu_background = "/Зображення/Шпалери головного меню.png";
	pink.field = "#ffffff";
	pink.field_border = "#ffffff";
	pink.regular_text = "#000000";
	pink.hovered_text = "#555555";

	themes.push_back(white);
	themes.push_back(black);
	themes.push_back(pink);
}

