#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QColor>
#include <QStyleOption>
#include <QPainter>

class Main_menu : public QWidget
{
	Q_OBJECT

public:
	Main_menu(QWidget* parent = nullptr);
	~Main_menu();

public slots:
	void change_appearance();
	void show_continue_option(bool show);
	void change_main_menu_background(QString filename);
signals:
	void play_new_game();
	void continue_game();
	void show_personal_recods();
	void show_appearance_menu();
	void show_guide();
	void close_program();

protected:
	void paintEvent(QPaintEvent*);

private:
	void set_button_style(QPushButton* button);
	void change_buttons_style();

	QColor text_color;
	QColor background_color;

	QLabel * game_name;
	QPushButton * play_button;
	QPushButton * continue_game_button;
	QPushButton * personal_best_button;
	QPushButton * appearance_button;
	QPushButton * guide_button;
	QPushButton * exit_button;
};
