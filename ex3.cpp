#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

// Fonction de hachage 
string simpleHash(string input) {
    unsigned long hash = 5381;
    for (char c : input) {
        hash = ((hash << 5) + hash) + c;
    }
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << hash;
    return ss.str();
}

// Structure d'un validateur pour le consensus Proof of Stake
struct Validateur {
    string nom;
    int solde; 

    Validateur(string n, int s) {
        nom = n;
        solde = s;
    }
};

// Algorithme de selection Proof of Stake 
Validateur choisirValidateur(const vector<Validateur>& validateurs) {
    int totalStake = 0;
    for (size_t i = 0; i < validateurs.size(); i++) {
        totalStake += validateurs[i].solde;
    }

    // Tirage d'un nombre aleatoire entre 0 et totalStake - 1
    int ticket = rand() % totalStake;
    int cumul = 0;

    for (size_t i = 0; i < validateurs.size(); i++) {
        cumul += validateurs[i].solde;
        if (ticket < cumul) {
            return validateurs[i];
        }
    }
    return validateurs[0];
}

// Validation via Proof of Work 
void validerAvecPoW(string donnes, int difficulte) {
    cout << "--- 1. Validation avec Proof of Work (PoW) ---" << endl;
    string cible(difficulte, '0');
    long long nonce = 0;
    string hash = "";

    auto debut = chrono::high_resolution_clock::now();

    do {
        nonce++;
        stringstream ss;
        ss << donnes << nonce;
        hash = simpleHash(ss.str());
    } while (hash.substr(0, difficulte) != cible);

    auto fin = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duree = fin - debut;

    cout << "Bloc valide par PoW !" << endl;
    cout << "  Difficulte demandee : " << difficulte << " zero(s)" << endl;
    cout << "  Nonce trouve        : " << nonce << endl;
    cout << "  Hash final          : " << hash << endl;
    cout << "  Temps d'execution   : " << duree.count() << " ms" << endl;
}

// Validation via Proof of Stake 
void validerAvecPoS(string donnes, const vector<Validateur>& validateurs) {
    cout << "\n 2. Validation avec Proof of Stake (PoS) " << endl;

    auto debut = chrono::high_resolution_clock::now();

    // Selection du validateur en fonction de ses jetons
    Validateur gagnant = choisirValidateur(validateurs);

    // Signature du bloc par le hash direct des donnees et du nom du validateur
    stringstream ss;
    ss << donnes << gagnant.nom;
    string hash = simpleHash(ss.str());

    auto fin = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duree = fin - debut;

    cout << "Bloc valide par PoS !" << endl;
    cout << "  Validateur selectionne : " << gagnant.nom << " (Mise: " << gagnant.solde << " DH)" << endl;
    cout << "  Hash genere            : " << hash << endl;
    cout << "  Temps d'execution      : " << duree.count() << " ms" << endl;
}

int main() {
    srand(time(0)); 

    cout << "Comparaison des Mecanismes de Consensus (PoW vs PoS) " << endl;
    cout << endl;

    string donnesTest = "Fatima transfere 250 DH a Youssef";

    // Test du consensus PoW
    validerAvecPoW(donnesTest, 4);

    // Configuration des noeuds validateurs pour le PoS
    vector<Validateur> listeValidateurs;
    listeValidateurs.push_back(Validateur("Validateur Hajar", 500));
    listeValidateurs.push_back(Validateur("Validateur Mehdi", 300));
    listeValidateurs.push_back(Validateur("Validateur Salma", 150));
    listeValidateurs.push_back(Validateur("Validateur Ali", 50));

    //  Test du consensus PoS
    validerAvecPoS(donnesTest, listeValidateurs);

    cout << endl;
    cout << "Conclusion academique :" << endl;
    cout << "Le mecanisme Proof of Stake est nettement plus rapide car il ne" << endl;
    cout << "necessite aucun calcul d'ajustement de hash par force brute (nonce)," << endl;
    cout << "ce qui supprime le temps d'attente et l'empreinte energetique." << endl;

    return 0;
}