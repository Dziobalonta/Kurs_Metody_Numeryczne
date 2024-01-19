#include <iostream>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <ctime>

const int N = 80;

using std::cout;
using std::endl;

void matrix_print(gsl_matrix* macierz)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << gsl_matrix_get(macierz,i,j) << " ";
        }
        cout << endl;
    }
}

void vector_print(gsl_vector* wektor)
{

    for (int i = 0; i < N; i++)
    {
        cout << gsl_vector_get(wektor,i) << " ";
    }
    cout << endl;
}

void Sherman_Morris_solve(gsl_vector* uv,gsl_vector* q,gsl_vector* z, gsl_vector* y)
{
    //v^T * z -> vec tansp * vec = scalar
    double skalar = 0;
    gsl_blas_ddot(uv, z, &skalar);

    //q * (v^T *z) -> vec * scalar = vector
    gsl_vector* q_modified = gsl_vector_alloc(N);
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

void Back_substitution(gsl_matrix* macierz,gsl_vector* q,gsl_vector* z,gsl_vector* uv,gsl_vector* b)
{
    for (int i = N-1; i >= 0; i--)
    {
        //Rozwiaznie Bq=u
        double poprzedni = 0;
        if (i < N-1)
        {
            poprzedni = gsl_matrix_get(macierz,i,i+1) * gsl_vector_get(q,i+1);
        }
        double value = (gsl_vector_get(uv,i) - poprzedni) / gsl_matrix_get(macierz,i,i);
        gsl_vector_set(q,i,value);

        poprzedni = 0;
        //Rozwiaznie Bz=b
        if (i < N-1)
        {
            poprzedni = gsl_matrix_get(macierz,i,i+1) * gsl_vector_get(z,i+1);
        }
        value = (gsl_vector_get(b,i) - poprzedni) / gsl_matrix_get(macierz,i,i);
        gsl_vector_set(z,i,value);
    }
}

int main()
{
    gsl_matrix* B = gsl_matrix_alloc(N,N);
    gsl_vector* b = gsl_vector_alloc(N);
    gsl_vector* uv = gsl_vector_alloc(N);
    clock_t start, stop;

    //Inicjalizacja macierzy B i wektora u/v
    gsl_matrix_set_all(B,0);
    for (int i = 0; i < N; i++)
    {
        gsl_matrix_set(B,i,i,11);
        if(i < N-1) gsl_matrix_set(B,i,i+1,7);
        
        gsl_vector_set(b,i,5);
        gsl_vector_set(uv,i,1);
    }

    gsl_vector* z = gsl_vector_alloc(N);
    gsl_vector* q = gsl_vector_alloc(N);

    start = clock();

    //Back substitution 
    //Bq=u oraz Bz=b
    Back_substitution(B,q,z,uv,b);

    //zastosowanie wzoru Shermana - Morrisona
    gsl_vector* y = gsl_vector_alloc(N);
    Sherman_Morris_solve(uv,q,z,y);

    stop = clock();
    double czas_wykonania = double(stop - start) / double(CLOCKS_PER_SEC);
    vector_print(y);
    cout << "Czas wykonania: " << czas_wykonania << endl;

   
    gsl_matrix_free(B);
    gsl_vector_free(b);
    gsl_vector_free(uv);
    gsl_vector_free(q);
    gsl_vector_free(z);
    gsl_vector_free(y);
   return 0;
}