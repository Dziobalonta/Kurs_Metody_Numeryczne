#include <iostream>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_eigen.h>

using std::cout;
using std::endl;

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

void vector_print_complex(gsl_vector_complex* wektor)
{
    for (int i = 0; i < 4; ++i) {
        gsl_complex z = gsl_vector_complex_get(wektor, i);
        cout << GSL_REAL(z) << " ";
    }
    cout << endl;
}

void matrix_print_complex(gsl_matrix_complex* macierz)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gsl_complex z = gsl_matrix_complex_get(macierz,i,j);
            cout << GSL_REAL(z) << "   ";
        }
        cout << endl;
    }
}

int main()
{
    gsl_matrix* A = gsl_matrix_alloc(4,4);
    gsl_vector* wartosci_wlasne = gsl_vector_alloc(4);
    gsl_matrix* wektory_wlasne = gsl_matrix_alloc(4, 4);
    double macierzA[] = {3, 4, 2, 4, 4, 7, 1, -3, 2, 1, 3, 2, 4, -3, 2, 2};

    //Inicjalizacja macierzy A
    gsl_matrix_view Aview = gsl_matrix_view_array(macierzA,4,4);
    gsl_matrix_memcpy(A, &Aview.matrix);

    //matrix_print(A);
    
    //Inicjalizacja workspace'u do wyliczenia wartosci wlasnych i wektorow
    gsl_eigen_symmv_workspace* workspace = gsl_eigen_symmv_alloc(4);

    //wyliczenie wartosci wlasnych i wektorow
    gsl_eigen_symmv(A,wartosci_wlasne, wektory_wlasne, workspace);
    gsl_eigen_symmv_sort(wartosci_wlasne,wektory_wlasne,GSL_EIGEN_SORT_ABS_ASC);

    //wartosci wlasne
    cout << "Wartosci wlasne: " << endl;
    vector_print(wartosci_wlasne);

    //wektory wlasne
    cout << "Wektory wlasne: " << endl;
    matrix_print(wektory_wlasne);

    gsl_eigen_symmv_free(workspace);
    gsl_vector_free(wartosci_wlasne);
    gsl_matrix_free(wektory_wlasne);
    gsl_matrix_free(A);
    return 0;
}