#pragma once

#include <QWidget>
#include "Structures.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QLineEdit>


class Appearance_menu : public QWidget
{
	Q_OBJECT

public:
	Appearance_menu(QList<Theme> themes, int active_theme = 0, QWidget *parent = nullptr);
	~Appearance_menu();
	void init();

signals:
	void leave();
	void theme_changed(Theme theme);
	void text_size_changed(Text_size size);


private:
	void set_header(QWidget * header);
	void set_themes(QWidget * themes_widget);
	void set_theme_options(QWidget * theme_options);
	void set_text_color_options(QWidget * text_options);
	void set_field_and_border_color_options(QWidget * field_and_border_options);
	void set_background_options(QWidget * background_options);
	

	void set_general_options(QWidget * general_options);
	
	void add_theme(Theme theme);
	void delete_theme(int theme_index);
	void change_theme_options(Theme theme);
	void rename_theme(int theme_index);

	void set_theme_control(QWidget * theme_control);
	void set_default_themes();
	
	QList<Theme> themes;
	int active_theme_index;

};


// Потрібно зберігати всі теми
