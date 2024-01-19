#include <iostream>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_eigen.h>
#include <vector>
#include "gnuplot-iostream.h"

const int N = 10000;
const double epsilon = 1e-6;

using std::cout;
using std::endl;
using std::vector;

void matrix_print(gsl_matrix* macierz)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << gsl_matrix_get(macierz,i,j) << " ";
        }
        cout << endl;
    }
}

void vector_print(gsl_vector* wektor)
{

    for (int i = 0; i < 4; i++)
    {
        cout << gsl_vector_get(wektor,i) << " ";
    }
    cout << endl;
}

int main()
{
    gsl_matrix* A = gsl_matrix_alloc(4,4);
    gsl_matrix* Q = gsl_matrix_alloc(4,4);
    gsl_matrix* R = gsl_matrix_alloc(4,4);
    gsl_vector* tau = gsl_vector_alloc(4);
    double macierzA[] = {3, 6, 6, 9, 1, 4, 0, 9, 0, 0.2, 6, 12, 0, 0, 0.1, 6};

    //Inicjalizacja macierzy A
    gsl_matrix_view Aview = gsl_matrix_view_array(macierzA,4,4);
    gsl_matrix_memcpy(A, &Aview.matrix);
    double p = 0.0, prev_value;
    vector<double> Blad_dokladnosci; 
    for (int i = 0; i < N; i++)
    {
        //QR decomp
        gsl_linalg_QR_decomp(A,tau);

        //Zapisanie wynikow do osobnych macierzy
        gsl_linalg_QR_unpack(A, tau, Q, R);

        // matrix_print(Q);
        // cout << endl;
        // matrix_print(R);
        // cout << endl;

        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, R, Q, 0.0, A);
        
        p = fabs(gsl_matrix_get(A,0,0) - prev_value);
        if ( p < epsilon)  break; // konczy dzialanie wczesniej jesli osiagnie dokladnosc zadana

        //matrix_print(wektory_wlasne);
        //cout << endl;
        
        Blad_dokladnosci.push_back(p);
        prev_value = p;
        prev_value = gsl_matrix_get(A,0,0);
    }

    cout << "Wartosci wlasne: " << endl;
    for (int i = 3; i >= 0; i--)
    {
        cout << gsl_matrix_get(A,i,i) << " ";
    } cout << endl;

    Gnuplot wykres;

    // wykres << "set terminal eps\n";
    // wykres << "set output 'wyniki_a.eps'\n";
    wykres << "set title 'Wartość błędu względem wykonanych iteracji ' font 'Arial,15'\n";
    wykres << "set xlabel 'Ilość iteracji' font 'Arial,13'\n";
    wykres << "set ylabel 'Wartość błędu bezwzględnego' font 'Arial,13'\n"; // |x^k - x^{k-1}| w skali logarytmicznej

    wykres << "set logscale y\n";
    wykres << "set key font 'Arial Bold,12'\n";
    wykres << "plot '-' with lines title 'Różnica pomiędzy kolejnymi iteracjami'\n";
    wykres.send(Blad_dokladnosci);

    gsl_matrix_free(A);
    gsl_matrix_free(Q);
    gsl_matrix_free(R);
    gsl_vector_free(tau);
    return 0;
}