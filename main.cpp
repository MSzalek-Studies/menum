#include "gnuplot_i.hpp"
#define GNUPLOT_PATH "C:\\gnuplot\\bin"
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;
double poczatekPrzedzialu, koniecPrzedzialu;
int nrOpcji, nrFunkcji, liczbaWezlow;
double *wspolczynnikiFunkcjiWielomianu;
int stopienWielomianu;
double krok;
double *x;
double *y;
bool czyPlik = false, czyWyborFunkcji = true;
double interpoluj(double* wspolczynniki, double xx);
void rysujWszystko(double* wspolczynnikiWielomianu);

typedef double (*FunkcjaInterpolacyjna)(double x);
FunkcjaInterpolacyjna wybranaFunkcja;

void wczytajPunkty(int ilePunktow)
{
    ifstream plik;
    plik.open("punkty.txt");

    if(plik.good())
    {
        x = new double[liczbaWezlow];
        y = new double[liczbaWezlow];

        for(int i = 0; i<ilePunktow; i++)
        {
            plik>>x[i];
            plik>>y[i];
        }
        plik.close();
        poczatekPrzedzialu = x[0];
        koniecPrzedzialu = x[ilePunktow - 1];
        krok = x[1] - x[0];

    } else {
        cout<<"Nie mozna otworzyc pliku!";
    }
}

void wybierzZakresArgumentow()
{
    do {
        cout<<"\nPodaj poczatek przedzialu: ";
        cin>>poczatekPrzedzialu;
        cout<<"\nPodaj koniec przedzialu: ";
        cin>>koniecPrzedzialu;
    } while(poczatekPrzedzialu >= koniecPrzedzialu);
}

double funkcjaLiniowa (double x) {
    return (2*x - 10);
}

double wartoscBezwzgledna(double x) {
    return fabs(x*x*x - 8);
}

double funkcjaTrygonometryczna (double x) {
    return 2*sin(x + 2);
}

double zlozenieFunkcji (double x) {
    return (2*sin(x) + 3*x);
}

double horner(double wsp[],int st, double x)
{
  double wynik = wsp[0];

  for(int i=1;i<=st;i++)
    wynik = wynik*x + wsp[i];

  return wynik;
}

double wielomian(double x) {
   // double wspolczynniki[] = {1, -1, -2, 1}; //x^3-x^2-2x+1

    if(czyWyborFunkcji){
        cout<<"Podaj stopien wielomianu: ";
        cin>>stopienWielomianu;

        wspolczynnikiFunkcjiWielomianu = new double [stopienWielomianu+1];

        for(int i=0;i<=stopienWielomianu;i++)
        {
            cout<<"Podaj wspolczynnik stojacy przy potedze "<<stopienWielomianu-i<<": ";
            cin>>wspolczynnikiFunkcjiWielomianu[i];
        }
        czyWyborFunkcji = false;
    }

    return horner(wspolczynnikiFunkcjiWielomianu, stopienWielomianu, x);
}

FunkcjaInterpolacyjna wybierzFunkcje()
{
    cout<<"\nWybierz funkcje: "<<endl;
    cout<<"[1] 2*x - 10"<<endl;
    cout<<"[2] |x^3 - 8|"<<endl;
    cout<<"[3] 2*sin(x + 2)"<<endl;
    cout<<"[4] 2*sin(x) + 3*x "<<endl;
    cout<<"[5] Wielomian (nalezy podac wspolczynniki wielomianu)"<<endl;
    cin>>nrFunkcji;

    switch (nrFunkcji) {
        case 1:
            return funkcjaLiniowa;
        case 2:
            return wartoscBezwzgledna;
        case 3:
            return funkcjaTrygonometryczna;
        case 4:
            return zlozenieFunkcji;
        case 5:
            return wielomian;
    }
}

void wybierzLiczbeWezlow()
{
    cout<<endl<<"Podaj liczbe wezlow: ";
    cin>>liczbaWezlow;
}

void wybierzOpcjePobraniaWartosciXiY ()
{
    cout<<"Wybierz sposob wczytania wartosci argumentow [1-2]: ";
    cout<<"\n\n1) Pobranie punktow o wspolrzednych (x, y) z pliku";
    cout<<"\n2) Wybranie funkcji oraz podanie poczatku i konca przedzialu\n";
    cin>>nrOpcji;
}
void wyliczKrok()
{
    krok = fabs(koniecPrzedzialu - poczatekPrzedzialu)/(liczbaWezlow - 1);
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

void wyborParametrowProgramu()
{
    wybierzOpcjePobraniaWartosciXiY();
    wybierzLiczbeWezlow();
    if(nrOpcji == 1){
        czyPlik = true;
        wczytajPunkty(liczbaWezlow);
    } else if(nrOpcji == 2){
        czyPlik = false;
        wybranaFunkcja = wybierzFunkcje();
        wybierzZakresArgumentow();
        wyliczKrok();
        inicjalizujTablice();
        wyliczWartosciFunkcjiDlaX(wybranaFunkcja);
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
        delta += wspolczynnikZnaku * wspolczynnikiDelty[i] * y[stopien-i];
        wspolczynnikZnaku *= -1;
    }
    return delta;
}

double* wyliczWspolczynniki()
{
    double* wspolczynniki = new double[liczbaWezlow];
    double dzielnik = 1;
    int** trojkatPascala = wyliczTrojkatPascala(liczbaWezlow);
    wspolczynniki[0] = y[0];
    for (int i = 1; i < liczbaWezlow; i++)
    {
        double delta = wyliczDelta(trojkatPascala[i], i);
        dzielnik *= i*krok;
        wspolczynniki[i] = delta / dzielnik;
    }
    return wspolczynniki;
}

double interpoluj(double* wspolczynniki, double X)
{
    double wynik = wspolczynniki[0];
    double tempX = 1;
    for (int i = 0; i < liczbaWezlow-1; i++)
    {
        tempX *= (X - x[i]);
        wynik += wspolczynniki[i+1] * tempX;
    }
    return wynik;
}

int main()
{
    wyborParametrowProgramu();

    double* wspolczynnikiWielomianu = wyliczWspolczynniki();
    rysujWszystko(wspolczynnikiWielomianu);

    return 0;
}

//-------RYSOWANIE-------
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

void rysujWszystko(double* wspolczynnikiWielomianu)
{
    int liczbaProbek = 100;
    Gnuplot gnuplot = przygotujGnuPlota();
    rysujPunkty(gnuplot);
    if(!czyPlik) {
        rysujFunkcjeWejsciowa(gnuplot, liczbaProbek);
    }
    rysujFunkcjeInterpolowana(gnuplot, liczbaProbek, wspolczynnikiWielomianu);
}
