#pragma once

#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "Structures.h"

class Records : public QWidget
{
	Q_OBJECT

public slots:
	// діставатиме рекорди з бази даних
	void load_records(QList<QList<QList<Record>>>& records_from_database);
	// отримувати рекорд під час гри
	void get_new_record(Record record);

signals:
	void leave();

public:
	Records(QWidget *parent = nullptr);
	~Records();

	const QList<QList<QList<Record>>>& get_records() const;
private:

	void add_record(const Record& record);
	// Якщо в категорії пусто, то можна написати текст "Поки що тут пусто"
	void change_current_records();
	void clear_record_widgets();
	void change_category();
	void set_difficulty(Difficulties difficulty);

	bool hints_allowed;
	bool errors_allowed;

	Difficulties current_difficulty;
	Categories current_category;

	QVBoxLayout * records_value_layout;

	QList<QList<QList<Record>>> records;
};
