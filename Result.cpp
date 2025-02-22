#include "Result.h"

Result::Result(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle("Результат гри");

	auto* layout = new QVBoxLayout(this);

	result_label = new QLabel(this);

	info_widget = new QStackedWidget(this);

	auto* info_layout = new QVBoxLayout(info_widget);

	set_won_info_widget();
	set_lost_info_widget();

	auto * options_widget = new QWidget(this);
	auto* options_layout = new QHBoxLayout(options_widget);

	auto* back_to_menu_button = new QPushButton("У головне меню", this);
	auto* play_again_button = new QPushButton("Зіграти ще раз", this);

	options_layout->addWidget(back_to_menu_button);
	options_layout->addSpacing(20);
	options_layout->addWidget(play_again_button);

	layout->addWidget(result_label, 1, Qt::AlignCenter);
	layout->addWidget(info_widget, 4, Qt::AlignCenter);
	layout->addWidget(options_widget, 1, Qt::AlignCenter);

	setModal(true);

	connect(back_to_menu_button, &QPushButton::pressed, [=]()
		{
			emit back_to_menu();
			close();
		});

	connect(play_again_button, &QPushButton::pressed, [=]()
		{
			emit play_again();
			close();
		});
}

void Result::set_won_info_widget()
{
	auto * won_info_widget = new QWidget;
	auto * won_info_layout = new QVBoxLayout(won_info_widget);

	time_label = new QLabel;
	difficulty_label = new QLabel;

	auto * hints_and_errors_layout = new QHBoxLayout;

	errors_label = new QLabel;
	hints_label = new QLabel;

	hints_and_errors_layout->addWidget(errors_label, 1, Qt::AlignCenter);
	hints_and_errors_layout->addWidget(hints_label, 1, Qt::AlignCenter);

	won_info_layout->addWidget(time_label, 1, Qt::AlignCenter);
	won_info_layout->addWidget(difficulty_label, 1, Qt::AlignCenter);
	won_info_layout->addLayout(hints_and_errors_layout);


	info_widget->addWidget(won_info_widget);
}

void Result::set_won_info(Record& result)
{
	QString difficulty_name;

	switch (result.difficulty)
	{
	case easy:
		difficulty_name = "легка";
		break;
	case normal:
		difficulty_name = "нормальна";
		break;
	case hard:
		difficulty_name = "складна";
		break;
	case ultra_hard:
		difficulty_name = "надскладна";
		break;
	default:
		break;
	}

	int time_hours = result.time / 3600;
	int time_minutes = (result.time / 60) % 60;
	int time_seconds = result.time % 60;

	QString time;
	if (time_hours > 0)
		time = QString::number(time_hours) + "год " + QString::number(time_minutes) + "хв " + QString::number(time_seconds) + "с";
	else
		time = QString::number(time_minutes) + "хв " + QString::number(time_seconds) + "с";
	
	result_label->setText("Вітаю!");
	time_label->setText("Ви справились за: " + time);
	difficulty_label->setText("Складність: " + difficulty_name);
	errors_label->setText("Помилок: " + QString::number(result.errors) + " / 3");
	hints_label->setText("Підказок: " + QString::number(result.hints) + " / 3");
}

void Result::set_lost_info()
{
	result_label->setText("Зрада..");
	int index = QRandomGenerator::global()->bounded(0, phrases.size());

	phrase->setText(phrases[index]);	
}

void Result::set_lost_info_widget()
{
	auto* lost_info_widget = new QWidget;
	auto* lost_info_layout = new QVBoxLayout(lost_info_widget);

	phrase = new QLabel();

	phrases.push_back("Я розчарований у вас..");
	phrases.push_back("Ти ганьдьба для своїх пращурів");
	phrases.push_back("Ти взагалі вмієш грати?");
	phrases.push_back("Твій інтелект нижче середнього");
	phrases.push_back("Ти взагалі знаєш, що таке судоку?");
	phrases.push_back("Якби від тебе залежала доля людства..");
	phrases.push_back("Ти взагалі вмієш думати?");
	phrases.push_back("Якби ти випадково клацав, то зіграв би краще");
	phrases.push_back("Навіть душ не відмиє від цього позору");
	phrases.push_back("Спробуй простіші ігри");
	phrases.push_back("Спробуй хоч трішки подумати");

	lost_info_layout->addWidget(phrase, 0, Qt::AlignHCenter | Qt::AlignTop);

	info_widget->addWidget(lost_info_widget);
}

void Result::show_won(Record& result)
{
	set_won_info(result);
	info_widget->setCurrentIndex(0);
	show();
}

void Result::show_lost()
{
	set_lost_info();
	info_widget->setCurrentIndex(1);
	show();
}

void Result::change_theme(Theme theme)
{
	setStyleSheet("background-color:" + theme.background_color.name() + ";");
}


Result::~Result()
{}
