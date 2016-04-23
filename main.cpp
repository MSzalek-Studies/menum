#include <iostream>
#include <cmath>
using namespace std;
double poczatekPrzedzialu, koniecPrzedzialu;
int nrFunkcji, liczbaWezlow;
double krok;
double *x;
double *y;

typedef double (*FunkcjaInterpolacyjna)(double x);
FunkcjaInterpolacyjna wybranaFunkcja;


void wybierzZakresArgumentow()
{
    do {
        cout<<"Podaj poczatek przedzialu: ";
        cin>>poczatekPrzedzialu;
        cout<<"Podaj koniec przedzialu: ";
        cin>>koniecPrzedzialu;
    } while(poczatekPrzedzialu >= koniecPrzedzialu);
}

double horner(double wsp[],int st, double x)
{
  double wynik = wsp[0];

  for(int i=1;i<=st;i++)
    wynik = wynik*x + wsp[i];

  return wynik;
}

double funkcjaTrygonometryczna (double x) {
    return x*x-10;
}

double wielomian(double x) {
    double wspolczynniki[] = {1, -1, -2, 1}; //x^3-x^2-2x+1
    return horner(wspolczynniki, 3, x);
}

double funkcjaWykladnicza (double x) {
    return (pow(2, x) - 3*x);
}

double zlozenieFunkcji (double x) {
    return (pow(2,x) - 8*x*x + cos(x));
}

FunkcjaInterpolacyjna wybierzFunkcje()
{
    cout<<"Wybierz funkcje: "<<endl;
    cout<<"[1] "<<endl;
    cout<<"[2] "<<endl;
    cout<<"[3] "<<endl;
    cout<<"[4] "<<endl;
    cin>>nrFunkcji;

    switch (nrFunkcji) {
        case 1:
            return funkcjaTrygonometryczna;
        case 2:
            return funkcjaWykladnicza;
        case 3:
            return wielomian;
        case 4:
            return zlozenieFunkcji;
        default:
            return funkcjaTrygonometryczna;
    }
}

void wybierzLiczbeWezlow()
{
    cout<<endl<<"Podaj liczbe wezlow: ";
    cin>>liczbaWezlow;
}

void wyliczKrok()
{
    krok = fabs(koniecPrzedzialu - poczatekPrzedzialu)/(liczbaWezlow - 1);
}

void wyborParametrowProgramu()
{
    wybranaFunkcja = wybierzFunkcje();
    wybierzZakresArgumentow();
    wybierzLiczbeWezlow();
    wyliczKrok();
}

void inicjalizujTablice ()
{
    x = new double[liczbaWezlow];
    y = new double[liczbaWezlow];

    for(int i = 0; i < liczbaWezlow; i++)
    {
        x[i] = poczatekPrzedzialu + i*krok;
    }
}

void wyliczWartosciFunkcjiDlaX(FunkcjaInterpolacyjna funkcja)
{
    for(int i = 0; i < liczbaWezlow; i++)
    {
        y[i] = funkcja(x[i]);
    }
}

int** wyliczTrojkatPascala(int ileWierszy)
{
    int** trojkatPascala = new int*[ileWierszy];
    for (int i = 0; i < ileWierszy; i++)
    {
        trojkatPascala[i] = new int[i+1];
        trojkatPascala[i][0] = 1;
        trojkatPascala[i][i] = 1;
        for (int j = 0; j < i-1; j++)
        {
            trojkatPascala[i][j+1] = trojkatPascala[i-1][j] + trojkatPascala[i-1][j+1];
        }
    }
    return trojkatPascala;
}

double wyliczDelta(int* wspolczynnikiDelty, int stopien)
{
    double delta = 0;
    int wspolczynnikZnaku = 1;
    for (int i = 0; i < stopien+1; i++)
    {
        delta += wspolczynnikiDelty[i] * y[stopien-i];
        wspolczynnikZnaku *= -1;
    }
    return delta;
}

double* wyliczWspolczynniki()
{
    double* wspolczynniki = new double[liczbaWezlow];
    double iloraz = 1;
    int** trojkatPascala = wyliczTrojkatPascala(liczbaWezlow);
    wspolczynniki[0] = y[0];
    for (int i = 1; i < liczbaWezlow; i++)
    {
        double delta = wyliczDelta(trojkatPascala[i], i);
        iloraz *= i*krok;
        wspolczynniki[i] = delta / iloraz;
    }
    return wspolczynniki;
}

double interpoluj(double* wspolczynniki, double xx)
{
    double wynik = wspolczynniki[0];
    double tempX = 1;
    for (int i = 0; i < liczbaWezlow-1; i++)
    {
        tempX *= (xx - x[i]);
        wynik += wspolczynniki[i+1] * tempX;
    }
    return wynik;
}
int main()
{
    wyborParametrowProgramu();
    inicjalizujTablice();

    wyliczWartosciFunkcjiDlaX(wybranaFunkcja);
    double* wspolczynnikiWielomianu = wyliczWspolczynniki();

    return 0;
}
