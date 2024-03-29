#include <iostream>
#include <math.h>

using namespace std;

const int V = 9; // Liczba wierzcho�k�w w grafie
const int E = 24; // Liczba kraw�dzi w grafie

// Struktura kraw�dzi grafu
struct Edge {
    int src, dest, weight;  // Wierzcho�ki kraw�dzi, waga kraw�dzi
};

// Struktura do reprezentowania zbioru wierzcho�k�w
struct Subset {
    int parent; //wska�nik na rodzica w drzewie zbior�w,
    int rank;   //G��boko�� drzewa
};

// Por�wnanie kraw�dzi na podstawie ich wagi
bool compareEdges(Edge a, Edge b) {
    return a.weight < b.weight;
}

// Znalezienie zbioru, do kt�rego nale�y element i
int find(Subset *subsets, int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

// Implementacja bubble sort
void bubbleSort(Edge *arr, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j].weight > arr[j + 1].weight)
                swap(arr[j], arr[j + 1]);
}

// Po��czenie dw�ch zbior�w - x i y
void Connect(Subset *subsets, int x, int y) { // Tablica struktur - reprezentacja zbior�w, wierzcho�ki x,y
    int xroot = find(subsets, x);   //Znalezienie orzenia zbioru do ktorego nalezy wierzcho�ek x
    int yroot = find(subsets, y);   //Znalezienie orzenia zbioru do ktorego nalezy wierzcho�ek y

    //rank to przyblizona wartosc drzewa, wybieramy, kt�ry z korzeni stanie si� rodzicem, mniejsza ranga = dziecko
    if (subsets[xroot].rank < subsets[yroot].rank)  //Aby nie wyst�pi� probelm przy r�wnowa�eniu drzewa poniewa� wtedy wyst�pi problem z u�yciem funkcji 'find'
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {  //Je�li s� r�wne, zwi�kszamy rang� drzewa wynikowego
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

void Kruskal(Edge *edges) {  //Przyjmuje wska�nik na krawed�
    Edge result[V];  // Wynik minimalnego drzewa rozpinaj�cego (MST), b�dzie mie� V-1 kraw�dzi
    int e = 0;  // Indeks dla wyniku MST
    int i = 0;  // Indeks do iteracji po posortowanych kraw�dziach

    // Sortowanie kraw�dzi na podstawie ich wag
    bubbleSort(edges, E);

    // Tworzenie V r�nych zbior�w. Ka�dy wierzcho�ek pocz�tkowo jest w swoim w�asnym zbiorze.
    Subset* subsets = new Subset[V];

    for (int v = 0; v < V; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    // Iteracja przez posortowane kraw�dzie i wyb�r tych, kt�re nie tworz� cyklu
    while (e < V - 1 && i < E) {    //Dop�ki nie skonstruujemy wystarczaj�cej liczby kraw�dzi lub nie przekroszymy wszystkich krawedzi
        Edge next_edge = edges[i++];    //Kolejna kraw�d� posortownej tablicy kraw�dzi

        int x = find(subsets, next_edge.src);   //zbi�r, do kt�rego nale�y wierzcho�ek �r�d�owy
        int y = find(subsets, next_edge.dest);  //zbi�r, do kt�rego nale�y wierzcho�ek docelowy 

        // Je�li w��czenie tej kraw�dzi nie powoduje powstania cyklu, dodaj j� do wyniku
        if (x != y) {
            result[e++] = next_edge;    // Dodanie krawedzi do wynikow
            Connect(subsets, x, y);   // Funkcja ��cz�ca oba zbiory w jeden wi�kszy
        }
    }

    // Wy�wietlenie skonstruowanego MST
    cout << "Krawedzie w konstrukcji:" << endl;

    for (i = 0; i < e; ++i)
        cout << result[i].src << " - " << result[i].dest << " = " << result[i].weight << endl;

    //Zwolnienie pami�ci
    delete[] subsets;
}


int main() {
	//Algorytm Kruskala
    
    //Tablica krawedzi grafu (Od,do,waga)
    Edge edges[E] = {
        {1, 2, 5}, {1, 4, 4}, {2, 1, 5}, {2, 8, 3}, {3, 5, 7}, {3, 6, 8}, 
        {3, 7, 1}, {3, 8, 7}, {4,1,4}, {4,6,20}, {4,8,14}, {5,3,7}, {5,6,3}, 
        {5,7,2}, {6,3,8}, {6,4,20}, {6,5,3}, {7,3,1}, {7,5,2}, {7,8,7}, {8,2,3}, 
        {8,3,7}, {8,4,14}, {8,7,7} 
    };

    Kruskal(edges);

	system("PAUSE");
	return 0;
}