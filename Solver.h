#pragma once
#include <QList>
#include <unordered_set>
#include <QHash>
#include <QSet>
#include "Structures.h"



//вирішувач судоку має мати в собі купу технік для вирішення судоку. завдяки ним він перевірить чи можна вирішити головоломку, а також визначає складність цього судоку.
class Solver
{
private:
	int size;
protected:
	QList<QList<QList<int>>> notes;
	QList<QList<int>> field;
public:
	//заповнюємо записки числами, які можуть підійти. Тобто, кандидатами.
	Solver(const QList<QList<int>>& uncompleted_sudoku) : size(uncompleted_sudoku.size()), notes(size, QList<QList<int>>(size)), field(uncompleted_sudoku)
	{
		for (int row = 0; row < size; row++)
		{
			for (int col = 0; col < size; col++)
			{
				if (uncompleted_sudoku[row][col] == 0)
				{
					for (int i = 1; i <= 9; i++)
					{
						if (isValid(row, col, i, uncompleted_sudoku))
							notes[row][col].push_back(i);
					}
				}
			}
		}
	}
	Solver() : size(-1)
	{
	}

	void solve();


	void load_puzzle(const QList<QList<int>>& uncompleted_sudoku);
	QList<QList<int>> get_sudoku();
	QList<QList<QList<int>>> get_notes();
	bool isValid(int row, int col, int number, const QList<QList<int>>& field) const;
	bool isSolved();

	bool solve_with_easy_techniques();
	bool solve_with_normal_techniques();
	bool solve_with_hard_techniques();
private:

	bool use_easy_techniques();
	bool use_normal_techniques();
	bool use_hard_techniques();

	void clear_row_and_col(int row, int col, int number);
	void clear_row(int row, int number);
	void clear_col(int col, int number);
	void clear_box(int boxrow, int boxcol, int number);

	bool clear_field(int row, int col, int number);

	bool clear_row_except(int row, int number, int exceptindex);
	bool clear_col_except(int col, int number, int exceptindex);

	bool clear_col_except(int col, int number, QSet<int> except_indexes);
	bool clear_row_except(int row, int number, QSet<int> except_indexes);


	bool clear_row_in_box(int boxrow, int boxcol, int row, int number);
	bool clear_col_in_box(int boxrow, int boxcol, int col, int number);

	
	// видалення в рядку всіх чисел, окрім виключних координат
	bool clear_numbers_in_row_except(int row, QSet<int>& numbers, QSet<coords>&& except_coords);
	bool clear_numbers_in_col_except(int col, QSet<int>& numbers, QSet<coords>&& except_coords);
	bool clear_numbers_in_box_except(int boxrow, int boxcol, QSet<int>& numbers, QSet<coords>&& except_coords);

	
	bool delete_numbers_in_field_except_numbers(int row, int col, QList<int>& except_numbers);



public:
	//перевірка на те, чи є поля у яких може бути лише одне число, якщо так, то вставити число в поле й видалити зі записок.
	bool single_candidate();


	// перевірка кожного квадрата 3на3, чи є в ньому число, яке може бути лише в одному місці
	bool single_position();

private:
	//перевірка на те, чи є у квадраті (поле 3 на 3) число, яке може бути лише в одному місці
	bool single_position_box(int boxrow, int boxcol);



public:
	// Перевірка чи число в квадраті є лише на одній лінії (горизонтальній або вертикальній), 
	// бо якщо є, то це означає, що це число не може стояти на цій ж лінії, але в інших квадратах
	bool candidate_lines();

private:
	bool candidate_lines_box(int boxrow, int boxcol);



public:
	// Пошук двох пар, які стоять на лише на двох лініях (горизонтальних і/або вертикальних). Це означає, що це число на іншому квадраті не може бути на тих самих лініях
	bool double_pairs();
	
private:
	two_lines_coordinates double_pairs_box(int boxRow, int boxCol, int number);
	bool double_pairs_number(int number);



public:
	// Пошук двох чисел, які лежать лише у двох на двох полях, тобто є поле де можливі числа { 1, 5 } і є ще одне таке ж { 1, 5 }.
	// Це означає, що ті числа обов'язково будуть саме тут, просто не відомо де саме, тому їх можна прибрати з інших місць. АЛЕ
	// Якщо ти знайшов ті числа в одній коробці, то кандидатів треба прибирати саме в тій коробці, так само з рядками та стовпцями.
	bool naked_pair();

private:
	bool naked_pair_rows();
	bool naked_pair_cols();
	bool naked_pair_box();





public:
	bool hidden_numbers();

private:
	bool hidden_numbers_in_row(int row, int numbers_to_find);
	bool hidden_numbers_in_column(int col, int numbers_to_find);
	bool hidden_numbers_in_box(int box, int numbers_to_find);
	
	bool hidden_numbers_in_rows(int numbers_to_find);
	bool hidden_numbers_in_columns(int numbers_to_find);
	bool hidden_numbers_in_box(int numbers_to_find);


	bool is_good_overlaping(int number, QList<int>& combination, QHash<int, QList<coords>>& number_occurences, int max_numbers, QSet<coords>& overlapings);
	bool is_good_combination(QList<int>& combination, QHash<int,QList<coords>>& number_occurences, int max_numbers, QSet<coords>& overlapings);
	QList<QList<int>> get_combinations(QList<int>& numbers, int size);
	QList<int> get_potential_numbers(QList<int>& numbers, QHash<int,QList<coords>>& occurence, int max_numbers);



		
public:
	bool x_wings();
	bool swordfish();
	bool jellyfish();


private:
	bool x_wings_rows(int number);
	bool x_wings_columns(int number);
	
	
	bool swordfish_rows(int number);
	bool swordfish_columns(int number);
	
	
	bool jellyfish_rows(int number);
	bool jellyfish_columns(int number);


	/*
	* Х-крила, риба-меч та медуза - це одна й та ж тактика, але з різним масштабом (для Х-крил треба лише два рядка\стовпця, а для медузи чотири,
	* так само з найбільшою можливою к-стю чисел на рядку\стовпці. Найменша к-сть чисел на рядку\стовпці - це два.
	* Тож наступні функції потрібні для них трьох, а різниця між алгоритмами буде лише в параметрах, які вони надсилають до тих функцій. 
	* 
	* Пояснення стратегії:
	*	 A Б В Г
	* 1| 2 2 2	   Приклад на стовпцях (А і Б)
	* 2|  	   2   Не відомо правильної позиції двійок, чи A1 Б2, чи Б1 A2
	* 3| 2 2 2 2   Але двійка обов'язково буде на рядках 1 та 2, то ж двійки на стовпці В та Г можна прибрати
	* 4|     2 2
	* 5|     2 2
	* 
	*/

	//Надає масив рядків з стовпцями, на яких бажане число не з'являлось менше 2 разів і не більше maximum_occurence
	QList<QPair<int, QList<int>>> get_rows_with_number(int number, int maximum_occurence);

	//Надає масив стовпців з рядками, на яких бажане число не з'являлось менше 2 разів і не більше maximum_occurence
	QList<QPair<int, QList<int>>> get_columns_with_number(int number, int maximum_occurence);

	//Дає невпорядковану мапу, де кожен індекс показує к-сть разів, яку він з'являвся у списку
	QHash<int, int> get_indexes_occurence(QList<QPair<int, QList<int>>>& lists);

	//Надає індекси з масиву в QPair, де перше значення це індекс одного рядка/стовпця, а друге це індекси стовпців/рядків на яких є число
	QSet<int> get_indexes_from_list(QList<QPair<int, QList<int>>>& list);

	//Повертає всі можливі комбінації індексів списків. До прикладу, 
	//стовпці, які мають у собі рядки на яких з'являлось число, індекси стовпців: 1, 2, 3, а розмір: 2
	//Повертає [1,2], [1,3], [2,3]
	QList<QList<int>> get_lists_combinations(QList<QPair<int, QList<int>>>& list, int size);


	
	



	
	


	
	
	





public:
	bool simple_chains();


private:
	bool simple_chains_number(int number);

	// Перевірка на те, чи є в рішенні кандидати, які стоять на одному рядку\стовпці\квадраті, тобто перевірка чи це рішення може бути не правильним
	// Повертає 0, якщо не видно очевидно не правильного рішення, 1 - якщо перше рішення не правильне, 2 - якщо друге
	int find_wrong_possible_solution(QPair<QList<coords>, QList<coords>>& chain);
	// Пошук кандидатів поза ланцюгом, які стоять на одну рядку\стовпці\квадраті з кандидатами з обох рішень, тобто тих кандидатів, які незалежно від рішення будуть усунуті
	bool find_candidates_affected_by_both_solutions(QPair<QList<coords>, QList<coords>>& chain, int number);
	bool find_candidates_affected_by_both_solutions_in_box(QList<coords>& first_solution, QList<coords>& second_solution, int number);
	bool process_boxes(QList<coords>& solution, QHash<coords, coords>& boxes_to_process, QHash<coords, coords>& boxes_avoid, int number);
	bool delete_candidates_affected_by_both_solutions_in_box(coords& candidate, coords box_coords, coords except_candidate, int number);







	/*
	*	Ланцюги складаються зі зв'язків чисел. 
	* 
	*	Ланцюг по своїй природі має лише два варіанти вирішення
	*
	* 
	*	 Р - рішення, Н - не рішення
	*		 
	*		Р - Н           Н - Р
	*		    |     або       |
	*		    Р               Н
	*	 
	* 
	*	Є два типи зв'язків: звичайні й двочислові.
	* 
	*	Звичайний зв'язок:
	*	Зв'язки чисел, це єдині два числа на рядку/стовпці/квадраті. Це означає, що або одне число є рішенням, або інше.
	*	Число з однієї зв'язки може бути числом іншої зв'язки, так утворюються ланцюги:
	*	
	*	3 - 3 
	*	    |   Простий ланцюг з трьох чисел (двох зв'язок)
	*	    3 
	*	
	*	
	*	Двочисловий зв'язок:
	*	Схожий на звичайний зв'язок, але зв'язок може складатись з різних чисел.
	* 
	* 
	*	3,4 - 4,5
	*		   |	Двочисловий зв'язок
	*		  3,5
	* 
	*/

	/*
	*	Ця функцію знаходить зв'язки кандидата
	*	
	*	3 - 3 <- наш кандидат
	*	    | 
	*	    3 <- інший кандидат, що зв'язаний з нашим кандидатом
	* 
	*	Повертає координати кандидатів, що зв'язані з цільовим кандидатом
	*/



	// Буде шукати зв'язки, що з'єднані з наданим кандидатом
	QList<coords> find_bi_location_links_of_candidate(coords candidate, QList<QPair<coords,coords>>& links);
	// Надасть усі зв'язки, а тобто єдині два кандидати на рядок\стовпець\квадрат 
	QList<QPair<coords,coords>> find_all_bi_location_links(int number);


	// Ця функція використає функцію з пошуку всіх зв'язків, а потім буде відправляти по одному в find_bi_location_chain (ЯКЩО цей зв'язок ще не був використаний!)
	QList<QPair<QList<coords>, QList<coords>>> find_bi_location_chains(int number);

	// Ця функція буде будувати ланцюг з наданого першого зв'язка. Наступні зв'язки шукає використовуючи bi_location_lins_of_candidate
	QPair<QList<coords>, QList<coords>> find_bi_location_chain(QPair<coords, coords>& link, QList<QPair<coords,coords>>& links, QSet<coords>& used_candidates);





public:

	bool xy_wing();

	bool xyz_wing();

	//bool wxyz_wing();


private:
	QHash<int,QList<QList<coords>>> get_chains(int start_cell_size);

	bool clear_candidates_that_seen_from_chains(QList<coords>& chain, int number);
	bool clear_candidate_that_seen_from_2_cells(QList<coords>& chain, int number);
	bool clear_candidates_from_box_that_seen_from_chains(QList<coords>& chain, int number);
	bool clear_candidates_from_box_that_seen_from_cell(QList<coords>& except_cells, coords& cell, coords& box_to_clear, int number);

	void build_wing_chains(int row, int col, QHash<int, QList<QList<coords>>>& chains);
	QList<coords> get_cells_that_see_cell(int start_cell_row, int start_cell_col);
	QList<coords> get_cells_that_see_cell_xy_wing(int start_cell_row, int start_cell_col);
	void build_wings_with_hinge_number(QList<coords>& cells, int cells_amount, coords start_cell, QHash<int, QList<QList<coords>>>& chains, int hinge_number);
	QList<QList<int>> get_indexes_combinations(int indexes_amount, int array_size);
	void build_chains_with_combinations(QList<coords>& cells, int cells_amount, coords start_cell, QHash<int, QList<QList<coords>>>& chains, QList<QList<int>>& combinations, int hinge_number);
	void add_chain_if_valid_xy_wing(bool cell_in_start_cell_row, bool cell_in_start_cell_col, bool cell_in_start_cell_box, QSet<int>& used_numbers, coords& start_cell, QList<int>& combination, QList<coords>& cells,  QHash<int, QList<QList<coords>>>& chains, int hinge_number);
	void add_chain_if_valid_xyz_wing(bool cell_in_start_cell_row, bool cell_in_start_cell_col, bool cell_in_start_cell_box, QSet<int>& used_numbers, coords& start_cell, QList<int>& combination, QList<coords>& cells,  QHash<int, QList<QList<coords>>>& chains, int hinge_number);
	void build_chains_with_combinations_wxyz(QList<coords>& cells, int cells_amount, coords start_cell, QHash<int, QList<QList<coords>>>& chains, QList<QList<int>>& combinations, int hinge_number);
};
