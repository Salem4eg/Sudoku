#include "GUI.h"
#include <QtWidgets/QApplication>

/*
* // \/ доки не готовий новий алгоритм, то це приховано
    //Testing_solver solver_test;
    //solver_test.show();

    // Для перевірки нових алгоритмів
    //Testing_algorithms algorithm;
    //algorithm.show();
    //algorithm.test_hidden_candidates();

    //algorithm.check_with_random_generated_tests();
    //algorithm.test_simple_chains_with_failed_random_generated_test();
    //algorithm.test_simple_chains_with_random_generated_tests();

*/

#include "Maker_window.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    
    GUI w;
    w.show();

    
    return a.exec();
}
