// g++ -o num2.o num2.cpp -lgsl -lgslcblas
// ./num2.o
#include <iostream>
#include <random>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>

using std::cout;
using std::endl;

void matrix_fill(gsl_matrix* macierz, int rows, int cols, double data[][5])
{
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            gsl_matrix_set(macierz, row, col, data[row][col]);
        }
    }
}

void matrix_print(gsl_matrix* macierz, int rows,int cols)
{
    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            double value = gsl_matrix_get(macierz, row, col);
            printf("%.9f ", value);
        }
        printf("\n");
    }
}

void vector_fill(gsl_vector* wektor, int size,double data[])
{
    for (size_t i = 0; i < size; i++)
    {
        gsl_vector_set(wektor, i, data[i]);
    }
    

}

void vector_print(gsl_vector* wektor, int size)
{
    printf("[ ");
    for (size_t i = 0; i < size; i++)
    {
        double value = gsl_vector_get(wektor, i);
        printf("%.9f ", value);
    }
    printf("]\n\n");
}

void GenerateErrorVector(gsl_vector* wektor, int size)
{
    std::random_device rd;
    std::mt19937 gen(rd());    //generator liczb losowych
    std::uniform_real_distribution<double> dist(-1, 1);
        for (int i = 0; i < size; i++)
        {
            
            double randomValue = dist(gen) * pow(10,-6);
            gsl_vector_set(wektor, i, randomValue);
        }
}

void Solver_fixed(gsl_matrix* macierz,gsl_vector* wektor,gsl_vector* wynik, double data[][5])
{
    gsl_linalg_HH_solve(macierz, wektor, wynik);
    matrix_fill(macierz, 5, 5, data);   //HH_solve korzysta z A jako miejsca na tymczasowe wynik - trzeba nadpisac
}

int main()
{
    double dataA[5][5] = {{2.554219275 , 0.871733993 , 0.052575899 , 0.240740262 , 0.316022841},
                    {0.871733993 , 0.553460938 , -0.070921727 , 0.255463951 , 0.707334556},
                    {0.052575899 , -0.070921727 , 3.409888776 , 0.293510439 , 0.847758171},
                    {0.240740262 , 0.255463951 , 0.293510439 , 1.108336850 , -0.206925123},
                    {0.316022841 , 0.707334556 , 0.847758171 , -0.206925123 , 2.374094162}};

    double dataB[5][5] = {{2.645152285 , 0.544589368 , 0.009976745 , 0.327869824 , 0.424193304},
                    {0.544589368 , 1.730410927 , 0.082334875 , -0.057997220 , 0.318175706},
                    {0.009976745 , 0.082334875 , 3.429845092 , 0.252693077 , 0.797083832},
                    {0.327869824 , -0.057997220 , 0.252693077 , 1.191822050 , -0.103279098},
                    {0.424193304 , 0.318175706 , 0.797083832 , -0.103279098 , 2.502769647}};

    double datab[5] = {-0.642912346 , -1.408195475 , 4.595622394 , -5.073473196 , 2.178020609};

    gsl_matrix* macierzA = gsl_matrix_alloc(5,5);
    gsl_matrix* macierzB = gsl_matrix_alloc(5,5);
    gsl_vector* wektorb = gsl_vector_alloc(5);

    matrix_fill(macierzA, 5, 5, dataA);
    matrix_fill(macierzB, 5, 5, dataB);
    vector_fill(wektorb,5,datab);

    gsl_vector* Randomowy = gsl_vector_alloc(5);
    GenerateErrorVector(Randomowy,5);

    gsl_vector* wynikA = gsl_vector_alloc(5);
    Solver_fixed(macierzA, wektorb, wynikA, dataA);
    cout << "\t\t\t=== DLA MACIERZY A₁ ===\nA₁ * y = b\n\ny = ";
    vector_print(wynikA,5);

    gsl_vector* wynikA_Zaburzony = gsl_vector_alloc(5);
    gsl_vector_add(wektorb, Randomowy);    // zaburzenie wektora b randomowymi malymi wartosciami
    
    Solver_fixed(macierzA, wektorb, wynikA_Zaburzony, dataA);
    cout << "A₁ * y = (b + △ b)\n\ny = ";
    vector_print(wynikA_Zaburzony,5);

    vector_fill(wektorb,5,datab); //przywrocenie defaultowych wartości
    gsl_vector* wynikB = gsl_vector_alloc(5);
    Solver_fixed(macierzB, wektorb, wynikB, dataB);
    cout << "\t\t\t=== DLA MACIERZY A₂ ===\nA₂ * y = b\n\ny = ";
    vector_print(wynikB,5);

    gsl_vector* wynikB_Zaburzony = gsl_vector_alloc(5);
    gsl_vector_add(wektorb, Randomowy);    // zaburzenie wektora b randomowymi malymi wartosciami
    
    Solver_fixed(macierzB, wektorb, wynikB_Zaburzony,dataB);
    cout << "A₂ * y = (b + △ b)\n\ny = ";
    vector_print(wynikB_Zaburzony,5);
    
    gsl_matrix_free(macierzA);
    gsl_matrix_free(macierzB);
    gsl_vector_free(wektorb);
    gsl_vector_free(wynikA);
    gsl_vector_free(wynikB);
    gsl_vector_free(Randomowy);
    gsl_vector_free(wynikA_Zaburzony);
    gsl_vector_free(wynikB_Zaburzony);
    return 0;
}