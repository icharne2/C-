#include <iostream>
#include <string>

using namespace std;

const int MAX_CHAR = 256;   // jako maksymalna liczba mo�liwych znak�w, jakie mog� wyst�pi� w tek�cie na podstawie ASCII

// Funkcja obliczaj�ca automat sko�czony dla danego wzorca
void buildFSM(const string& pattern, int M, int** fsm) {
    fsm[0][pattern[0]] = 1;  // Inicjalizacja pierwszej kraw�dzi w automacie dla pierwszego znaku wzorca

    int x = 0;          // Zmienna pomocnicza do przechodzenia po automacie
    for (int i = 1; i <= M; ++i) {  // Przechodzimy przez kolejne znaki wzorca
        for (int j = 0; j < MAX_CHAR; ++j) {
            fsm[i][j] = fsm[x][j];      // Kopiujemy stany z poprzedniego wiersza automatu
        }
        if (i < M) {
            fsm[i][pattern[i]] = i + 1;     // Aktualizujemy automat dla kolejnego znaku wzorca
            x = fsm[x][pattern[i]];     // Przechodzimy do poprzedniego stanu w automacie
        }
    }
}

// Funkcja wyszukuj�ca wzorzec w tek�cie za pomoc� automatu sko�czonego
void searchPattern(const string& text, const string& pattern) {
    int M = pattern.length();   //Rozmair wzorca
    int N = text.length();      //Rozmiar tekstu

    // Inicjalizacja automatu sko�czonego (FSM)
    int** fsm = new int* [M + 1];   //Alokacja 2 wymiarowej tablicy

    for (int i = 0; i <= M; ++i) {
        fsm[i] = new int[MAX_CHAR];
        //Alokacja pami�ci dla tablicy stanu automatu
    }

    // Inicjalizacja tablicy stan�w
    for (int i = 0; i <= M; ++i) {  //Przechodzi przez wszystkie stany automatu (od 0 do M), w��cznie z dodatkowym stanem M, kt�ry jest u�ywany do obs�ugi sytuacji granicznych podczas przetwarzania wzorca.
        for (int j = 0; j < MAX_CHAR; ++j) {    //Przechodzi przez wszystkie mo�liwe znak
            fsm[i][j] = 0;  //Wyczyszczenie tablicy stanow - brak przej�cia do innych stan�w.
        }
    }

    // Budowa automatu sko�czonego
    buildFSM(pattern, M, fsm);

    // Wyszukiwanie wzorca w tek�cie
    int state = 0; // Pocz�tkowy stan automatu
    int occurrences = 0; // Licznik wyst�pie� wzorca

    for (int i = 0; i < N; ++i) {
        state = fsm[state][text[i]];    // Przechodzenie do kolejnego stanu w automacie
        if (state == M) {   //Sprawdza, czy wzorzec zosta� znaleziony
            cout << "Znaleziono wzorzec na indeksie: " << i - M + 1 << endl;
            state = fsm[state][pattern[M - 1]]; // Przesuni�cie do kolejnego pasuj�cego fragmentu

            occurrences++;  //inkrementacja licznika wyst�pie� wzorca
        }
    }

    cout << "Liczba wystapien wzorca: " << occurrences << endl;

    // Zwolnienie pami�ci
    for (int i = 0; i <= M; ++i) {
        delete[] fsm[i];
    }
    delete[] fsm;
}


int main() {
    string text = "ABABABACABA";
    string pattern = "ABABACA";

    searchPattern(text, pattern);

    system("PAUSE");
    return 0;
}
