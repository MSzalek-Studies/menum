#include <iostream>
#include <cmath>
using namespace std;

typedef double (*FunkcjaPodcalkowa)(double x);

double dokladnosc;
int poczatekPrzedzialu, koniecPrzedzialu;
FunkcjaPodcalkowa funkcja;
double tablicaWartosci[4][2][5] = { {{0.585786, 3.414214, 0, 0, 0}, {0.853553, 0.146447, 0, 0, 0}},
                                    {{0.415775, 2.294280, 6.289945, 0, 0}, {0.711093, 0.278518, 0.010389, 0, 0}},
                                    {{0.322548, 1.745761, 4.536620, 2.395071, 0}, {0.603154, 0.357419, 0.038888, 0.000539, 0}},
                                    {{0.263560, 1.413403, 3.596426, 7.085810, 12.640801}, {0.521756, 0.398667, 0.075942, 0.003612, 0.000032}},
                                };

double funkcjaKwadratowa (double x) {
    return x*x - 4*x + 7;
}

double horner(double wsp[],int st, double x)
{
  double wynik = wsp[0];

  for(int i=1;i<=st;i++) {
    wynik = wynik*x + wsp[i];
  }
  return wynik;
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
    return cos(x) + 3*x*x - x;
}

FunkcjaPodcalkowa wybierzFunkcje()
{
    int nrFunkcji;
    cout<<"\nWybierz funkcje: "<<endl;
    cout<<"[1] x^2 -4x + 7"<<endl;
    cout<<"[2] 2sin(x + 2)"<<endl;
    cout<<"[3] cos(x) + 3x^2 - x"<<endl;
    cout<<"[4] x^3  + 3x^2 + 4x + 1"<<endl;
    cin>>nrFunkcji;

    switch (nrFunkcji) {
        case 1:
            return funkcjaKwadratowa;
        case 2:
            return funkcjaTrygonometryczna;
        case 3:
            return zlaczenieFunkcji;
        case 4:
            return wielomian;
    }
}

double funkcjaWagowa(double x) {
    return pow(M_E, -x);
}

double funkcjaWagowaCotesa(double x) {
    return funkcjaWagowa(x);
}

double wybierzDokladnosc() {
    cout<<"\nPodaj dokladnosc z jaka chcesz obliczyc calke: ";
    cin>>dokladnosc;
}

double metodaSimpsona (double poczatek, double koniec) {
    double suma = 0, poprzedniaSuma, h, y1, y2, y3;
    int liczbaPodprzedzialowCalkowania = 1;
    do {
        poprzedniaSuma = suma;
        suma = 0;
        h = ( koniec - poczatek )/(liczbaPodprzedzialowCalkowania*1.0);
        for(int i = 0; i < liczbaPodprzedzialowCalkowania; i++) {
            y1 = funkcjaWagowa(poczatek + i * h) * funkcja(poczatek + i * h);
            y2 = funkcjaWagowa(poczatek  + (i+0.5) * h) * funkcja(poczatek + (i+0.5) * h);
            y3 = funkcjaWagowa(poczatek + (i+1) * h) * funkcja(poczatek + (i+1) * h);
            suma += h*(y1 + 4 * y2 + y3)/6.0;
        }
        liczbaPodprzedzialowCalkowania++;

    } while(fabs(suma - poprzedniaSuma) >= dokladnosc);
    return suma;
}

double metodaGaussaLaguerra (double *tablicaPierwiastkow, double *tablicaWag, int rozmiar) {
    double suma = 0;
    for(int i = 0; i < rozmiar; i++) {
        suma += tablicaWag[i] * funkcja(tablicaPierwiastkow[i]);
    }
    return suma;
}

double obliczKwadratureNetwonaCotesa() {
    int poczatekPrzedzialuGranicy = 0, koniecPrzedzialuGranicy = 1;
    double sumaCalkowita = 0, sumaWPrzedziale;
    do {
        sumaWPrzedziale = metodaSimpsona(poczatekPrzedzialuGranicy, koniecPrzedzialuGranicy);
        sumaCalkowita += sumaWPrzedziale;
        poczatekPrzedzialuGranicy += 1;
        koniecPrzedzialuGranicy += 1;

    } while (sumaWPrzedziale >= dokladnosc);
    return sumaCalkowita;
}
void wybierzParametry () {
    //wybierzKrancePrzedzialuCalkowania();
    wybierzDokladnosc();
    funkcja = wybierzFunkcje();
    cout<<funkcja<<endl;
}
int main()
{
    wybierzParametry();
    cout<<"Wynik metoda Simpsona: "<<obliczKwadratureNetwonaCotesa()<<endl;
    cout<<"Wynik metoda Gaussa-Laguerre'a na 2 wezlach: "<<metodaGaussaLaguerra(tablicaWartosci[0][0], tablicaWartosci[0][1], 2)<<endl;
    cout<<"Wynik metoda Gaussa-Laguerre'a na 3 wezlach: "<<metodaGaussaLaguerra(tablicaWartosci[1][0], tablicaWartosci[1][1], 3)<<endl;
    cout<<"Wynik metoda Gaussa-Laguerre'a na 4 wezlach: "<<metodaGaussaLaguerra(tablicaWartosci[2][0], tablicaWartosci[2][1], 4)<<endl;
    cout<<"Wynik metoda Gaussa-Laguerre'a na 5 wezlach: "<<metodaGaussaLaguerra(tablicaWartosci[3][0], tablicaWartosci[3][1], 5)<<endl;

    return 0;
}
