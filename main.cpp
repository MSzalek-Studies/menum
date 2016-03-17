#include "gnuplot_i.hpp"
#define GNUPLOT_PATH "C:\\gnuplot\\bin"
#include <iostream>
#include <cmath>

bool czy_dokladnosc, czy_bisekcja;
double dokladnosc;
int liczba_iteracji;
double wczytany_poczatek_przedzialu, wczytany_koniec_przedzialu, poprzedni_poczatek_przedzialu;
double poczatek_przedzialu, koniec_przedzialu, pierwiastek, poprzedni_pierwiastek;

typedef double (*FunkcjaNieliniowa)(double x);
typedef double (*MetodaLiczenia)(FunkcjaNieliniowa);
using namespace std;

void wybierz_kryterium_zakonczenia();
void wczytaj_krance_przedzialu (FunkcjaNieliniowa fun);
double szacuj_pierwiastek(FunkcjaNieliniowa fun, MetodaLiczenia metodaLiczenia);
void rysuj_wykres(double pierwiastek_bisekcji, double pierwiastek_siecznych, FunkcjaNieliniowa funkcja);
double funkcja_trygonometryczna(double x);
double funkcja_kwadratowa(double x);
FunkcjaNieliniowa wybierz_funkcje();

double metoda_bisekcji(FunkcjaNieliniowa fun) {
    czy_bisekcja = true;
    return (poczatek_przedzialu + koniec_przedzialu) / 2;
}

double metoda_siecznych(FunkcjaNieliniowa fun) {
    czy_bisekcja = false;
    return koniec_przedzialu - fun(koniec_przedzialu)*(koniec_przedzialu-poczatek_przedzialu)/(fun(koniec_przedzialu)-fun(poczatek_przedzialu));
}

int main() {
    cout.precision(10);
    Gnuplot::set_GNUPlotPath( GNUPLOT_PATH );

    FunkcjaNieliniowa wybrana_funkcja = wybierz_funkcje();
    wybierz_kryterium_zakonczenia();
    wczytaj_krance_przedzialu(wybrana_funkcja);

    cout<<"\nMETODA BISEKCJI:\n\n";
    double pierwiastek_bisekcji = szacuj_pierwiastek(wybrana_funkcja, metoda_bisekcji);
    cout<<"\nMETODA SIECZNYCH:\n\n";
    double pierwiastek_siecznych = szacuj_pierwiastek(wybrana_funkcja, metoda_siecznych);

    rysuj_wykres(pierwiastek_bisekcji, pierwiastek_siecznych, wybrana_funkcja);

    return 0;
}

double funkcja_trygonometryczna (double x) {
    return sin(x);
}
double funkcja_kwadratowa(double x) {
    return (2*x);
}
double funkcja_wykladnicza (double x) {
    return (pow(2, x) - 5);
}

FunkcjaNieliniowa wybierz_funkcje() {

    cout<<"Wybierz funkcje [1-3]: "<<endl;
    cout<<"[1] sin(2*x-4)"<<endl;
    cout<<"[2] 4*x*x - 3*x + 5"<<endl;
    cout<<"[3] 2^x - 5"<<endl;
    int wybor_funkcji;
    cin>>wybor_funkcji;

    switch (wybor_funkcji) {
        case 1: return funkcja_trygonometryczna;
        case 2: return funkcja_kwadratowa;
        case 3: return funkcja_wykladnicza;
        default: return funkcja_kwadratowa;
    }

}

void wybierz_kryterium_zakonczenia() {
    cout<<"Podaj kryterium zakonczenia programu:"<<endl;
    cout<<"1) Dokladnosc"<<endl;
    cout<<"2) Liczba iteracji"<<endl;
    int kryterium_zakonczenia;
    cin>>kryterium_zakonczenia;
    if(kryterium_zakonczenia == 1)
    {
        czy_dokladnosc = true;
        cout<<"Podaj dokladnosc: ";
        cin>>dokladnosc;
    }
    else if (kryterium_zakonczenia == 2)
    {
        czy_dokladnosc = false;
        cout<<"Podaj liczba iteracji: ";
        cin>>liczba_iteracji;
    }
}

bool czy_rowne_znaki (double arg0, double arg1, FunkcjaNieliniowa fun) {
    //TODO: zobaczyæ co siê stanie jak bêdzie 0.
    return fun(arg0) * fun(arg1) > 0;
}

bool wyswietl_komunikat_o_zlych_krancach() {
    cout<<"Podane wartosci krancow przedzialow nie spelniaja zalozenia"<<endl;
    cout<<"o roznych znakach wybranej funkcji obliczonych dla jego krancow!"<<endl<<endl;
    return true;
}
void wczytaj_krance_przedzialu (FunkcjaNieliniowa fun) {
    do {
        cout<<"Podaj poczatek przedzialu: ";
        cin>>wczytany_poczatek_przedzialu;
        cout<<"Podaj koniec przedzialu: ";
        cin>>wczytany_koniec_przedzialu;
    } while (czy_rowne_znaki(wczytany_poczatek_przedzialu, wczytany_koniec_przedzialu, fun) && wyswietl_komunikat_o_zlych_krancach());
}

void aktualizuj_krance_metoda_bisekcji(FunkcjaNieliniowa fun){

    if( czy_rowne_znaki(poczatek_przedzialu, pierwiastek, fun))
        poczatek_przedzialu = pierwiastek;
    else
        koniec_przedzialu = pierwiastek;
}

void aktualizuj_krance_metoda_siecznych (FunkcjaNieliniowa fun) {

    koniec_przedzialu = poczatek_przedzialu;
    poczatek_przedzialu = pierwiastek;
}

void aktualizuj_krance(FunkcjaNieliniowa fun) {
    if(czy_bisekcja)
        aktualizuj_krance_metoda_bisekcji(fun);
    else
        aktualizuj_krance_metoda_siecznych(fun);
}

void licz_wg_dokladnosci(FunkcjaNieliniowa fun, MetodaLiczenia metodaLiczenia) {
    int przeprowadzonychIteracji = 1;
    pierwiastek = metodaLiczenia(fun);
    aktualizuj_krance(fun);
    do {
        poprzedni_pierwiastek = pierwiastek;
        pierwiastek = metodaLiczenia(fun);
        aktualizuj_krance(fun);
        przeprowadzonychIteracji++;
    } while(fabs(pierwiastek - poprzedni_pierwiastek) >= dokladnosc);

    cout<<"przeprowadzonych iteracji: "<<przeprowadzonychIteracji<<endl;
}
void licz_wg_iteracji(FunkcjaNieliniowa fun, MetodaLiczenia metodaLiczenia) {
    for( int i = 0; i < liczba_iteracji; i++) {
        pierwiastek = metodaLiczenia(fun);
        aktualizuj_krance(fun);
    }
}

double szacuj_pierwiastek(FunkcjaNieliniowa fun, MetodaLiczenia metodaLiczenia) {
    poczatek_przedzialu = wczytany_poczatek_przedzialu;
    koniec_przedzialu = wczytany_koniec_przedzialu;

    if (czy_dokladnosc) {
        licz_wg_dokladnosci(fun, metodaLiczenia);
    } else {
        licz_wg_iteracji(fun, metodaLiczenia);
    }
    cout<<"pierwiastek: "<<fixed<<pierwiastek<<endl;
    return pierwiastek;
}

void rysuj_punkt(double x, double y, string nazwa, Gnuplot &plot) {
    plot.set_style("points");
    plot.set_pointsize( 2.0 );
    vector<double> vecX(1);
    vector<double> vecY(1);
    vecX[0] = x;
    vecY[0] = y;
    plot.plot_xy( vecX, vecY, nazwa );
    getchar();
}

void rysuj_wykres(double pierwiastek_bisekcji, double pierwiastek_siecznych, FunkcjaNieliniowa funkcja){
    int liczba_probek = 100;
    Gnuplot main_plot;
    //main_plot.set_title( "tytul wykresu" );
    main_plot.set_xlabel( "X" );
    main_plot.set_ylabel( "Y" );
    main_plot.set_style( "lines" );
    main_plot.set_grid();
    main_plot.set_xrange(wczytany_poczatek_przedzialu, wczytany_koniec_przedzialu);
    vector<double> x(liczba_probek+1);
    vector<double> y(liczba_probek+1);
    double krok = (wczytany_koniec_przedzialu - wczytany_poczatek_przedzialu) / liczba_probek;
    for (int i=0; i<=liczba_probek; i++) {
        x[i] = wczytany_poczatek_przedzialu + i*krok;
        y[i] = funkcja(x[i]);
    }
    main_plot.plot_xy( x, y, "przebieg funkcji" );
    rysuj_punkt(pierwiastek_bisekcji, funkcja(pierwiastek_bisekcji), "Bisekcja", main_plot);
    rysuj_punkt(pierwiastek_siecznych, funkcja(pierwiastek_siecznych), "Sieczne", main_plot);
}


