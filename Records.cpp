#include "Records.h"
#include <QThread>
#include <algorithm>

Records::Records(QWidget* parent)
	: QWidget(parent)
{
	records = QList<QList<QList<Record>>>(4, QList<QList<Record>>(4));
	current_difficulty = hard;
	current_category = everything_included;
	hints_allowed = true;
	errors_allowed = true;

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




	
	Record record_hints_included		{ 5000000, 0, 1, hard };
	Record record_errors_included		{ 5000000, 1, 0, hard };
	Record record_everything_included	{ 3600000, 1, 1, hard };
	Record record_nothing_included		{ 4500000, 0, 0, hard };

	
	for (int i = 0; i < 10; i++)
	{
		get_new_record(record_hints_included);
		get_new_record(record_errors_included);
		get_new_record(record_everything_included);
		get_new_record(record_nothing_included);
	}

	change_category();

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


	int time_in_seconds = record.time / 1000;
	int time_seconds = time_in_seconds % 60;
	int time_minutes = time_in_seconds / 60 % 60;

	QString time_str = QString::number(time_minutes) + ":" + QString::number(time_seconds);


	if (time_in_seconds / 60 >= 60)
	{
		int time_hours = time_in_seconds / 60 / 60;

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

	for (auto& record : records[current_difficulty][current_category])
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


	if (category != everything_included)
	{
		if (records[record.difficulty][everything_included].size() < 10 || records[record.difficulty][everything_included].last().time > record.time)
		{
			records[record.difficulty][everything_included].push_back(record);

			std::sort(records[record.difficulty][everything_included].begin(), records[record.difficulty][everything_included].end());

			while (records[record.difficulty][everything_included].size() > 10)
				records[record.difficulty][everything_included].pop_back();
		}
	}


	if (category == current_category)
		change_current_records();

}

// Додати до кожної категорії найшвидші рекорди, які підходять під ту категорію. Гарантовано щонайбільше 10 рекордів на одну категорію.
void Records::load_records(QList<QList<QList<Record>>>& records_from_database)
{
	records = records_from_database;
}

void Records::set_difficulty(Difficulties difficulty)
{
	current_difficulty = difficulty;

	change_current_records();
}

const QList<QList<QList<Record>>>& Records::get_records() const
{
	return records;
}

