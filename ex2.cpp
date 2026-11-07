#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <chrono>

using namespace std;

// Fonction de hachage pour le calcul de l'empreinte
string simpleHash(string input) {
    unsigned long hash = 5381;
    for (char c : input) {
        hash = ((hash << 5) + hash) + c;
    }
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << hash;
    return ss.str();
}

// Structure representant un bloc de la blockchain
struct Bloc {
    int index;
    string precedentHash;
    string donnes;
    long long nonce;
    string hash;

    // Constructeur : initialise les attributs du bloc
    Bloc(int idx, string prevHash, string data) {
        index = idx;
        precedentHash = prevHash;
        donnes = data;
        nonce = 0;
        hash = calculerHash();
    }

    // Calcul de l'empreinte numerique combinant l'en-tete et les donnees
    string calculerHash() {
        stringstream ss;
        ss << index << precedentHash << donnes << nonce;
        return simpleHash(ss.str());
    }

    // Implementation de l'algorithme Proof of Work
    void minerBloc(int difficulte) {
        // Definition de la cible selon le niveau de difficulte demande
        string cible(difficulte, '0');

        // Mesure du temps de calcul
        auto debut = chrono::high_resolution_clock::now();

        // Recherche du nonce valide (le hash doit commencer par les zeros cibles)
        while (hash.substr(0, difficulte) != cible) {
            nonce++;
            hash = calculerHash();
        }

        auto fin = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duree = fin - debut;

        // Affichage des resultats de minage
        cout << "Bloc " << index << " valide :" << endl;
        cout << "  Difficulte      : " << difficulte << endl;
        cout << "  Nonce trouve    : " << nonce << endl;
        cout << "  Hash genere     : " << hash << endl;
        cout << "  Temps d'execution : " << duree.count() << " ms" << endl;
        cout << endl;
    }
};

int main() {
    cout << " Evaluation du mecanisme de Proof of Work " << endl;
    cout << endl;

    // Chaine de blocs
    vector<Bloc> blockchain;

    // Creation et minage du Bloc Genesis
    cout << "Creation du Bloc 0 (Genesis) :" << endl;
    Bloc genesis(0, "0000000000000000", "Bloc initial du reseau");
    genesis.minerBloc(1);
    blockchain.push_back(genesis);

    // Bloc 1 avec une difficulte de 2
    cout << "Creation du Bloc 1 :" << endl;
    Bloc bloc1(1, blockchain.back().hash, "Ali vers Reda : 100 DH");
    bloc1.minerBloc(2);
    blockchain.push_back(bloc1);

    // Bloc 2 avec une difficulte de 3
    cout << "Creation du Bloc 2 :" << endl;
    Bloc bloc2(2, blockchain.back().hash, "Israe vers Oumaima : 45 DH");
    bloc2.minerBloc(3);
    blockchain.push_back(bloc2);

    // Bloc 3 avec une difficulte de 4 
    cout << "Creation du Bloc 3 :" << endl;
    Bloc bloc3(3, blockchain.back().hash, "Houda vers Layla : 15 DH");
    bloc3.minerBloc(4);
    blockchain.push_back(bloc3);

    return 0;
}