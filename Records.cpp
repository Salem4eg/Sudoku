#include "Records.h"
#include <QThread>
#include <algorithm>

Records::Records(QWidget* parent)
	: QWidget(parent), current_difficulty(hard), current_category(everything_included), hints_allowed(true), errors_allowed(true)
{

	records = QList<QList<QList<Record>>>(4, QList<QList<Record>>(4));

	auto* main_layout = new QVBoxLayout(this);

	auto* upper_part = new QWidget;
	upper_part->setStyleSheet("background-color: gray");
	auto* upper_part_layout = new QHBoxLayout(upper_part);

	auto* quit_button = new QPushButton("Вийти");
	auto* records_label = new QLabel("Особисті рекорди");
	auto* free_space = new QWidget;

	upper_part_layout->addWidget(quit_button, 1, Qt::AlignLeft);
	upper_part_layout->addWidget(records_label, 1, Qt::AlignCenter);
	upper_part_layout->addWidget(free_space, 1);

	auto* central_part = new QWidget;
	central_part->setStyleSheet("background-color: gray");
	auto* central_part_layout = new QHBoxLayout(central_part);

	auto* show_options = new QWidget;
	auto* show_options_layout = new QVBoxLayout(show_options);

	auto* change_difficulty = new QPushButton("Складність: складна");
	change_difficulty->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	auto* toggle_errors = new QPushButton("Без помилок");
	toggle_errors->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	auto* toggle_hints = new QPushButton("Без підказок");
	toggle_hints->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	auto* options_free_space = new QWidget;



	show_options_layout->addWidget(change_difficulty);
	show_options_layout->addWidget(toggle_errors);
	show_options_layout->addWidget(toggle_hints);
	show_options_layout->addWidget(options_free_space);

	show_options_layout->setContentsMargins(50, 120, 0, 0);
	show_options_layout->setSpacing(20);

	auto* records = new QWidget;
	records->setStyleSheet("background-color: white");
	auto* records_layout = new QVBoxLayout(records);

	auto* records_labels = new QWidget;
	auto* records_labels_layout = new QHBoxLayout(records_labels);

	auto* place = new QLabel("Місце");
	auto* time = new QLabel("Час");
	auto* errors = new QLabel("Помилки");
	auto* hints = new QLabel("Підказки");

	records_labels_layout->addWidget(place, 1, Qt::AlignHCenter);
	records_labels_layout->addWidget(time, 1, Qt::AlignHCenter);
	records_labels_layout->addWidget(errors, 1, Qt::AlignHCenter);
	records_labels_layout->addWidget(hints, 1, Qt::AlignHCenter);


	auto* records_value = new QWidget;

	records_value_layout = new QVBoxLayout(records_value);

	auto* records_free_space = new QWidget;

	records_layout->addWidget(records_labels);
	records_layout->addWidget(records_value);
	records_layout->addWidget(records_free_space, 2);

	auto* central_part_free_space = new QWidget;


	central_part_layout->addWidget(show_options, 2);
	central_part_layout->addWidget(records, 3);
	central_part_layout->addWidget(central_part_free_space, 2);


	main_layout->addWidget(upper_part, 1);
	main_layout->addWidget(central_part, 8);


	load_records();

	// Зробити зв'язок з кнопками, щоб без помилок, без підказок, складність були зв'язані з лямбда-функціями, які змінять складність/дозвіл, і використають change_current_records()
	// для дозволів додатково використати change_category() перед change_current_records()
	connect(quit_button, &QPushButton::pressed, this, &Records::leave);
	connect(toggle_errors, &QPushButton::pressed, [=]() 
		{ 
			errors_allowed = !errors_allowed;

			if (errors_allowed)
				toggle_errors->setText("З помилками"); 
			else
				toggle_errors->setText("Без помилок");

			change_category();
		});

	connect(toggle_hints, &QPushButton::pressed, [=]()
		{
			hints_allowed = !hints_allowed;

			if (hints_allowed)
				toggle_hints->setText("З підказками");
			else
				toggle_hints->setText("Без підказок");

			change_category();
		});


}

Records::~Records()
{}

// Відповідає за додавання рекорду до віджета
void Records::add_record(const Record& record)
{
	auto* record_widget = new QWidget;
	record_widget->setStyleSheet("background-color: red");
	record_widget->setFixedHeight(50);

	auto* record_layout = new QHBoxLayout(record_widget);


	int time_seconds = record.time % 60;
	int time_minutes = record.time / 60 % 60;

	QString time_str = QString::number(time_minutes) + ":" + QString::number(time_seconds);


	if (record.time / 60 >= 60)
	{
		int time_hours = record.time / 60 / 60;

		time_str = QString::number(time_hours) + ":" + time_str;
	}



	auto* time = new QLabel(time_str);
	auto* place = new QLabel(QString::number(records_value_layout->count() + 1));
	auto* errors = new QLabel(QString::number(record.errors));
	auto* hints = new QLabel(QString::number(record.hints));

	record_layout->addWidget(place, 0, Qt::AlignHCenter);
	record_layout->addWidget(time, 0, Qt::AlignHCenter);
	record_layout->addWidget(errors, 0, Qt::AlignHCenter);
	record_layout->addWidget(hints, 0, Qt::AlignHCenter);


	records_value_layout->addWidget(record_widget);
}

void Records::change_category()
{
	if (!hints_allowed && !errors_allowed)
		current_category = Categories::nothing_included;
	else if (!hints_allowed)
		current_category = Categories::only_errors_included;
	else if (!errors_allowed)
		current_category = Categories::only_hints_included;
	else
		current_category = Categories::everything_included;

	change_current_records();
}

void Records::change_current_records()
{
	clear_record_widgets();

	if (current_category != everything_included)
	{
		for (auto& record : records[current_difficulty][current_category])
			add_record(record);

		return;
	}

	// То є криндж
	QList<Record> all_records;

	for (auto& category : records[current_difficulty])
	{
		for (auto& record : category)
			all_records.push_back(record);
	}

	std::sort(all_records.begin(), all_records.end());

	for (auto& record : all_records)
		add_record(record);

}

void Records::clear_record_widgets()
{
	while (QLayoutItem* item = records_value_layout->takeAt(0)) 
	{
        if (item->widget())
            item->widget()->deleteLater();

        delete item;
    }
}


void Records::get_new_record(Record record)
{
	Categories category;

	if (record.errors == 0 && record.hints == 0)
		category = nothing_included;
	else if (record.errors > 0 && record.hints == 0)
		category = only_errors_included;
	else if (record.errors == 0 && record.hints > 0)
		category = only_hints_included;
	else
		category = everything_included;

	if (records[record.difficulty][category].size() < 10 || records[record.difficulty][category].last().time > record.time)
	{
		records[record.difficulty][category].push_back(record);

		std::sort(records[record.difficulty][category].begin(), records[record.difficulty][category].end());

		while (records[record.difficulty][category].size() > 10)
			records[record.difficulty][category].pop_back();
	}

	if (category == current_category || current_category == everything_included)
		change_current_records();

}

void Records::load_records()
{
	QList<Record> unfiltered_records = game_info.get_records();

	for (auto& record : unfiltered_records)
	{
		bool has_errors = record.errors > 0;
		bool used_hints = record.hints > 0;

		if (has_errors && !used_hints)
			records[record.difficulty][Categories::only_errors_included].push_back(record);
		else if (used_hints && !has_errors)
			records[record.difficulty][Categories::only_hints_included].push_back(record);
		else if (!used_hints && !has_errors)
			records[record.difficulty][Categories::nothing_included].push_back(record);
		else
			records[record.difficulty][Categories::everything_included].push_back(record);
	}
}

void Records::set_difficulty(Difficulties difficulty)
{
	current_difficulty = difficulty;

	change_current_records();
}

void Records::save_records()
{
	game_info.save_records(records);
}

