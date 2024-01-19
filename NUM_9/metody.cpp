#include <iostream>
#include <vector>
#include <cmath>

const double EPSILON = 1e-6;
const int MAX_ITERATIONS = 1000;
const double SOLUTION = asin(0.4);

using std::asin;
using std::vector;
using std::abs;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

pair<vector<double>, double> Bisekcja(double (*fun)(double),double x1, double x2)
{
    vector<double> errors;
    double x3, last = 0;

    for (int i = 0; i <= MAX_ITERATIONS; i++)
    {
        x3 = (x1 + x2) / 2;

        errors.push_back(abs(x3 - SOLUTION));
        
        if(abs(x3 - last) < EPSILON) break;

        if (fun(x1) * fun(x3) < 0) x2 = x3;
        else if (fun(x3) * fun(x2) < 0) x1 = x3;
        else cout << "Metoda Bisekcji nie jest wstanie okreslic!" << endl;

        last = x3;
    }
    return make_pair(errors,x3);
}

pair<vector<double>, double> Falsi(double (*fun)(double),double x1, double x2)
{
    vector<double> errors;
    double x3;

    for (int i = 0; i < MAX_ITERATIONS; i++)
    {
        x3 = (fun(x1) * x2 - fun(x2) * x1) / (fun(x1) - fun(x2));

        errors.push_back(abs(x3 - SOLUTION));
        
        if(abs(x3 - x2) <= EPSILON) break;

        if (fun(x1) * fun(x3) < 0) x2 = x3;
        else if (fun(x3) * fun(x2) < 0) x1 = x3;
        else cout << "Metoda Regula Falsi nie jest wstanie okreslic!" << endl;
    }
    return make_pair(errors,x3);
}

pair<vector<double>, double> Sieczne(double (*fun)(double), double x1, double x2) 
{
    vector<double> errors;
    double x3;
    for (int i = 0; i < MAX_ITERATIONS; i++) 
    {
        x3 = x2 - fun(x2) * ((x2 - x1) / (fun(x2) - fun(x1)));

        errors.push_back(abs(x3 - SOLUTION));

        if (abs(x3 - x2) <= EPSILON) break;

        x1 = x2;
        x2 = x3;
    }
    return make_pair(errors,x3);
}

pair<vector<double>, double> Newton(double (*fun)(double), double (*derr_fun)(double), double x) 
{
    vector<double> errors;
    double next_x;

    for (int i = 0; i < MAX_ITERATIONS; i++) 
    {
        next_x = x - fun(x) / derr_fun(x);

        errors.push_back(abs(next_x - SOLUTION));

        if (abs(next_x - x) <= EPSILON) break;

        x = next_x;
    }
    return make_pair(errors,next_x);
}