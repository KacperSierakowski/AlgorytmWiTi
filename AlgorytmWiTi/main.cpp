#include <iostream>
#include <fstream>
#include <Windows.h>
#include "Zadanie.h"
#include <vector>


using namespace std;

class Permutacja {

public:
	vector <Zadanie>ZadaniawDanejKombinacji;//podzbior wszstkich zadan
	int Cmax;
	int OstatnieZadanie;
	int KaraZaWszystkieSpoznieni;
	int LiczCmax();
	int iloscZadanwDanejKombinacji;

};
int Permutacja::LiczCmax() {
	
	Cmax = 0;

	for (int i = 0; i < ZadaniawDanejKombinacji.size(); i++){
		Cmax += ZadaniawDanejKombinacji[i].CzasWykonywania_p;
	}
	return Cmax;
}


void swapDane(Zadanie *Danerpq, int i, int j)
{
	swap(Danerpq[i].CzasWykonywania_p, Danerpq[j].CzasWykonywania_p);
	swap(Danerpq[i].NumerZadania, Danerpq[j].NumerZadania);
	swap(Danerpq[i].WspolczynnikKary_w, Danerpq[j].WspolczynnikKary_w);
	swap(Danerpq[i].ZadanyTerminZakonczenia_d, Danerpq[j].ZadanyTerminZakonczenia_d);
}

void SortowanieSzybkie_R(Zadanie *Danerpq, int lewy, int prawy) {
	int i = lewy;
	int j = prawy;
	int x = Danerpq[(lewy + prawy) / 2].CzasWykonywania_p;
	do {
		while (Danerpq[i].CzasWykonywania_p < x)
			i++;
		while (Danerpq[j].CzasWykonywania_p > x)
			j--;
		if (i <= j) {
			swapDane(Danerpq, i, j);
			i++;
			j--;
		}
	} while (i <= j);
	if (lewy < j) { SortowanieSzybkie_R(Danerpq, lewy, j); }
	if (prawy > i) { SortowanieSzybkie_R(Danerpq, i, prawy); }
}

void AlgorytmWiTi(Zadanie *DaneWiTi, int liczbaZadan) {

	int *T;
	T = new int[liczbaZadan];
	int sumaWiTi=0;
	int C=0;

	for (int i = 0; i < liczbaZadan; i++) {


		C = C + DaneWiTi[i].CzasWykonywania_p;
		cout << "C: " << C << endl;

		if (C>DaneWiTi[i].ZadanyTerminZakonczenia_d) {
		//	C = C + DaneWiTi[i].WspolczynnikKary_w;
			cout << "C: " << C << endl;
		}

		T[i] = max(C - DaneWiTi[i].ZadanyTerminZakonczenia_d, 0);
		cout << "T: " << T[i] << endl;
		sumaWiTi =sumaWiTi+ DaneWiTi[i].WspolczynnikKary_w*T[i];
	}

	int iloœæPermutacji = 0;
	iloœæPermutacji = 1 << liczbaZadan;
	Permutacja TymczasowaPermutacja;
	vector<Permutacja> WszystkiePermutacje;
	WszystkiePermutacje.push_back(TymczasowaPermutacja);
	int adres = 0,k2=0;

	int i = 0;
	for (i = 0; i < iloœæPermutacji - 1; i++) {

		WszystkiePermutacje.push_back(TymczasowaPermutacja);
		WszystkiePermutacje[i].LiczCmax();
		for (int j = 0; j < liczbaZadan;j++) {
			if (SprawdzBity(j, i)==true){
				WszystkiePermutacje[i].ZadaniawDanejKombinacji.push_back(DaneWiTi[j]); // dodaj zadanie
			}
		}
		WszystkiePermutacje[i].iloscZadanwDanejKombinacji = WszystkiePermutacje[i].ZadaniawDanejKombinacji.size(); // pole pomocnicze
		WszystkiePermutacje[i].LiczCmax(); // policzenie ile wykonuje siê wszystkie zadania
		WszystkiePermutacje[i].KaraZaWszystkieSpoznieni = 9999999; // ustawienie nieskonczonej kary na start

		for (int b = 0; b <= liczbaZadan; b++) {
			if (SprawdzBity(b, i)==true) {
				int t = max(0, (WszystkiePermutacje[i].Cmax-DaneWiTi[b].ZadanyTerminZakonczenia_d));//spoznienie
				int k = DaneWiTi[b].WspolczynnikKary_w * t;//kara - spoznienie * wspolczynnik

				adres = reset(b, i);//resetujemy jeden z bitów i odliczamy kare dla elementu dla którego skasowaliœmy bit
				k2 = WszystkiePermutacje[adres].KaraZaWszystkieSpoznieni; // kara z poprzedniej kombnacji

				if (k + k2 < WszystkiePermutacje[i].KaraZaWszystkieSpoznieni)//jesli znajedziemy mniejsza kare od aktualnej
				{
					WszystkiePermutacje[i].KaraZaWszystkieSpoznieni = k + k2;//podmieniamy kare
					WszystkiePermutacje[i].OstatnieZadanie = 1 << b;//pomnieniamy ostatnie zadanie
				}
			}
		}
	}
	cout << "suma kar za spoznienia zadan w optymalnym rozwiazaniu: " << WszystkiePermutacje[i - 1].KaraZaWszystkieSpoznieni << endl;


	cout << "sumaWiTi: " << sumaWiTi << endl;
}
bool SprawdzBity(int b, int it){//sprawdza czy bity sa rowne
	return ((it >> b) & 1) == 1 ? true : false;
}
int reset(int b, int it) {//funkcja resetuje b-ty bit od prawej dla zadanej permutacji
	return 	(it &= ~(1 << b));
}


int main() {

	int kkk = 0;
	kkk = 5 >> 10;
	cout << "kkk" << kkk << endl;

	fstream Plik_z_Danymi;
	int liczbaZadan;
	int TempCzasWykonywania_p, TempWspolczynnikKary_w, TempZadanyTerminZakonczenia;
	Zadanie *DaneWiTi;
	Plik_z_Danymi.open("data10.txt", ios::in | ios::out);
	if (Plik_z_Danymi.good() == true) {

		cout << "Uzyskano dostep do pliku!" << endl;
		Plik_z_Danymi >> liczbaZadan;
		DaneWiTi = new Zadanie[liczbaZadan];
		for (int i = 0; i < liczbaZadan; i++) {
			Plik_z_Danymi >> TempCzasWykonywania_p >> TempWspolczynnikKary_w >> TempZadanyTerminZakonczenia;
			DaneWiTi[i].CzasWykonywania_p = TempCzasWykonywania_p;
			DaneWiTi[i].WspolczynnikKary_w = TempWspolczynnikKary_w;
			DaneWiTi[i].ZadanyTerminZakonczenia_d = TempZadanyTerminZakonczenia;
			DaneWiTi[i].NumerZadania = i;
			cout << DaneWiTi[i].CzasWykonywania_p << " " << DaneWiTi[i].WspolczynnikKary_w << " " << DaneWiTi[i].ZadanyTerminZakonczenia_d <<" "<< DaneWiTi[i].NumerZadania <<std::endl;
		}cout << endl;

		AlgorytmWiTi(DaneWiTi, liczbaZadan);
		SortowanieSzybkie_R(DaneWiTi, 0, liczbaZadan - 1);
		cout << "jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj" << endl;
		AlgorytmWiTi(DaneWiTi, liczbaZadan);


	}else {
		cout << "Brak dostepu do pliku" << endl;
	}



	system("pause");
	Plik_z_Danymi.close();
	return 0;
}