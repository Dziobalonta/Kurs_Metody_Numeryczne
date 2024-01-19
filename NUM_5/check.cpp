#include <iostream>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

const int N = 124;

using std::cout;
using std::endl;

int main()
{
    gsl_matrix* A = gsl_matrix_alloc(N,N);
    gsl_vector* b = gsl_vector_alloc(N);
    gsl_permutation* p = gsl_permutation_alloc(N);
    int znak;

    //Inicjalizacja macierzy A i wektora b
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            double wartosc;

            if (i == j) wartosc = 3;
            else if(i+1 == j || i == j+1) wartosc = 1;
            else if(i+2 == j || i == j+2) wartosc = 0.15;
            else wartosc = 0;

            gsl_matrix_set(A,i,j,wartosc);
            //cout << gsl_matrix_get(A,i,j) << " ";
        }
        //cout << endl;
        gsl_vector_set(b,i,i+1);
        //cout << gsl_vector_get(b,i) << " ";
    }
    gsl_vector* x = gsl_vector_alloc(N);

    gsl_linalg_LU_decomp(A,p,&znak);
    gsl_linalg_LU_solve(A,p,b,x);

    for (int i = 0; i < N; i++)
    {
        cout << gsl_vector_get(x,i) << " ";
    }

    gsl_vector_free(b);
    gsl_vector_free(x);
    gsl_matrix_free(A);
    gsl_permutation_free(p);
    return 0;
}