#include <iostream>
#include "gnuplot-iostream.h"
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::to_string;

const int N = 25;
const int ACC = 100;

double funkcja(double x) 
{
    return 1.0 / (1.0 + (50 * x * x));
}

vector<vector<double>> Jednorodne(int n)
{
    vector<vector<double>> punkty(2,vector<double>(n));

    for (int i = 0; i <= n; i++)
    {
        double x = -1.0 + (2.0*i) / (n-1);
        punkty[0][i] = x;
    }
    return punkty;
}

vector<vector<double>> Cosinusoid(int n)
{
    vector<vector<double>> punkty(2,vector<double>(n));

    for (int i = 0; i < n; i++)
    {
         double x = cos((2 * i + 1) / (2.0 * (n)) * M_PI);
        punkty[0][i] = x;
    }
    return punkty;
}

double lagrangeInterpolation(const std::vector<std::vector<double>>& punkty, double x) {
    double wynik = 0;
    for (int i = 0; i < punkty[1].size(); ++i) {
        double licznik = 1;
        double mianownik = 1;
        for (int j = 0; j < punkty[1].size(); ++j) {
            if (i == j) continue;
            licznik *= x - punkty[0][j];  // x values
            mianownik *= punkty[0][i] - punkty[0][j];  // x values
        }
        wynik += punkty[1][i] * licznik / mianownik;  // y values
    }
    return wynik;
}

int main()
{
    vector<vector<double>> PunktyJednorodne_A(2,vector<double>(N)), PunktyJednorodne_B(2,vector<double>(N));
    vector<vector<double>> ForPolynomial_A(2,vector<double>(ACC)),ForPolynomial_B(2,vector<double>(ACC));

    PunktyJednorodne_A = Jednorodne(N);
    PunktyJednorodne_B = Cosinusoid(N);
    ForPolynomial_A = Jednorodne(ACC);
    ForPolynomial_B = Cosinusoid(ACC);

    for (int i = 0; i < PunktyJednorodne_A[1].size(); i++)
    {
        PunktyJednorodne_A[1][i] = funkcja(PunktyJednorodne_A[0][i]);
        PunktyJednorodne_B[1][i] = funkcja(PunktyJednorodne_B[0][i]);

    }

    for (int i = 0; i < ForPolynomial_A[1].size(); i++)
    {
        ForPolynomial_A[1][i] = funkcja(ForPolynomial_A[0][i]);
        ForPolynomial_A[1][i] = lagrangeInterpolation(PunktyJednorodne_A,ForPolynomial_A[0][i]);

        ForPolynomial_B[1][i] = funkcja(ForPolynomial_B[0][i]);
        ForPolynomial_B[1][i] = lagrangeInterpolation(PunktyJednorodne_B,ForPolynomial_B[0][i]);
    }

    Gnuplot wykresA, wykresB;

    wykresA << "set terminal eps\n";
    wykresA << "set output 'wyniki_a_"<< N <<".eps'\n";
    wykresA << "set title 'Interpolacja Lagrange dla n = "<< N <<" punktów' font 'Arial,15'\n";
    wykresA << "set grid\n";
    wykresA << "set xlabel 'x' font 'Arial,13'\n";
    wykresA << "set ylabel 'f(x)' font 'Arial,13'\n";
    wykresA << "set xrange [-1:1]\n";
    wykresA << "set yrange [-1:2]\n";
    wykresA << "set key font 'Arial Bold,12'\n";
    wykresA << "plot 1/(1+50*x**2) title 'f(x)' linewidth 4 dashtype 2 linecolor rgb '#8c8c8c',"
        << "'-' with lines title 'Interpolacja Lagrange' linewidth 4 linecolor rgb '#56B4E9',"
       << "'-' title 'punkty' pointtype 7 pointsize 0.5 linecolor rgb 'black'\n";
    wykresA.send(ForPolynomial_A);
    wykresA.send(PunktyJednorodne_A);

    wykresB << "set terminal eps\n";
    wykresB << "set output 'wyniki_b_"<< N <<".eps'\n";
    wykresB << "set title 'Interpolacja Lagrange dla n = "<< N <<" punktów' font 'Arial,15'\n";
    wykresB << "set grid\n";
    wykresB << "set xlabel 'x' font 'Arial,13'\n";
    wykresB << "set ylabel 'f(x)' font 'Arial,13'\n";
    wykresB << "set xrange [-1:1]\n";
    wykresB << "set yrange [-1:2]\n";
    wykresB << "set key font 'Arial Bold,12'\n";
    wykresB << "plot 1/(1+50*x**2) title 'f(x)' linewidth 4 dashtype 2 linecolor rgb '#8c8c8c',"
        << "'-' with lines title 'Interpolacja Lagrange' linewidth 4 linecolor rgb 'purple',"
       << "'-' title 'punkty' pointtype 7 pointsize 0.5 linecolor rgb 'black'\n";
    wykresB.send(ForPolynomial_B);
    wykresB.send(PunktyJednorodne_B);

    return 0;
}