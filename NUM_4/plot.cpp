#include <iostream>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <ctime>
#include "gnuplot-iostream.h"

// const int N = 750;
// const int repeat = 10;

using std::vector;
using std::cout;
using std::endl;
using std::atoi;

void matrix_print(int iter, gsl_matrix* macierz)
{
    for (int i = 0; i < iter; i++)
    {
        for (int j = 0; j < iter; j++)
        {
            cout << gsl_matrix_get(macierz,i,j) << " ";
        }
        cout << endl;
    }
}

void vector_print(int iter, gsl_vector* wektor)
{

    for (int i = 0; i < iter; i++)
    {
        cout << gsl_vector_get(wektor,i) << " ";
    }
    cout << endl;
}

void Sherman_Morris_solve(int iter,gsl_vector* uv,gsl_vector* q,gsl_vector* z, gsl_vector* y)
{
//v^T * z -> vec tansp * vec = scalar
    double skalar = 0;
    gsl_blas_ddot(uv, z, &skalar);

    //q * (v^T *z) -> vec * scalar = vector
    gsl_vector* q_modified = gsl_vector_alloc(iter);
    gsl_vector_memcpy(q_modified,q);
    gsl_vector_scale(q_modified,skalar);

    //v^T * q -> vec tansp * vec  = scalar
    double dzielenie = 0;
    gsl_blas_ddot(uv, q, &dzielenie);
    dzielenie++;


    //((q*u^T) * z)/(1 + v^T * q) -> vec  / scal  = vec
    gsl_vector_scale(q_modified,1/dzielenie);

    gsl_vector_sub(z, q_modified);
    gsl_vector_memcpy(y,z);

    gsl_vector_free(q_modified);
}

void Back_substitution(int iter, gsl_matrix* macierz,gsl_vector* q,gsl_vector* z,gsl_vector* uv,gsl_vector* b)
{
    for (int i = iter-1; i >= 0; i--)
    {
        //Rozwiaznie Bq=u
        double poprzedni = 0;
        if (i < iter-1)
        {
            poprzedni = gsl_matrix_get(macierz,i,i+1) * gsl_vector_get(q,i+1);
        }
        double value = (gsl_vector_get(uv,i) - poprzedni) / gsl_matrix_get(macierz,i,i);
        gsl_vector_set(q,i,value);

        poprzedni = 0;
        //Rozwiaznie Bz=b
        if (i < iter-1)
        {
            poprzedni = gsl_matrix_get(macierz,i,i+1) * gsl_vector_get(z,i+1);
        }
        value = (gsl_vector_get(b,i) - poprzedni) / gsl_matrix_get(macierz,i,i);
        gsl_vector_set(z,i,value);
    }
}

int main(int argc, char* argv[])
{
    int N = atoi(argv[1]);
    int repeat = atoi(argv[2]);
    vector<double> Czasy;
    for (int k = 1; k <= N; k++)
    {
        clock_t start, stop;
        double srednia = 0;
        for (int m = 0; m < repeat; m++)
        {
            gsl_matrix* B = gsl_matrix_alloc(k,k);
            gsl_vector* b = gsl_vector_alloc(k);
            gsl_vector* uv = gsl_vector_alloc(k);
            gsl_vector* z = gsl_vector_alloc(k);
            gsl_vector* q = gsl_vector_alloc(k);

            //Inicjalizacja macierzy B i wektora u/v
            gsl_matrix_set_all(B,0);
            for (int i = 0; i < k; i++)
            {
                gsl_matrix_set(B,i,i,11);
                if(i < k-1) gsl_matrix_set(B,i,i+1,7);
                
                gsl_vector_set(b,i,5);
                gsl_vector_set(uv,i,1);
            }

            start = clock();

            //Back substitution 
            //Bq=u oraz Bz=b
            Back_substitution(k,B,q,z,uv,b);

            //zastosowanie wzoru Shermana - Morrisona
            gsl_vector* y = gsl_vector_alloc(k);
            Sherman_Morris_solve(k,uv,q,z,y);

            stop = clock();
            double czas_wykonania = double(stop - start) / double(CLOCKS_PER_SEC);
            srednia += czas_wykonania;

            gsl_matrix_free(B);
            gsl_vector_free(b);
            gsl_vector_free(uv);
            gsl_vector_free(q);
            gsl_vector_free(z);
            gsl_vector_free(y);
        }
        srednia /= repeat;
        Czasy.push_back(srednia);
        //vector_print(k,y);
        cout << "Rozmiar: " << k+1 << " " << "Czas wykonania SM: " << srednia << endl;
    }

    Gnuplot wykres;

    // wykres << "set terminal eps\n";
    // wykres << "set output 'wyniki.eps'\n";
    wykres << "set title 'Czas wykonania operacji względem rozmiaru danych N = " << N <<"' font 'Arial,15'\n";
    wykres << "set xlabel 'N - rozmiar' font 'Arial,13'\n";
    wykres << "set ylabel 'uśredniony czas wykonania " << repeat << " operacji[s]' font 'Arial,13'\n";
    wykres << "set key font 'Arial Bold,12'\n";
    wykres << "plot '-' with lines title 'Algorytm Shermana-Morrisa'\n";
    wykres.send(Czasy);

   return 0;
}