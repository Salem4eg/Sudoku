#pragma once

#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include "Structures.h"

class Records : public QWidget
{
	Q_OBJECT

public slots:
	// отримувати рекорд під час гри
	void get_new_record(Record record);
	void change_background(QString& background);
	QList<QList<QList<Record>>> get_records();
	void load_records(QList<Record> unfiltered_records);

signals:
	void leave();

public:
	Records(QWidget *parent = nullptr);
	~Records();

protected:
	void paintEvent(QPaintEvent * event);

private:

	void add_record(const Record& record);
	void change_current_records();
	void clear_record_widgets();
	void change_category();
	void set_difficulty(Difficulties difficulty);

	bool hints_allowed;
	bool errors_allowed;

	Difficulties current_difficulty;
	Categories current_category;

	QVBoxLayout * records_value_layout;

	// records поділяється на складність, а складність поділяється на категорію (без помилок + без підказок, без підказок, без помилок, усі рекорди)
	QList<QList<QList<Record>>> records;
};
