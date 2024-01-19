#include <iostream>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_eigen.h>
#include <vector>
#include "gnuplot-iostream.h"

const int N = 10000;
const double epsilon = 1e-8;

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
    gsl_vector* tmp = gsl_vector_alloc(4);
    gsl_vector* v = gsl_vector_alloc(4);
    gsl_vector* prev_v = gsl_vector_alloc(4);
    gsl_vector_set_all(v,1);
    double macierzA[] = {3, 4, 2, 4, 4, 7, 1, -3, 2, 1, 3, 2, 4, -3, 2, 2};


    //Inicjalizacja macierzy A
    gsl_matrix_view Aview = gsl_matrix_view_array(macierzA,4,4);
    gsl_matrix_memcpy(A, &Aview.matrix);

    double prevEigen = 0.0, Eigen;
    vector<double> Bledy;
    for (int i = 0; i < N; i++)
    {
        //mnozenie Av_1 = v_2
        gsl_blas_dgemv(CblasNoTrans, 1.0, A, v, 0.0, tmp);

        Eigen = gsl_blas_dnrm2(tmp);
        gsl_vector_scale(tmp, 1/Eigen); //normalizacja przez wektora

        gsl_vector_sub(prev_v,v);
        double err = gsl_blas_dnrm2(prev_v);
        if (err < epsilon)  break; // konczy dzialanie wczesniej jesli osiagnie dokladnosc zadana
        Bledy.push_back(err);
        gsl_vector_memcpy(prev_v,v);

        gsl_vector_memcpy(v,tmp);
    }

    //wartosci wlasne
    cout << "Najwieksza wartosc wlasna: " << endl;
    cout << Eigen << endl;
    cout << "Odpowiadajacy wektor wlasny: " << endl;
    vector_print(v);

    Gnuplot wykres;

    // wykres << "set terminal eps\n";
    // wykres << "set output 'wyniki_b.eps'\n";
    wykres << "set title 'Wartość błędu względem wykonanych iteracji ' font 'Arial,15'\n";
    wykres << "set xlabel 'Ilość iteracji' font 'Arial,13'\n";
    wykres << "set ylabel 'Wartość błędu bezwzględnego' font 'Arial,13'\n"; // |x^k - x^{k-1}| w skali logarytmicznej

    wykres << "set logscale y\n";
    wykres << "set key font 'Arial Bold,12'\n";
    wykres << "plot '-' with lines title 'Różnica pomiędzy kolejnymi iteracjami'\n";
    wykres.send(Bledy);

    gsl_vector_free(tmp);
    gsl_vector_free(v);
    gsl_vector_free(prev_v);
    gsl_matrix_free(A);
    return 0;
}