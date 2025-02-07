#pragma once

#include <QPushButton>
#include <QPainter>

class Cell : public QPushButton
{
	Q_OBJECT

	enum State
	{
		Default, Wrong, Highlighted, Highlighted_active
	};


signals:
	void set_active(Cell * cell);

public:
	Cell(QWidget *parent, int row, int col);
	~Cell();

	
	void add_candidate(int number);
	void remove_candidate(int number);
	void set_number(int number);
	void highlight_number(int number);

	void toggle_wrong_state(int number = -1);

	void highlight_cell();
	void highlight_as_active_cell();
	void set_default_state();
	


	bool is_finished();
	bool is_wrong();
	bool is_candidate_noted(int number);
	
	void clear();

	// Повертає finished_number не перевіряючи чи клітинка завершена
	int get_number();
	QList<int> get_candidates();
	QPoint get_coords();

protected:
	void paintEvent(QPaintEvent * event) override;

private:
	QList<bool> notepad;
	State cell_state;
	bool finished;
	int finished_number;
	int wrong_number;
	int highlighted_number;
	QPoint coords;
	// Зберігає координати прямокутників, що присвоєні до свого числа. Використовується, щоб одразу знати де малювати підсвічений прямокутник для числа, а також для намалювання кандидата
	QList<QPoint> number_rectangles;
};
