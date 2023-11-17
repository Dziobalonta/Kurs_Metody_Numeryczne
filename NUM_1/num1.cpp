//	g++ -o num1 num1.cpp -I/path/to/boost/include -L/path/to/boost/lib -lboost_iostreams
//	./num1
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "gnuplot-iostream.h"

using std::ofstream;
using std::vector;
using std::endl;
using std::system;
using std::abs;
using std::cout;

double Funkcja(double x)
{
	return sin(pow(x,2));
}

double Pochodna(double x)
{
	return 2*x*cos(pow(x,2)); 
}

//Wzor dla podpunktu A DOUBLE
double WzorD_A(double (*f)(double), double x, double h) 
{
    double value = (f(x + h) - f(x)) / h;
	return value;
}

//Wzor dla podpunktu B DOUBLE
double WzorD_B(double (*f)(double), double x, double h) 
{
    double value = (f(x + h) - f(x - h)) / (2 * h);
	return value;
}

float FunkcjaF(float x)
{
	return sin(pow(x,2));
}

float PochodnaF(float x)
{
	return 2*x*cos(pow(x,2)); 
}

//Wzor dla podpunktu A FLOAT
float WzorF_A(float (*f)(float), float x, float h) 
{
    float value = (f(x + h) - f(x)) / h;
	return value;
}

//Wzor dla podpunktu B FLOAT
float WzorF_B(float (*f)(float), float x, float h) 
{
    float value = (f(x + h) - f(x - h)) / (2 * h);
	return value;
}

vector<double> Blad_przyblizenia(double (*Wzor)(double (*)(double), double, double))
{
	vector<double> tablica_bledow;
    double e=-56;
    while(e<0)
	{
		double h = pow(2,e);
		double blad = abs(Pochodna(0.2) - Wzor(Funkcja, 0.2, h));
		tablica_bledow.push_back(blad);
        e = e + 0.5;
	}
	return tablica_bledow;
}

vector<float> Blad_przyblizenia_F(float (*Wzor)(float (*)(float), float, float))
{
	vector<float> tablica_bledow;
    float e=-56;
    while(e<35)
	{
		float h = pow(2,e);
		float blad = abs(PochodnaF(0.2) - Wzor(FunkcjaF, 0.2, h));
		tablica_bledow.push_back(blad);
        e = e + 1;
	}
	return tablica_bledow;
}

int main() 
{
	vector<double> wynikiA_D = Blad_przyblizenia(WzorD_A);
    vector<float> wynikiA_F = Blad_przyblizenia_F(WzorF_A);

    vector<double> wynikiB_D = Blad_przyblizenia(WzorD_B);
    vector<float> wynikiB_F = Blad_przyblizenia_F(WzorF_B);

	
	Gnuplot wykresA, wykresB;

    // wykresA << "set terminal png size 1200,700\n";
    // wykresA << "set output 'wyniki_A.png'\n";
    wykresA << "set title 'Przybliżenie pochodnej funkcji f(x) = sin(x^2) dla wzoru A)' font 'Arial,15'\n";
    wykresA << "set xlabel 'h (2^{x})' font 'Arial,13'\n";
    wykresA << "set ylabel 'skala logarytmiczna e' font 'Arial,13'\n";
    wykresA << "set logscale y\n";
    wykresA << "set key font 'Arial Bold,12'\n";
    wykresA << "set key enhanced\n";
    wykresA << "plot '-' with lines title 'double',"
        << "'-' with lines title 'float'\n";  
    wykresA.send(wynikiA_D);
    wykresA.send(wynikiA_F);

    // wykresB << "set terminal png size 1200,700\n";
    // wykresB << "set output 'wyniki_B.png'\n";
    wykresB << "set title 'Przybliżenie pochodnej funkcji f(x) = sin(x^2) dla wzoru B)' font 'Arial,15'\n";
    wykresB << "set xlabel 'h (2^{x})' font 'Arial,13'\n";
    wykresB << "set ylabel 'skala logarytmiczna e' font 'Arial,13'\n";
    wykresB << "set logscale y\n";
    wykresB << "set key font 'Arial Bold,12'\n";
    wykresB << "set key enhanced\n";  
    wykresB << "plot '-' with lines title 'double',"
        << "'-' with lines title 'float'\n";  
    wykresB.send(wynikiB_D);
    wykresB.send(wynikiB_F);

    return 0;
}
