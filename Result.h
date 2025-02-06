#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include "Structures.h"
#include <QRandomGenerator>

class Result  : public QDialog
{
	Q_OBJECT

		
public slots:
	void show_won(Record& result);
	void show_lost();

signals:
	void back_to_menu();
	void play_again();


public:
	Result(QWidget *parent);
	~Result();


private:
	QStackedWidget* info_widget;

	QLabel * result_label;
	QLabel * time_label;
	QLabel * errors_label;
	QLabel * hints_label;
	QLabel * difficulty_label;

	QLabel * phrase;

	QList<QString> phrases;

	void set_won_info(Record& result);
	void set_won_info_widget();

	void set_lost_info();
	void set_lost_info_widget();
};
