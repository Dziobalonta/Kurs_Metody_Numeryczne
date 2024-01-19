#include <iostream>
#include "gnuplot-iostream.h"
#include <vector>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_linalg.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;

const int N = 100;

gsl_matrix* Fill_matrix(gsl_matrix* A,vector<vector<double>> data)
{
    int k;
    for (int i = 0; i < N; i++)
    {
        gsl_matrix_set(A,i,0,pow(data[0][i],2));
        gsl_matrix_set(A,i,1,sin(data[0][i]));
        gsl_matrix_set(A,i,2,cos(5 * data[0][i]));
        gsl_matrix_set(A,i,3,exp(-1 * data[0][i]));
        
    }
    
    return A;
}

gsl_vector* Fill_vector(gsl_vector* y,vector<vector<double>> data)
{
    for (int i = 0; i < N; i++)
    {
        gsl_vector_set(y,i,data[1][i]);
        //cout << gsl_vector_get(y,i) << endl;
    }
    
    return y;
}

void vector_Print(gsl_vector* y)
{
    for (int i = 0; i < 4; i++)
    {
        cout << gsl_vector_get(y,i) << endl;
    }
    
}

void matrix_Print(gsl_matrix* A)
{
    for (int i = 0; i < N; i++)
    {
       for (int j = 0; j < 4; j++)
       {
          cout << gsl_matrix_get(A,i,j) << " ";
       }
       cout << endl;
    }
    
}

int main()
{
    vector<vector<double>> data(2, vector<double>(N, 0.0));
    ifstream plik("data2023.dat");
    gsl_matrix* A = gsl_matrix_alloc(N,4);
    gsl_vector* p = gsl_vector_alloc(4);
    gsl_vector* y_tylda = gsl_vector_alloc(N);
    gsl_vector* residual = gsl_vector_alloc(N);
    gsl_vector_set_all(p,0);

    double x,y;
    for (int j = 0; j < N; ++j) {
        plik >> x >> y;
        data[0][j] = x;
        data[1][j] = y;
        //cout << data[0][j]<< " " << data[1][j] << endl;
    }

    A = Fill_matrix(A,data);
    //matrix_Print(A);
    y_tylda = Fill_vector(y_tylda,data);

    gsl_vector *tau = gsl_vector_alloc(4);
    gsl_linalg_QR_decomp(A, tau);

    // Solve for x using the QR decomposition - overdetermined system of equations
    gsl_linalg_QR_lssolve(A, tau, y_tylda, p,residual);

    cout << "Wspolczynniki: " << endl;
    cout << "a = " << gsl_vector_get(p,0) << endl;
    cout << "b = " << gsl_vector_get(p,1) << endl;
    cout << "c = " << gsl_vector_get(p,2) << endl;
    cout << "d = " << gsl_vector_get(p,3) << endl;

    Gnuplot wykres;
    // wykres << "set terminal eps\n";
    // wykres << "set output 'wyniki_a.eps'\n";
    wykres << "set title 'Aproksymacja funkcji na podstawie punktów z pliku data2023.dat' font 'Arial,15'\n";
    wykres << "set grid\n";
    wykres << "set xlabel 'x' font 'Arial,13'\n";
    wykres << "set ylabel 'wartość nieznanej funkcji' font 'Arial,13'\n";
    wykres << "set key font 'Arial Bold,12'\n";
    wykres << "a=" << gsl_vector_get(p,0) << "\n";
    wykres << "b=" << gsl_vector_get(p,1) << "\n";
    wykres << "c=" << gsl_vector_get(p,2) << "\n";
    wykres << "d=" << gsl_vector_get(p,3) << "\n";
    wykres << "plot a*x**2 + b*sin(x) + c*cos(5*x) + d*exp(-x) title 'Uzyskana funkcja',"
        << "'-' title 'data2023.dat' pointsize 0.5\n";
    wykres.send(data);

    gsl_matrix_free(A);
    gsl_vector_free(tau);
    gsl_vector_free(y_tylda);
    gsl_vector_free(p);
    gsl_vector_free(residual);
    plik.close();
    return 0;
}