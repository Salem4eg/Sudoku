#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QLabel>
#include <QPushButton>
#include <QStyleOption>
#include <QPainter>

#include "GameInfo.h"
#include "Structures.h"

class Settings_menu : public QWidget
{
	Q_OBJECT

public:
	Settings_menu(QWidget *parent = nullptr);
	~Settings_menu();

	Theme get_current_theme();
	void save_settings();

signals:
	void theme_changed(Theme theme);
	void fill_candidates_at_start(bool fill);
	void remove_invalid_candidates(bool remove);
	void leave();

protected:
	void paintEvent(QPaintEvent * event);

private:
	void toggle_fill_candidates_button();
	void toggle_remove_candidates_button();

	void load_settings();

	void create_themes();
	void change_theme();
	bool fill_candidates;
	bool remove_candidates;

	QList<Theme> themes;
	QList<QLabel *> labels;

	QPushButton * fill_candidates_button;
	QPushButton * remove_invalid_candidates_button;

	Theme current_theme;

	GameInfo game_info;
};
