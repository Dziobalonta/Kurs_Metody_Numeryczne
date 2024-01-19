#include <iostream>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

using std::cout;
using std::endl;

int main() {
    const int N = 124;

    // Alokowanie pamieci
    gsl_matrix* A = gsl_matrix_alloc(N, N);
    gsl_vector* y = gsl_vector_alloc(N);
    gsl_vector* x = gsl_vector_alloc(N);

    // Inicjalizacja macierzy A i wektora x
    for (int i = 0; i < N; ++i) 
    {
        for (int j = 0; j < N; ++j) 
        {
            double val = 0.0;
            if (i == j) val = 1.2;
            else if (i == j + 1) val = 0.2;
            else if (i + 1 == j) val = 0.1 / (i + 1);
            else if (i + 2 == j) val = 0.15 / ((i + 1) * (i + 1));
            
            gsl_matrix_set(A, i, j, val);
        }
        gsl_vector_set(x, i, i + 1);
    }

    // Rozklad LU
    gsl_permutation* permutacja = gsl_permutation_alloc(N);
    int znakp;
    gsl_linalg_LU_decomp(A, permutacja, &znakp);

    // cout << "Rozklad LU:" << endl;
    // for (int i = 0; i < N; ++i)
    // {
    //     if (i < N-1) cout << gsl_matrix_get(A,i+1,i) << "\t";
    //     else cout << "0\t";
     
    //     cout << gsl_matrix_get(A,i,i) << "\t"; 

    //     if (i < N-1) cout << gsl_matrix_get(A,i,i+1) << "\t";
    //     else cout << "0\t";

    //     if (i < N-2) cout << gsl_matrix_get(A,i,i+2) << endl;
    //     else cout << "0\n";
    // }
        

    // Wyznacznik Macierzy A
    double det = gsl_linalg_LU_det(A, znakp);

    // Ay = x
    gsl_linalg_LU_solve(A, permutacja, x, y);

    cout << "Wektor y:" << endl;
    for (int i = 0; i < N; ++i) cout << gsl_vector_get(y, i) << " ";
    
    cout << "\n\nWyznacznik macierzy A:\n" << det << endl;

    // Zwolnienie pamieci
    gsl_matrix_free(A);
    gsl_vector_free(y);
    gsl_vector_free(x);
    gsl_permutation_free(permutacja);

    return 0;
}
