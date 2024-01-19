#include <iostream>
#include <cmath>
#include <vector>
#include "metody.cpp"
#include "gnuplot-iostream.h"

using std::endl;
using std::cout;

double FunF(double x)
{
    return sin(x) - 0.4;
}

double derrFunF(double x)
{
    return cos(x);
}

int main()
{
    auto res_Bis = Bisekcja(FunF, 0, M_PI_2);
    vector Err_Bis = res_Bis.first;
    double miej_zer_Bis = res_Bis.second;
    cout << "Metoda Bisekcji: " << miej_zer_Bis << endl;

    auto res_Fal = Falsi(FunF, 0, M_PI_2);
    vector Err_Fal = res_Fal.first;
    double miej_zer_Fal = res_Fal.second;
    cout << "Metoda Regula Falsi: " << miej_zer_Fal << endl;

    auto res_Sie = Sieczne(FunF, 0, M_PI_2);
    vector Err_Sie = res_Sie.first;
    double miej_zer_Sie = res_Sie.second;
    cout << "Metoda Siecznych: " << miej_zer_Sie << endl;

    auto res_New = Newton(FunF,derrFunF,0);
    vector Err_New = res_New.first;
    double miej_zer_New = res_New.second;
    cout << "Metoda Newtona: " << miej_zer_New << endl;

    Gnuplot wykres;
    // wykres << "set terminal eps\n";
    // wykres << "set output 'wyniki_a.eps'\n";
    wykres << "set title 'Zbieżność metod iteracyjnych dla f(x) = sin(x) - 0.4' font 'Arial,15'\n";
    wykres << "set grid\n";
    wykres << "set xlabel 'Liczba iteracji' font 'Arial,13'\n";
    wykres << "set ylabel '|x^i - x^*| w skali logarytmicznej' font 'Arial,13'\n";
    wykres << "set logscale y\n";
    wykres << "set xrange [0:21]\n";
    wykres << "set key font 'Arial Bold,12'\n";
    wykres << "plot '-' with linespoints pointtype 7 pointsize 0.5 title  'Metoda Bisekcji' ,"
        << " '-' with linespoints  pointtype 7 pointsize 0.5 title 'Metoda Regula Falsi',"
        << " '-' with linespoints  pointtype 7 pointsize 0.5 title 'Metoda Siecznych',"
        << " '-' with linespoints pointtype 7 pointsize 0.5 title 'Metoda Newtona'\n";
    wykres.send(Err_Bis);
    wykres.send(Err_Fal);
    wykres.send(Err_Sie);
    wykres.send(Err_New);
    return 0;
}