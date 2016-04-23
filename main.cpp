#include "gnuplot_i.hpp"
#define GNUPLOT_PATH "C:\\gnuplot\\bin"
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;
double poczatekPrzedzialu, koniecPrzedzialu;
int nrFunkcji, liczbaWezlow;
double krok;
double *x;
double *y;

typedef double (*FunkcjaInterpolacyjna)(double x);
FunkcjaInterpolacyjna wybranaFunkcja;

void rysujWykres(vector<double> vecX, vector<double> vecY, string nazwa, Gnuplot &plot) {
    plot.set_style( "lines" );
    plot.plot_xy( vecX, vecY, nazwa );
}
void rysujFunkcjeWejsciowa(Gnuplot &plot, int liczbaProbek)
{
    vector<double> vecX(liczbaProbek+1);
    vector<double> vecY(liczbaProbek+1);
    double krok = (koniecPrzedzialu - poczatekPrzedzialu) / liczbaProbek;
    for (int i=0; i<=liczbaProbek; i++) {
        vecX[i] = poczatekPrzedzialu + i*krok;
        vecY[i] = wybranaFunkcja(vecX[i]);
        cout<<vecX[i]<<" "<<vecY[i]<<endl;
    }
    rysujWykres(vecX, vecY, "wejsciowa", plot);
}
void rysujFunkcjeInterpolowana(Gnuplot &plot, int liczbaProbek, double* wspolczynnikiWielomianu)
{
    vector<double> vecX(liczbaProbek+1);
    vector<double> vecY(liczbaProbek+1);
    double krok = (koniecPrzedzialu - poczatekPrzedzialu) / liczbaProbek;
    for (int i=0; i<=liczbaProbek; i++) {
        vecX[i] = poczatekPrzedzialu + i*krok;
        vecY[i] = interpoluj(wspolczynnikiWielomianu, vecX[i]);
        cout<<vecX[i]<<" "<<vecY[i]<<endl;
    }
    rysujWykres(vecX, vecY, "wyjsciowa", plot);
    getchar();
}

void rysujPunkty(Gnuplot &plot)
{
    vector<double> vectorPunktyX(x, x+liczbaWezlow);
    vector<double> vectorPunktyY(y, y+liczbaWezlow);
    plot.set_style("points");
    plot.set_pointsize( 2.0 );
    plot.plot_xy( vectorPunktyX, vectorPunktyY, "punkty" );
    getchar();
}

Gnuplot przygotujGnuPlota()
{
   Gnuplot::set_GNUPlotPath( GNUPLOT_PATH );
    Gnuplot main_plot;
    //main_plot.set_title( "tytul wykresu" );
    main_plot.set_xlabel( "X" );
    main_plot.set_ylabel( "Y" );

    main_plot.set_grid();
    main_plot.set_xrange(poczatekPrzedzialu, koniecPrzedzialu);
    return main_plot;
}
void wyswietlWszystko(double* wspolczynnikiWielomianu)
{
    int liczbaProbek = 10;
    Gnuplot gnuplot = przygotujGnuPlota();
    rysujPunkty(gnuplot);
    rysujFunkcjeWejsciowa(gnuplot, liczbaProbek);
    rysujFunkcjeInterpolowana(gnuplot, liczbaProbek, wspolczynnikiWielomianu);
}

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
    return x;
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
    wyswietlWszystko(wspolczynnikiWielomianu);

    return 0;
}
