#pragma once

#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>

#include "Records.h"
#include "Main_menu.h"
#include "Play_menu.h"
#include "Settings_menu.h"
#include "Tests.h"
#include "GameInfo.h"

class GUI : public QMainWindow
{
	Q_OBJECT

public slots:
	void play_new_game();
	void continue_game();
	void show_main_menu();
	void show_records_menu();
	void show_settings_menu();

	void change_theme(Theme theme);
public:
	GUI(QWidget* parent = nullptr);
	~GUI();
protected:
	void closeEvent(QCloseEvent * event) override;
private:
	QStackedWidget * menus;
	
	Records * personal_records;
	Main_menu * main_menu;
	Play_menu * play_menu;
	Settings_menu * settings_menu;

	QGridLayout * layout;

	QColor background_color;
	QColor text_color;

	Text_size text_size;
	Theme current_theme;

	QDialog * set_difficulty_dialog;

	GameInfo game_info;

	bool has_saved_game;
};
