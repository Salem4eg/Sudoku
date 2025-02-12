#pragma once

#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>

#include "Records.h"
#include "Main_menu.h"
#include "Play_menu.h"
#include "Appearance_menu.h"
#include "Tests.h"

class GUI : public QMainWindow
{
	Q_OBJECT

public slots:
	void play_new_game();
	void continue_game();
	void show_main_menu();
	void show_records_menu();
	void show_appearance_menu();

	void change_theme(Theme theme);
	void change_text_size(Text_size size);
public:
	GUI(QWidget* parent = nullptr);
	~GUI();
protected:
	void closeEvent(QCloseEvent * event) override;
private:
	void set_button_style(QPushButton* button);

	QStackedWidget * menus;
	
	Records * personal_records;
	Main_menu * main_menu;
	Play_menu * play_menu;
	Appearance_menu * appearance_menu;

	QGridLayout * layout;

	QColor background_color;
	QColor text_color;

	Text_size text_size;
	Theme current_theme;
};
