#include <iostream>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>
#include "gnuplot-iostream.h"

using std::vector;
using std::cout;
using std::endl;

//const int N = 124;

int main(int argc,char* argv[])
{
    int N = std::atoi(argv[1]);
    vector<double> Czasy_wykonania;
    for (int j = 50; j <= N; j+=50)
    {    
        //cout << j << " ";
        double srednia = 0;
        for (int k = 1; k <= 100; k++)
        {
            vector<double> DwaNad(j); // N-2, reszta zera
            vector<double> JedenNad(j); // N-1, reszta zera
            vector<double> Diagonala(j); 
            vector<double> JedenPod(j); // N-1, reszta zera
            clock_t start, stop;

            // Inicjalizacja macierzy A
            for (int i = 0; i < j; ++i) 
            {
                // diagonala
                Diagonala[i] = 1.2;
                
                if (i < j-1) 
                {
                    //jeden pod diagonala ^
                    JedenPod[i] = 0.2;
                    //jeden nad diagonala v
                    JedenNad[i] = 0.1 / (i+1);
                }else
                {
                    JedenPod[i] = 0;
                    JedenNad[i] = 0;
                }

                if (i < j-2) 
                {
                    // dwa nad diagonala
                    DwaNad[i] = 0.15 / pow(i+1,2);
                }else DwaNad[i] = 0;
            }

            start = clock();


            //Rozklad LU macierzy
            for (int i = 0; i < j; i++)
            {
                //jeden nad diagonala ^
                if (i > 0 && i < j-1)
                {
                    JedenNad[i] -= JedenPod[i-1] * DwaNad[i-1];
                }

                //diagonala
                if (i > 0)
                {
                    Diagonala[i] -= JedenPod[i-1] * JedenNad[i-1]; 
                }

                //jeden pod diagonala v
                if (i < j-1)
                {
                    JedenPod[i] /= Diagonala[i];
                }
            }

            //rozwiaznie rownania  Lt = x dla t
            vector<double> wektorT(N,0.0);
            //cout << "Wektor t:" << endl;
            for (int i = 0; i < j; i++)
            {
                double poprzedni = 0;
                if(i > 0)
                {
                    poprzedni += JedenPod[i-1] * wektorT[i-1];
                }
                wektorT[i] = (i+1 - poprzedni);
                //cout << wektorT[i] << endl;
            }

            //rozwiaznie rownania  Uy = t dla y
            vector<double> wektorY(N,0.0);
            for (int i = j-1; i >= 0; i--)
            {
                double poprzedni = 0;
                if (i < j-1)
                {
                    poprzedni += JedenNad[i] * wektorY[i+1];
                }
                if (i < j-2)
                {
                    poprzedni += DwaNad[i] * wektorY[i+2];
                }
                wektorY[i] = (wektorT[i] - poprzedni) / Diagonala[i];
            }

            stop = clock();
            double czas_wykonania = double(stop - start) / double(CLOCKS_PER_SEC);
            srednia += czas_wykonania;
        }
        srednia /= 100; 
        Czasy_wykonania.push_back(srednia);
        //printf("%e\n",Czasy_wykonania.back());
    }
    Gnuplot wykres;

    // wykres << "set terminal png size 1200,700\n";
    // wykres << "set output 'wyniki.png'\n";
    wykres << "set title 'Czas wykonania operacji względem rozmiaru danych N = " << N <<"' font 'Arial,15'\n";
    wykres << "set xlabel 'N - rozmiar' font 'Arial,13'\n";
    wykres << "set ylabel 'czas wykonania [s]' font 'Arial,13'\n";
    wykres << "set xtics (";
    int scale=0;
    for (int i = 0; i <= N/50; i += (N/50)/10) {
        wykres << "\"" << scale << "\" " << i << ", ";
        scale += N/10;
    }
    wykres << ")\n";
    wykres << "set key font 'Arial Bold,12'\n";
    wykres << "plot '-' with lines title 'data'\n";
    wykres.send(Czasy_wykonania);

    return 0;
}