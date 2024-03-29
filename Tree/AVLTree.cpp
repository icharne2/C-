#include <iostream>

using namespace std;

struct BinaryTree {
    BinaryTree* parent, * left, * right;
    int key;
    int bf;  // Wsp�czynnik r�wnowa�enia (Balance Factor) - r�nica wysoko�ci lewego i prawego poddrzewa
};

// Funkcja wypisuj�ca drzewo w porz�dku inorder
void inorder(BinaryTree* root) {
    if (root) {
        inorder(root->left);
        cout << root->key << " node height:" << root->bf << endl;
        inorder(root->right);
    }
}

// Funkcja do obliczania wysoko�ci w�z�a
int height(BinaryTree* node) {
    if (node == nullptr) {
        return 0;
    }
    // Wysoko�� w�z�a to maksymalna warto�� pomi�dzy wysoko�ciami lewego i prawego poddrzewa, plus jeden (w�ze� g��wny)
    return max(height(node->left), height(node->right)) + 1;
}

// Aktualizacja wsp�czynnika r�wnowa�enia i wysoko�ci w�z�a
void updateHeight(BinaryTree* node) {
    if (node) {
        // Obliczanie wsp�czynnika r�wnowa�enia (Balance Factor)
        node->bf = height(node->left) - height(node->right);
    }
}

// Rotacja w lewo
void rotateLeft(BinaryTree*& x) {
    if (x && x->right) {
        BinaryTree* tmp = x->right;
        BinaryTree* tmp2 = tmp->left;

        // Przeprowadzanie rotacji w lewo
        tmp->left = x;
        x->right = tmp2;

        // Aktualizacja wysoko�ci w�z��w po rotacji
        updateHeight(x);
        updateHeight(tmp);

        x = tmp;  // Aktualizacja korzenia poddrzewa
    }
}

// Rotacja w prawo
void rotateRight(BinaryTree*& x) {
    if (x && x->left) {
        BinaryTree* tmp = x->left;
        BinaryTree* tmp2 = tmp->right;

        // Przeprowadzanie rotacji w prawo
        tmp->right = x;
        x->left = tmp2;

        // Aktualizacja wysoko�ci w�z��w po rotacji
        updateHeight(x);
        updateHeight(tmp);

        x = tmp;  // Aktualizacja korzenia poddrzewa
    }
}

// Balansowanie drzewa AVL po dodaniu w�z�a
void balanceTree(BinaryTree*& root, BinaryTree* node) {
    while (node != nullptr) {
        // Aktualizacja wsp�czynnika r�wnowa�enia i wysoko�ci dla bie��cego w�z�a
        updateHeight(node);

        // Sprawdzenie, czy drzewo jest niezbalansowane
        if (node->bf > 1) {     //Lewa strona drzewa jest wieksza
            if (node->left->bf < 0) {   //Czy lewa strona jest "ci�sza"
                // Rotacja lewo-prawo (LR)
                rotateLeft(node->left);
            }
            // Rotacja prawo (RR)
            rotateRight(node);
        }
        else if (node->bf < -1) {   //Prawa strona wi�ksza ni� lewa
            if (node->right->bf > 0) {  // Czy prawa strona jest "ci�sza"
                // Rotacja prawo-lewo (RL)
                rotateRight(node->right);
            }
            // Rotacja lewo (LL)
            rotateLeft(node);
        }

        // Przechodzenie w g�r� drzewa
        node = node->parent;
    }
}

// Dodawanie w�z�a do drzewa AVL
void AddToTree(BinaryTree*& root, int value, BinaryTree* parent = nullptr) {
    if (!root) {    // Je�eli korze� drzewa nie istnieje
        BinaryTree* tmp = new BinaryTree;
        tmp->key = value;
        tmp->parent = parent;
        tmp->left = tmp->right = nullptr; // Inne elementy nie istniej�
        tmp->bf = 0;
        root = tmp;

    }
    else {        // Je�eli drzewo nie jest puste
        if (value >= root->key) {   // Dodanie do prawej strony drzewa
            AddToTree(root->right, value, root);
        }
        else {        // Dodanie do lewej strony drzewa
            AddToTree(root->left, value, root);
        }

        // Balansowanie drzewa po dodaniu w�z�a
        balanceTree(root, root);
    }
}

// Znajdowanie najmniejszego w�z�a w danym drzewie
BinaryTree* findMin(BinaryTree* node) {
    while (node->left) {
        node = node->left;
    }
    return node;
}

void deleteNodeHelper(BinaryTree*& root, int key) {
    if (!root) {    //  Je�eli dzrewo nie istnieje
        return;
    }
    else {  //  W przeciwnym wypadku - jesli drzewo istanieje
        //Szukanie klucza do usuniecia
        if (key < root->key) {
            // Szukaj w lewym poddrzewie
            deleteNodeHelper(root->left, key);
        }
        else if (key > root->key) {
            // Szukaj w prawym poddrzewie
            deleteNodeHelper(root->right, key);
        }
        else {
            // Gdy znaleziono w�ze� do usuni�cia
            if (!root->left && !root->right) {
                // Przypadek 1: W�ze� nie ma dzieci (jest li�ciem)
                delete root;
                root = nullptr;
            }
            else if (!root->left) {
                // Przypadek 2: W�ze� ma tylko prawe dziecko
                BinaryTree* temp = root;
                root = root->right;
                delete temp;
            }
            else if (!root->right) {
                // Przypadek 2: W�ze� ma tylko lewe dziecko
                BinaryTree* temp = root;
                root = root->left;
                delete temp;
            }
            else {
                // Przypadek 3: W�ze� ma dwoje dzieci
                BinaryTree* minRight = findMin(root->right);
                root->key = minRight->key;
                deleteNodeHelper(root->right, minRight->key);
            }
        }
    }
}

// Funkcja usuwaj�ca w�ze� z drzewa AVL
void deleteNode(BinaryTree*& root, int key) {
    // Wywo�anie funkcji pomocniczej deleteNodeHelper, aby usun�� w�ze� o kluczu 'key'
    deleteNodeHelper(root, key);

    // Balansowanie drzewa po usuni�ciu w�z�a
    if (root) {
        balanceTree(root, root);
    }
}


int main() {
    BinaryTree* root = nullptr;

    // Dodawanie przyk�adowych w�z��w
    AddToTree(root, 30);
    AddToTree(root, 20);
    AddToTree(root, 40);
    AddToTree(root, 10);
    AddToTree(root, 25);
    AddToTree(root, 35);
    AddToTree(root, 50);
    AddToTree(root, 5);

    // Wypisywanie drzewa w porz�dku inorder po dodaniu elementow
    cout << "Print inorder AVL tree:" << endl;
    inorder(root);

    //Usuniecie wezla
    int tmp;
    cout << "Enter element to be removed" << endl;
    cin >> tmp;

    deleteNode(root, tmp);

    //Wypisanie drzewa po usunieciu wezla
    cout << "\nNode with key " << tmp << " deleted.\n\n";

    // Wypisywanie drzewa po usuni�ciu w�z�a
    cout << "Print inorder AVL tree (after deletion):" << endl;
    inorder(root);

    return 0;
}
