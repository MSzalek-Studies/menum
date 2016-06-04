#include "gnuplot_i.hpp"
#define GNUPLOT_PATH "C:\\gnuplot\\bin"
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

typedef double (*FunkcjaPodcalkowa)(double x);

vector<double> tablicaWielomianow;
FunkcjaPodcalkowa funkcja;
int stopienWielomianuAproksymacyjnego, liczbaWezlow;
double bladAproksymacji, dokladnosc;
bool czyBladAproksymacji;
double poczatekPrzedzialu, koniecPrzedzialu;

double tablicaWartosci[4][2][5] = { {{0.585786, 3.414214, 0, 0, 0}, {0.853553, 0.146447, 0, 0, 0}},
                                    {{0.415775, 2.294280, 6.289945, 0, 0}, {0.711093, 0.278518, 0.010389, 0, 0}},
                                    {{0.322548, 1.745761, 4.536620, 2.395071, 0}, {0.603154, 0.357419, 0.038888, 0.000539, 0}},
                                    {{0.263560, 1.413403, 3.596426, 7.085810, 12.640801}, {0.521756, 0.398667, 0.075942, 0.003612, 0.000032}},
                                };

double funkcjaLiniowa (double x) {
   // return (x*x*x -3*x*x)*pow(M_E, -4*x);
    return sin(x)/2;
    //return x*x*x-9*x*x+18*x-6;
}

double horner(double wsp[],int st, double x)
{
  double wynik = wsp[0];

  for(int i=1;i<=st;i++) {
    wynik = wynik*x + wsp[i];
  }
  return wynik;
}

double wartoscBezwzgledna(double x) {
    int stopienWielomianu = 3;
    double wspolczynnikiFunkcjiWielomianu[] = {1, 0, 0, -5};
    return abs(horner(wspolczynnikiFunkcjiWielomianu, stopienWielomianu, x));
}

double wielomian(double x) {

    int stopienWielomianu = 3;
    double wspolczynnikiFunkcjiWielomianu[] = {1, 3, 4, 1};

    return horner(wspolczynnikiFunkcjiWielomianu, stopienWielomianu, x);
}

double funkcjaTrygonometryczna (double x) {
    return sin(x) + cos(x);
}

double zlaczenieFunkcji(double x) {
    //return cos(x) + 3*x*x - x;
    return sqrt(x);
}

double funkcjaWagowa(double x) {
    return pow(M_E, -x);
}

FunkcjaPodcalkowa wybierzFunkcje()
{
    int nrFunkcji;
    cout<<"Wybierz aproksymowana funkcje: "<<endl;
    cout<<"[1] 3x + 8"<<endl;
    cout<<"[2] |x^3 - 5|"<<endl;
    cout<<"[3] sin(x) + cos(x)"<<endl;
    //cout<<"[4] cos(x) + 3x^2 - x"<<endl;
    cout<<"[4] sqrt(x)"<<endl;
    cout<<"[5] x^3  + 3x^2 + 4x + 1"<<endl;
    cin>>nrFunkcji;

    switch (nrFunkcji) {
        case 1:
            return funkcjaLiniowa;
        case 2:
            return wartoscBezwzgledna;
        case 3:
            return funkcjaTrygonometryczna;
        case 4:
            return zlaczenieFunkcji;
        case 5:
            return wielomian;
    }
}
void wybierzPrzedzial() {
    cout<<"Podaj poczatek przedzialu aproksymacji: ";
    cin>>poczatekPrzedzialu;
    cout<<"Podaj koniec przedzialu aproksymacji: ";
    cin>>koniecPrzedzialu;
}

double wybierzDokladnosc() {
    cout<<"Podaj dokladnosc z jaka chcesz obliczyc calke: ";
    cin>>dokladnosc;
}

void wybierzBladAproksymacji() {
    cout<<"Podaj blad aproksymacji: ";
    cin>>bladAproksymacji;
}

void wybierzStopien () {
    cout<<"Podaj stopien wielomianu aproksymacyjnego: ";
    cin>>stopienWielomianuAproksymacyjnego;
}

vector<double> wyliczTabliceWielomianow(int stopien, double x) {
    vector<double> tablicaWiel;
    tablicaWiel.push_back(1);
    tablicaWiel.push_back(x - 1);
    int silnia = 1;
    for(int k = 2; k <= stopien; k++) {
        double L = (x - 2*(k-1) - 1) * tablicaWiel[k - 1] - (k-1) * (k-1) * tablicaWiel[k - 2];
        silnia *= k;
        tablicaWiel.push_back(L/(1.0*silnia));
    }
    return tablicaWiel;
}

double metodaGaussaLaguerra (double *tablicaPierwiastkow, double *tablicaWag, int liczba_wezlow, int stopien) {
    double suma = 0;

    for(int i = 0; i < liczba_wezlow; i++) {
        vector<double> tab1 = wyliczTabliceWielomianow(stopien, tablicaPierwiastkow[i]);
        suma += tablicaWag[i] * funkcja(tablicaPierwiastkow[i])* tab1[stopien];
    }
    return suma;
}

double wyliczFunkcjeAproksymujaca(int stopien, double x) {
    vector<double> tablicaWielomianow = wyliczTabliceWielomianow(stopien, x);
    double wynik = 0;
    //cout<<endl<<"x"<<x<<endl;
    //cout<<"stopien"<<stopien<<endl<<endl;
    for(int i = 0; i <= stopien; i++) {
        double calka = metodaGaussaLaguerra(tablicaWartosci[liczbaWezlow-2][0], tablicaWartosci[liczbaWezlow-2][1], liczbaWezlow, i);
            //double calka = obliczKwadratureNetwonaCotesa(i);
        double skladnik = calka*tablicaWielomianow[i];
        wynik += skladnik;
        /*cout<<"tab["<<i<<"]:"<<tablicaWielomianow[i]<<endl;
        cout<<"ca³ka: "<<obliczKwadratureNetwonaCotesa(i)<<endl;
        cout<<"skladnik: "<<skladnik<<endl;
        cout<<"wynik skladowy: "<<wynik<<endl<<endl;*/
    }
    //cout<<"WYNIK: "<<wynik<<endl<<endl;
    return wynik;
}

double obliczBlad(double poczatek, double koniec, int stopien) {
    double suma = 0, poprzedniaSuma, h, y1, y2, y3, x1, x2, x3;
    int liczbaPodprzedzialowCalkowania = 1;
    do {
        poprzedniaSuma = suma;
        suma = 0;
        h = ( koniec - poczatek )/(liczbaPodprzedzialowCalkowania*1.0);
        for(int i = 0; i < liczbaPodprzedzialowCalkowania; i++) {
            x1 = poczatek + i*h;
            x2 = x1 + 0.5*h;
            x3 = x1 + h;
            y1 = funkcjaWagowa(x1) * (funkcja(x1) - wyliczFunkcjeAproksymujaca(stopien, x1)) * (funkcja(x1) - wyliczFunkcjeAproksymujaca(stopien, x1));
            y2 = funkcjaWagowa(x2) * (funkcja(x2) - wyliczFunkcjeAproksymujaca(stopien, x2)) * (funkcja(x2) - wyliczFunkcjeAproksymujaca(stopien, x2));
            y3 = funkcjaWagowa(x3) * (funkcja(x3) - wyliczFunkcjeAproksymujaca(stopien, x3)) * (funkcja(x3) - wyliczFunkcjeAproksymujaca(stopien, x3));
            suma += h*(y1 + 4 * y2 + y3)/6.0;
        }
        liczbaPodprzedzialowCalkowania++;

    } while(fabs(suma - poprzedniaSuma) >= dokladnosc);
    return suma;
}

void wyznaczStopienWielomianuAproksymujacego (double poczatek, double koniec) {
    int stopien = 0;
    double blad = 0;
    do {
        stopien++;
        blad = obliczBlad(poczatek, koniec, stopien);
        cout<<endl<<blad;

    } while(blad >= bladAproksymacji);

    stopienWielomianuAproksymacyjnego = stopien;
}

void wybierzDokladnoscLubStopienWielomianuAproksymacji () {
    cout<<"Podaj sposob zakonczenia algorytmu";
    cout<<endl<<"[1] Podanie bledu aproksymacji";
    cout<<endl<<"[2] Podanie stopnia wielomianu"<<endl;
    int opcja;
    cin>>opcja;
    czyBladAproksymacji = opcja == 1;

    if(czyBladAproksymacji) {
        wybierzBladAproksymacji();
        wyznaczStopienWielomianuAproksymujacego(poczatekPrzedzialu, koniecPrzedzialu);
        cout<<endl<<stopienWielomianuAproksymacyjnego;

    } else {
        wybierzStopien();
    }
}

void wybierzLiczbeWezlowDoObliczeniaCalki() {
    cout<<endl<<"Wybierz liczbe wezlow do obliczenia calki(2,3,4,5): ";
    cin>>liczbaWezlow;
}

void wybierzParametry() {
    funkcja = wybierzFunkcje();
    wybierzPrzedzial();
    wybierzDokladnosc();
    wybierzLiczbeWezlowDoObliczeniaCalki();
    //dokladnosc = 0.001;

    wybierzDokladnoscLubStopienWielomianuAproksymacji();
}

double wyliczSymbolNewtona(int gornaLiczba, int dolnaLiczba) {
    double wynik = 1;

    for(int i = 1; i <= gornaLiczba; i++){
        wynik = wynik * ( dolnaLiczba - i + 1 ) / i;
    }
    return wynik;
}

double wielomianLaguerra (int stopien, double x) {
    double suma = 0, mnoznik = 1, silniaM = 1;
    suma = mnoznik * wyliczSymbolNewtona(stopien, 0) * silniaM * pow(x, stopien);
    for(int m = 1; m <= stopien; m++) {
        silniaM *= m;
        mnoznik *= -1;
        suma += mnoznik * wyliczSymbolNewtona(stopien, m) * silniaM * pow(x, stopien - m);
    }
    return suma;
}


double metodaSimpsona (double poczatek, double koniec, int stopien) {
    double suma = 0, poprzedniaSuma, h, y1, y2, y3, x1, x2, x3;
    int liczbaPodprzedzialowCalkowania = 1;
    do {
        poprzedniaSuma = suma;
        suma = 0;
        h = ( koniec - poczatek )/(liczbaPodprzedzialowCalkowania*1.0);
        for(int i = 0; i < liczbaPodprzedzialowCalkowania; i++) {
            x1 = poczatek + i*h;
            x2 = x1 + 0.5*h;
            x3 = x1 + h;
            vector<double> tab1 = wyliczTabliceWielomianow(stopien, x1);
            y1 = funkcjaWagowa(x1) * funkcja(x1) * tab1[stopien];
            vector<double> tab2 = wyliczTabliceWielomianow(stopien, x2);
            y2 = funkcjaWagowa(x2) * funkcja(x2) * tab2[stopien];
            vector<double> tab3 = wyliczTabliceWielomianow(stopien, x3);
            y3 = funkcjaWagowa(x3) * funkcja(x3)* tab3[stopien];
            suma += h*(y1 + 4 * y2 + y3)/6.0;
        }
        liczbaPodprzedzialowCalkowania++;

    } while(fabs(suma - poprzedniaSuma) >= dokladnosc);
    return suma;
}

double obliczKwadratureNetwonaCotesa(int stopien) {
    int poczatekPrzedzialuGranicy = 0, koniecPrzedzialuGranicy = 1;
    double sumaCalkowita = 0, sumaWPrzedziale;
    do {
        sumaWPrzedziale = metodaSimpsona(poczatekPrzedzialuGranicy, koniecPrzedzialuGranicy, stopien);
        sumaCalkowita += sumaWPrzedziale;
        poczatekPrzedzialuGranicy += 1;
        koniecPrzedzialuGranicy += 1;

    } while (abs(sumaWPrzedziale) >= dokladnosc);
    return sumaCalkowita;
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
    double krok = (koniecPrzedzialu - poczatekPrzedzialu) / liczbaProbek ;
    for (int i=0; i<=liczbaProbek; i++) {
        vecX[i] = poczatekPrzedzialu + i*krok;
        vecY[i] = funkcja(vecX[i]);
    }

    rysujWykres(vecX, vecY, "funkcja wejsciowa", plot);
}

void rysujFunkcjeAproksymujaca(Gnuplot &plot, int liczbaProbek)
{
    vector<double> vecX(liczbaProbek+1);
    vector<double> vecY(liczbaProbek+1);
    double krok = (koniecPrzedzialu - poczatekPrzedzialu) / liczbaProbek;
        cout<<"przed forem"<<endl;

    for (int i=0; i<=liczbaProbek; i++) {
        vecX[i] = poczatekPrzedzialu + i*krok;
       // cout<<i<<endl;
        vecY[i] = wyliczFunkcjeAproksymujaca(stopienWielomianuAproksymacyjnego, vecX[i]);
    }
            cout<<"po forze"<<endl;

    rysujWykres(vecX, vecY, "funkcja wyjsciowa", plot);
            cout<<"po rysowaniu"<<endl;

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

void rysujWszystko()
{
    int liczbaProbek = 100;
    Gnuplot gnuplot = przygotujGnuPlota();
    cout<<"przed wejsciowa"<<endl;
    rysujFunkcjeWejsciowa(gnuplot, liczbaProbek);
    cout<<"przed wyjsciowa"<<endl;
    rysujFunkcjeAproksymujaca(gnuplot, liczbaProbek);
    cout<<"po wyjsciowa"<<endl;

    getchar();
}

int main()
{
    wybierzParametry();
    rysujWszystko();
    cout<<"Blad: "<<obliczBlad(poczatekPrzedzialu, koniecPrzedzialu, stopienWielomianuAproksymacyjnego);
    return 0;
}
