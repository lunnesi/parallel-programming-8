#include <iostream>
#include <cmath>
#include <omp.h>
#include <iomanip>

using namespace std;

double f(double x) {
    return exp(-x * x + 0.38) / (2.0 + sin(1.0 / (1.5 + x * x)));
}

int main() {
    omp_set_num_threads(24);
    //границы интегрирования
    double a = 0.4;
    double b = 1.0;
    long n = 10000000; 

    //для метода количество шагов n должно быть четным
    if (n % 2 != 0) n++;

    //шаг интегрирования
    double h = (b - a) / n;
    double sum = 0.0;
    double start_time = omp_get_wtime();


    //Используем директиву 'reduction', чтобы безопасно суммировать результаты из разных потоков
#pragma omp parallel for reduction(+:sum)
    for (long i = 1; i < n; i++) {
        double x = a + i * h;

        //коэффициенты чередуются: 4 для нечетных шагов, 2 для четных
        if (i % 2 == 0) {
            sum += 2.0 * f(x);
        }
        else {
            sum += 4.0 * f(x);
        }
    }
    //I ≈ (h/3) * [f(a) + f(b) + сумма_внутренних_точек]
    double result = (h / 3.0) * (f(a) + f(b) + sum);
    double end_time = omp_get_wtime();

  
    cout << fixed << setprecision(10);

    cout << " Simpson's Method with OpenMP" << endl;
    cout << " Interval: [" << a << ", " << b << "]" << endl;
    cout << " Number of steps (n): " << n << endl;
    cout << " Result: " << result << endl;
    cout << " Execution Time: " << (end_time - start_time) << " seconds" << endl;
    cout << " Max Threads: " << omp_get_max_threads() << endl;


    return 0;
}
