#include <iostream>
#include <cmath>
#include <vector>
#include "metody.cpp"
#include "gnuplot-iostream.h"

using std::endl;
using std::cout;
using std::sin;
using std::cos;
using std::pow;

double FunG(double x)
{
    double val = sin(x) - 0.4;
    return val * val;
}

double derrFunG(double x)
{
    return 2 * (sin(x)-0.4)* cos(x);
}

double u_FunG(double x)
{
    return FunG(x) / derrFunG(x);
}

double u_derrFunG(double x)
{
    return ((sin(x)-0.4)* sin(x))/(2*cos(x)*cos(x)) + 0.5;
}

int main()
{
    
    cout << "Dla g(x): " << endl;
    auto res_Sie = Sieczne(FunG, 0, 0.5);
    vector Err_Sie = res_Sie.first;
    double miej_zer_Sie = res_Sie.second;
    cout << "Metoda Siecznych: " << miej_zer_Sie << endl;

    auto res_New = Newton(FunG,derrFunG,0);
    vector Err_New = res_New.first;
    double miej_zer_New = res_New.second;
    cout << "Metoda Newtona: " << miej_zer_New << endl;

    cout << "Dla u(x): " << endl;
    auto u_res_Sie = Sieczne(u_FunG, 0, 0.5);
    vector u_Err_Sie = u_res_Sie.first;
    double u_miej_zer_Sie = u_res_Sie.second;
    cout << "Metoda Siecznych: " << u_miej_zer_Sie << endl;

    auto u_res_New = Newton(u_FunG,u_derrFunG,0);
    vector u_Err_New = u_res_New.first;
    double u_miej_zer_New = u_res_New.second;
    cout << "Metoda Newtona: " << u_miej_zer_New << endl;

    Gnuplot wykres1;
    // wykres1 << "set terminal eps\n";
    // wykres1 << "set output 'wyniki_b_g(x).eps'\n";
    wykres1 << "set title 'Zbieżność metod iteracyjnych dla f(x) = (sin(x) - 0.4)^2' font 'Arial,15'\n";
    wykres1 << "set grid\n";
    wykres1 << "set xlabel 'Liczba iteracji' font 'Arial,13'\n";
    wykres1 << "set ylabel '|x^i - x^*| w skali logarytmicznej' font 'Arial,13'\n";
    wykres1 << "set logscale y\n";
    wykres1 << "set xrange [0:21]\n";
    wykres1 << "set key font 'Arial Bold,12'\n";
    wykres1 << "plot '-' with linespoints pointtype 7 pointsize 0.5 title 'Metoda Siecznych',"
         << " '-' with linespoints pointtype 7 pointsize 0.5 title 'Metoda Newtona'\n";
    wykres1.send(Err_Sie);
    wykres1.send(Err_New);

    Gnuplot wykres2;
    // wykres2 << "set terminal eps\n";
    // wykres2 << "set output 'wyniki_b_u(x).eps'\n";
    wykres2 << "set title 'Zbieżność metod iteracyjnych dla u(x) = g(x)/g''(x)' font 'Arial,15'\n";
    wykres2 << "set grid\n";
    wykres2 << "set xlabel 'Liczba iteracji' font 'Arial,13'\n";
    wykres2 << "set ylabel '|x^i - x^*| w skali logarytmicznej' font 'Arial,13'\n";
    wykres2 << "set logscale y\n";
    wykres2 << "set xrange [0:4]\n";
    wykres2 << "set key font 'Arial Bold,12'\n";
    wykres2 << "plot '-' with linespoints pointtype 7 pointsize 0.5 title 'Metoda Siecznych',"
        << " '-' with linespoints pointtype 7 pointsize 0.5 title 'Metoda Newtona'\n";
    wykres2.send(u_Err_Sie);
    wykres2.send(u_Err_New);
    return 0;
}