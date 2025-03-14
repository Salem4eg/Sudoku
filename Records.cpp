#include "Records.h"
#include <QThread>
#include <algorithm>

Records::Records(QWidget* parent)
	: QWidget(parent), current_difficulty(hard), current_category(everything_included), hints_allowed(false), errors_allowed(false)
{
	setFont(QFont("Comic Sans Ms"));
	records = QList<QList<QList<Record>>>(4, QList<QList<Record>>(4));

	auto* main_layout = new QVBoxLayout(this);

	auto* upper_part = new QWidget;
	auto* upper_part_layout = new QHBoxLayout(upper_part);

	auto* quit_button = new QPushButton("Вийти");
	quit_button->setStyleSheet("font-size: 24px");
	auto* records_label = new QLabel("Особисті рекорди");
	records_label->setStyleSheet("font-size: 30px");
	auto* free_space = new QWidget;

	upper_part_layout->addWidget(quit_button, 1, Qt::AlignHCenter);
	upper_part_layout->addWidget(records_label, 7, Qt::AlignHCenter);
	upper_part_layout->addWidget(free_space, 1);

	auto* central_part = new QWidget;
	auto* central_part_layout = new QHBoxLayout(central_part);

	auto* show_options = new QWidget;
	auto* show_options_layout = new QVBoxLayout(show_options);

	auto* change_difficulty = new QPushButton("Складність: складна");
	auto* toggle_errors = new QPushButton("Без помилок");
	auto* toggle_hints = new QPushButton("Без підказок");
	auto* options_free_space = new QWidget;


	show_options_layout->addWidget(change_difficulty, 1, Qt::AlignLeft);
	show_options_layout->addWidget(toggle_errors, 1, Qt::AlignLeft);
	show_options_layout->addWidget(toggle_hints, 1, Qt::AlignLeft);
	show_options_layout->addWidget(options_free_space);

	show_options_layout->setContentsMargins(50, 120, 0, 0);
	show_options_layout->setSpacing(20);

	auto* records = new QWidget;
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


	central_part_layout->addSpacing(20);
	central_part_layout->addWidget(show_options, 2);
	central_part_layout->addWidget(records, 4);
	central_part_layout->addWidget(central_part_free_space, 2);


	main_layout->addWidget(upper_part, 1);
	main_layout->addWidget(central_part, 8);

	change_category();

	connect(quit_button, &QPushButton::pressed, this, &Records::leave);

	connect(change_difficulty, &QPushButton::pressed, [=]()
		{
			int difficulty = current_difficulty;
			difficulty++;

			if (difficulty > Difficulties::hard)
				difficulty = 0;

			current_difficulty = static_cast<Difficulties>(difficulty);

			switch (current_difficulty)
			{
			case easy:
				change_difficulty->setText("Складність: легка");
				break;
			case normal:
				change_difficulty->setText("Складність: звичайна");
				break;
			case hard:
				change_difficulty->setText("Складність: складна");
				break;
			case ultra_hard:
				change_difficulty->setText("Складність: надскладна");
				break;
			default:
				break;
			}

			change_current_records();
		});

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

void Records::paintEvent(QPaintEvent * event)
{
	QStyleOption opt;

	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// Відповідає за додавання рекорду до віджета
void Records::add_record(const Record& record)
{
	auto* record_widget = new QWidget;
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

	if (current_category == nothing_included)
	{
		for (auto& record : records[current_difficulty][current_category])
			add_record(record);

		return;
	}

	if (current_category != everything_included)
	{
		QList<Record> all_records;

		for (auto& record : records[current_difficulty][current_category])
			all_records.push_back(record);

		for (auto& record : records[current_difficulty][nothing_included])
			all_records.push_back(record);

		std::sort(all_records.begin(), all_records.end());

		for (int i = 0; i < all_records.size() && i <= 10; i++)
			add_record(all_records[i]);	

		return;
	}

	QList<Record> all_records;

	for (auto& category : records[current_difficulty])
	{
		for (auto& record : category)
			all_records.push_back(record);
	}

	std::sort(all_records.begin(), all_records.end());

	for (int i = 0; i < all_records.size() && i <= 10; i++)
		add_record(all_records[i]);	

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

void Records::load_records(QList<Record> unfiltered_records)
{
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

QList<QList<QList<Record>>> Records::get_records()
{
	return records;
}

void Records::change_background(QString& background)
{
	setStyleSheet("Records { background-image: url( " + background + "); }");
}