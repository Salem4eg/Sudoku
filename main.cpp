#include "GUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    
    GUI w;
    w.show();

    // \/ доки не готовий новий алгоритм, то це приховано
    /*Testing_solver solver_test;
    solver_test.show();*/

    // Для перевірки нових алгоритмів
    //Testing_algorithms algorithm;
    //algorithm.show();
    //algorithm.test_xyz_wing();

    //algorithm.check_with_random_generated_tests();
    //algorithm.test_simple_chains_with_failed_random_generated_test();
    //algorithm.test_simple_chains_with_random_generated_tests();
    return a.exec();
}
