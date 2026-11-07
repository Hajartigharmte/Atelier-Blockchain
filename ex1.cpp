#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// Fonction  pour simuler le hachage 
string simpleHash(string input) {
    unsigned long hash = 5381;
    for (char c : input) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << hash;
    return ss.str();
}

// Fonction qui construit l'arbre et retourne le Merkle Root
string getMerkleRoot(vector<string> transactions) {
    if (transactions.empty()) {
        return "";
    }

    // Etape 1: on calcule d'abord le hash de chaque transaction (les feuilles)
    vector<string> currentLevel;
    for (size_t i = 0; i < transactions.size(); i++) {
        currentLevel.push_back(simpleHash(transactions[i]));
    }

    cout << "--- Feuilles de l'arbre (Hashes) ---" << endl;
    for (size_t i = 0; i < currentLevel.size(); i++) {
        cout << "Feuille " << i + 1 << ": " << currentLevel[i] << endl;
    }
    cout<< endl;

    // Etape 2: on monte dans l'arbre niveau par niveau
    int niveau = 1;
    while (currentLevel.size() > 1) {
        vector<string> nextLevel;

        // Si le nombre d'elements est impair, on duplique le dernier element
        if (currentLevel.size() % 2 != 0) {
            currentLevel.push_back(currentLevel.back());
        }

        // On regroupe les hashes deux par deux et on les hache ensemble
        for (size_t i = 0; i < currentLevel.size(); i += 2) {
            string combine = currentLevel[i] + currentLevel[i + 1];
            nextLevel.push_back(simpleHash(combine));
        }

        // On passe au niveau superieur
        currentLevel = nextLevel;

        cout << "Niveau " << niveau << " (Nombre de noeuds = " << currentLevel.size() << "):" << endl;
        for (size_t i = 0; i < currentLevel.size(); i++) {
            cout << "  Hash " << i + 1 << ": " << currentLevel[i] << endl;
        }
        niveau++;
    }

    // Le dernier element restant est notre racine (Merkle Root)
    return currentLevel[0];
}

int main() {
    // Cas 1 : Exemple avec 4 transactions (nombre pair)
    vector<string> transactionsPair;
    transactionsPair.push_back("Tx1: Fatima -> Mehdi 200 DH");
    transactionsPair.push_back("Tx2: Youssef -> Salma 50 DH");
    transactionsPair.push_back("Tx3: Amine -> Khadija 120 DH");
    transactionsPair.push_back("Tx4: Omar -> Zineb 75 DH");

    cout << " Test 1: Calcul du Merkle Root (Cas pair: 4 transactions) " << endl;
    string rootPair = getMerkleRoot(transactionsPair);
    cout << "\n>>> Resultat Merkle Root: " << rootPair << "\n" << endl;

    // Cas 2 : Exemple avec 3 transactions pour verifier la gestion du cas impair
    vector<string> transactionsImpair;
    transactionsImpair.push_back("Tx1: Fatima -> Mehdi 200 DH");
    transactionsImpair.push_back("Tx2: Youssef -> Salma 50 DH");
    transactionsImpair.push_back("Tx3: Amine -> Khadija 120 DH");

    cout << "Test 2: Calcul du Merkle Root (Cas impair: 3 transactions) " << endl;
    string rootImpair = getMerkleRoot(transactionsImpair);
    cout << "\n>>> Resultat Merkle Root : " << rootImpair << "\n" << endl;

    return 0;
}