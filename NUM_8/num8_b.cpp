#include <iostream>
#include "gnuplot-iostream.h"
#include <vector>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_linalg.h>
#include <random>
#include <cmath>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;

const int N = 100;
const double a = 2.137;
const double b = 0.69;
const double c = 0.5;
const double d = 0.420;


//y=2.137*sin(2*x) + 0.69*x**3 + 0.5*atan(x) + 0.42*cos(5*x)

double Fun_val(double x)
{
   return a*sin(2*x)+b*pow(x,3)+c*atan(x)+d*cos(5*x);
}

vector<vector<double>> Generate_points(vector<vector<double>> data, int lower, int upper)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dystrubucja_pkt(lower, upper);
    std::normal_distribution<> zaburzenia(0, 1);

    for (int i = 0; i < data[0].size(); ++i) 
    {
        data[0][i] = dystrubucja_pkt(gen);
        data[1][i] = Fun_val(data[0][i]) + zaburzenia(gen);
    }
    return data;
}


gsl_matrix* Fill_matrix(gsl_matrix* A,vector<vector<double>> data)
{
    int k;
    for (int i = 0; i < N; i++)
    {
        gsl_matrix_set(A,i,0,sin(2 * data[0][i]));
        gsl_matrix_set(A,i,1,pow(data[0][i],3));
        gsl_matrix_set(A,i,2,atan(data[0][i]));
        gsl_matrix_set(A,i,3,cos(5 * data[0][i]));
        
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
    data = Generate_points(data,-2,2);
    cout << "Ustalone parametry:\n a = " << 2.137 << "\n b = " << 0.69 << "\n c = " << 0.5 << "\n d = " << 0.420 << endl;


    // for (int i = 0; i < data[0].size(); i++)
    // {
    //     cout << data[0][i] << " " << data[1][i] << endl;
    // }
    

    gsl_matrix* A = gsl_matrix_alloc(N,4);
    gsl_vector* p = gsl_vector_alloc(4);
    gsl_vector* y_tylda = gsl_vector_alloc(N);
    gsl_vector* residual = gsl_vector_alloc(N);
    gsl_vector_set_all(p,0);

    A = Fill_matrix(A,data);
    //matrix_Print(A);
    y_tylda = Fill_vector(y_tylda,data);

    gsl_vector *tau = gsl_vector_alloc(4);
    gsl_linalg_QR_decomp(A, tau);

    // Solve for x using the QR decomposition - overdetermined system of equations
    gsl_linalg_QR_lssolve(A, tau, y_tylda, p, residual);

    // cout << endl;
    // vector_Print(p);

    cout << "Uzyskane parametry:\n a = " << gsl_vector_get(p,0) << "\n b = " << gsl_vector_get(p,1) << "\n c = " << gsl_vector_get(p,2) << "\n d = " << gsl_vector_get(p,3) << endl;



    Gnuplot wykres;
    // wykres << "set terminal eps\n";
    // wykres << "set output 'wyniki_b.eps'\n";
    wykres << "set title 'Aproksymacja własnej funkcji na podstawie "<< N <<" losowych punktów' font 'Arial,15'\n";
    wykres << "set grid\n";
    wykres << "set xlabel 'x' font 'Arial,13'\n";
    wykres << "set ylabel 'wartość funkcji' font 'Arial,13'\n";
    wykres << "set key font 'Arial Bold,12'\n";
    wykres << "a=" << gsl_vector_get(p,0) << "\n";
    wykres << "b=" << gsl_vector_get(p,1) << "\n";
    wykres << "c=" << gsl_vector_get(p,2) << "\n";
    wykres << "d=" << gsl_vector_get(p,3) << "\n";
    wykres << "plot a*sin(2*x) + b*x**3 + c*atan(x) + d*cos(5*x) title 'Uzyskana funkcja',"
        << "2.137*sin(2*x) + 0.69*x**3 + 0.5*atan(x) + 0.42*cos(5*x) title 'Ustalona funkcja' dashtype 2 linecolor rgb '#8c8c8c',"
        << "'-' title 'Losowe zaburzone punkty ustalonej funkcji' pointtype 2 pointsize 0.5 linecolor rgb '#009E73'\n";
     wykres.send(data);
    return 0;
}