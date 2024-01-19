#include <iostream>
#include <algorithm>
#include <vector>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include "gnuplot-iostream.h"

const int N = 124;
const double ACCURACY = 1.0e-8;

using std::cout;
using std::endl;
using std::vector;

double getA(int i, int j)
{
    if (i == j) return 3;
    else if (std::abs(i - j) == 1) return 1;
    else if (std::abs(i - j) == 2) return 0.15;
    return 0.0;
}

double getb(int i)
{
    return  i+1;
}

bool Jacobi(double wektorx[], double norma, vector<double>* Blad)
{
    for (int j = 0; j < 100; j++)
    {
        double nowy_wektorx[N];
        for (int i = 0; i < N; i++)
        {
            double suma = 0.0;
            if (i > 0) suma += getA(i, i - 1) * wektorx[i - 1];
            if (i > 1) suma += getA(i, i - 2) * wektorx[i - 2];
            if (i < N - 1) suma += getA(i, i + 1) * wektorx[i + 1];
            if (i < N - 2) suma += getA(i, i + 2) * wektorx[i + 2];

            nowy_wektorx[i] = (getb(i) - suma) / getA(i,i);
            //cout << nowy_wektorx[i] << "    " << wektorx[i] << endl;
        }
        //cout << endl;
        double nowa_norma = 0;

        // Oblicza normę wektora
        gsl_vector_view gsl_view = gsl_vector_view_array(wektorx, N);
        gsl_vector_view gsl_view_new = gsl_vector_view_array(nowy_wektorx, N);
        gsl_vector_sub(&gsl_view.vector, &gsl_view_new.vector);
        nowa_norma = gsl_blas_dnrm2(&gsl_view_new.vector);
        //cout << "Norma: " << nowa_norma << endl << "Accuracy: " << std::abs(nowa_norma - norma) << endl << endl;
        Blad->push_back(std::abs(nowa_norma - norma));

        if(std::abs(nowa_norma - norma) < ACCURACY) 
        {
            cout << "Jacobi: " << endl << "Liczba iteracji: " << j << endl;
            for(auto element : nowy_wektorx) cout << element << " ";
            cout << endl;
            return true;

        }

        for (int i = 0; i < N; i++) wektorx[i] = nowy_wektorx[i];
        norma = nowa_norma;
    }
    return false;
}

bool Gauss(double wektorx[], double norma, vector<double>* Blad)
{
    for (int j = 0; j < 100; j++)
    {
        double nowy_wektorx[N];
        for (int i = 0; i < N; i++)
        {
            double suma1 = 0.0, suma2 = 0.0;
            if (i > 0) suma1 += getA(i, i - 1) * nowy_wektorx[i - 1];
            if (i > 1) suma1 += getA(i, i - 2) * nowy_wektorx[i - 2];
            if (i < N - 1) suma2 += getA(i, i + 1) * wektorx[i + 1];
            if (i < N - 2) suma2 += getA(i, i + 2) * wektorx[i + 2];

            nowy_wektorx[i] = (getb(i) - suma1 - suma2) / getA(i,i);
            //cout << nowy_wektorx[i] << "    " << wektorx[i] << endl;
        }
        //cout << endl;
        double nowa_norma = 0;

        // Oblicza normę wektora
        gsl_vector_view gsl_view = gsl_vector_view_array(wektorx, N);
        gsl_vector_view gsl_view_new = gsl_vector_view_array(nowy_wektorx, N);
        gsl_vector_sub(&gsl_view.vector, &gsl_view_new.vector);
        nowa_norma = gsl_blas_dnrm2(&gsl_view_new.vector);
        //cout << "Norma: " << nowa_norma << endl << "Accuracy: " << std::abs(nowa_norma - norma) << endl << endl;
        Blad->push_back(std::abs(nowa_norma - norma));

        if(std::abs(nowa_norma - norma) < ACCURACY) 
        {
            cout << "Gauss: " << endl << "Liczba iteracji: " << j << endl;
            for(auto element : nowy_wektorx) cout << element << " ";
            cout << endl;
            return true;

        }

        for (int i = 0; i < N; i++) wektorx[i] = nowy_wektorx[i];
        norma = nowa_norma;
    }
    return false;
}

int main()
{
    double wektorx[N];
    double norma = 0;
    vector<double> Blad_Jacobi, Blad_Gauss;
    std::fill(wektorx, wektorx + N, 0);
    // for (int i = 0; i < N; i++)
    // {
    //     for (int j = 0; j < N; j++)
    //     {
    //         cout << getA(i,j) << " " ;
    //     }
    //     cout << endl;
    // }

    if (Jacobi(wektorx,norma,&Blad_Jacobi) == false) cout << "Algorytm potrzebuje wiecej niz 100 powtorzen aby uzyskac wynik!\n";
    if (Gauss(wektorx,norma,&Blad_Gauss) == false) cout << "Algorytm potrzebuje wiecej niz 100 powtorzen aby uzyskac wynik!\n";

    // for(auto val: Blad_Jacobi){
    //     cout << val << endl;
    // }
    // cout << endl;

    // for(auto val: Blad_Gauss){
    //     cout << val << endl;
    // }
    
    Gnuplot wykres;

    // wykres << "set terminal eps\n";
    // wykres << "set output 'wyniki.eps'\n";
    wykres << "set title 'Wartość błędu względem rozmiaru danych N = " << N <<"' font 'Arial,15'\n";
    wykres << "set xlabel 'Ilość iteracji' font 'Arial,13'\n";
    wykres << "set ylabel 'Wartość błędu bezwzględnego' font 'Arial,13'\n"; // |x^k - x^{k-1}| w skali logarytmicznej

    wykres << "set logscale y\n";
    wykres << "set key font 'Arial Bold,12'\n";
    wykres << "plot '-' with lines title 'Metoda Jacobiego',"
        << "'-' with lines title 'Metoda Gaussa-Seidela'\n";
    wykres.send(Blad_Jacobi);
    wykres.send(Blad_Gauss);

    return 0;
}
