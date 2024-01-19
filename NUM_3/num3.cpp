#include <iostream>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>

using std::vector;
using std::cout;
using std::endl;

const int N = 124;

int main()
{
    vector<double> DwaNad(N); // N-2, reszta zera
    vector<double> JedenNad(N); // N-1, reszta zera
    vector<double> Diagonala(N); 
    vector<double> JedenPod(N); // N-1, reszta zera
    clock_t start, stop;

    // Inicjalizacja macierzy A
    for (int i = 0; i < N; ++i) 
    {
        // diagonala
        Diagonala[i] = 1.2;
        
        if (i < N-1) 
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

        if (i < N-2) 
        {
            // dwa nad diagonala
            DwaNad[i] = 0.15 / pow(i+1,2);
        }else DwaNad[i] = 0;
    }

    // cout << "Macierz A:" << std::endl;
    // for (int i = 0; i < N; i++)
    // {
    //     cout << JedenPod[i] << "\t" << Diagonala[i] << "\t" << JedenNad[i] << "\t" << DwaNad[i] << endl;
    // }
 
    start = clock();


    //Rozklad LU macierzy
    for (int i = 0; i < N; i++)
    {
        //jeden nad diagonala ^
        if (i > 0 && i < N-1)
        {
            JedenNad[i] -= JedenPod[i-1] * DwaNad[i-1];
        }

        //diagonala
        if (i > 0)
        {
            Diagonala[i] -= JedenPod[i-1] * JedenNad[i-1]; 
        }

        //jeden pod diagonala v
        if (i < N-1)
        {
            JedenPod[i] /= Diagonala[i];
        }

    }

    // cout << "A = LU:" << std::endl;
    // for (int i = 0; i < N; i++)
    // {
    //     cout << JedenPod[i] << "\t" << Diagonala[i] << "\t" << JedenNad[i] << "\t" << DwaNad[i] << endl;
    // }

    //rozwiaznie rownania  Lt = x dla t
    vector<double> wektorT(N,0.0);
    //cout << "Wektor t:" << endl;
    for (int i = 0; i < N; i++)
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
    for (int i = N-1; i >= 0; i--)
    {
        double poprzedni = 0;
        if (i < N-1)
        {
            poprzedni += JedenNad[i] * wektorY[i+1];
        }
        if (i < N-2)
        {
            poprzedni += DwaNad[i] * wektorY[i+2];
        }
        wektorY[i] = (wektorT[i] - poprzedni) / Diagonala[i];
    }

    stop = clock();
    double czas_wykonania = double(stop - start) / double(CLOCKS_PER_SEC);
    //printf("Czas wykonania: %f sek\n",czas_wykonania);

    //obliczanie wyznacznika macierzy A
    double det = 1;
    for (int i = 0; i < N; i++)
    {
        det *= Diagonala[i];
    }
    

    cout << "Wektor y:" << std::endl;
    for (const auto& value : wektorY) 
    {
        cout << value << " ";
    }
    cout << "\n\nWyznacznik macierzy A:\n" << det << endl;

    return 0;
}