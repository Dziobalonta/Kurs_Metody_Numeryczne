#include <iostream>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <ctime>

const int N = 80;

using std::cout;
using std::endl;

int main()
{
    gsl_matrix* A = gsl_matrix_alloc(N,N);
    gsl_vector* b = gsl_vector_alloc(N);
    gsl_permutation* p = gsl_permutation_alloc(N);
    clock_t start, stop;
    int znak;

    //Inicjalizacja macierzy A i wektora b
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            double wartosc;

            if (i == j) wartosc = 12;
            else if(i+1 == j) wartosc = 8;
            else wartosc = 1;

            gsl_matrix_set(A,i,j,wartosc);
            //cout << gsl_matrix_get(A,i,j) << " ";
        }
        //cout << endl;
        gsl_vector_set(b,i,5);
        //cout << gsl_vector_get(b,i) << " ";
    }
    gsl_vector* y = gsl_vector_alloc(N);

    start = clock();
    gsl_linalg_LU_decomp(A,p,&znak);
    gsl_linalg_LU_solve(A,p,b,y);
    stop = clock();
    double czas_wykonania = double(stop - start) / double(CLOCKS_PER_SEC);

    for (int i = 0; i < N; i++)
    {
        cout << gsl_vector_get(y,i) << " ";
    }
    cout << endl;
    cout << "Czas wykonania: " << czas_wykonania << endl;
    
    gsl_vector_free(b);
    gsl_vector_free(y);
    gsl_matrix_free(A);
    gsl_permutation_free(p);
    return 0;
}