#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

using namespace std;

// Algorithme de hachage 
string simpleHash(string input) {
    unsigned long hash = 5381;
    for (char c : input) {
        hash = ((hash << 5) + hash) + c;
    }
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << hash;
    return ss.str();
}

// Structure pour representer un bloc complet
struct Bloc {
    int index;
    string precedentHash;
    vector<string> transactions;
    long long nonce;
    string hash;
    string validateur; 

    Bloc(int idx, string prevHash, vector<string> txs) {
        index = idx;
        precedentHash = prevHash;
        transactions = txs;
        nonce = 0;
        validateur = "N/A";
        hash = calculerHash();
    }

    // Regroupe les transactions en une seule chaine
    string getTransactionsString() const {
        string res = "";
        for (size_t i = 0; i < transactions.size(); i++) {
            res += transactions[i] + ";";
        }
        return res;
    }

    // Calcule le hash du bloc selon ses attributs internes
    string calculerHash() const {
        stringstream ss;
        ss << index << precedentHash << getTransactionsString() << nonce << validateur;
        return simpleHash(ss.str());
    }

    // Minage par Proof of Work
    void minerPoW(int difficulte) {
        string cible(difficulte, '0');
        while (hash.substr(0, difficulte) != cible) {
            nonce++;
            hash = calculerHash();
        }
        cout << "Bloc " << index << " valide via PoW (Nonce: " << nonce << ", Hash: " << hash << ")" << endl;
    }

    // Validation par Proof of Stake
    void validerPoS(string nomValidateur) {
        validateur = nomValidateur;
        hash = calculerHash();
        cout << "Bloc " << index << " valide via PoS par : " << validateur << " (Hash: " << hash << ")" << endl;
    }
};

// Classe gerant l'ensemble de la chaine de blocs
class Blockchain {
private:
    vector<Bloc> chaine;
    int difficultePoW;

public:
    Blockchain(int difficulte) {
        difficultePoW = difficulte;
        creerBlocGenesis();
    }

    // Initialisation de la chaine avec le bloc initial
    void creerBlocGenesis() {
        vector<string> genesisTxs;
        genesisTxs.push_back("Genesis: Initialisation de la chaine");
        Bloc genesis(0, "0000000000000000", genesisTxs);
        genesis.minerPoW(1);
        chaine.push_back(genesis);
    }

    // Ajout d'un bloc avec le consensus PoW
    void ajouterBlocPoW(vector<string> transactions) {
        Bloc nouveauBloc(chaine.size(), chaine.back().hash, transactions);
        nouveauBloc.minerPoW(difficultePoW);
        chaine.push_back(nouveauBloc);
    }

    // Ajout d'un bloc avec le consensus PoS
    void ajouterBlocPoS(vector<string> transactions, string validateur) {
        Bloc nouveauBloc(chaine.size(), chaine.back().hash, transactions);
        nouveauBloc.validerPoS(validateur);
        chaine.push_back(nouveauBloc);
    }

    // Verification de l'integrite complete de la chaine
    bool verifierIntegrite() {
        for (size_t i = 1; i < chaine.size(); i++) {
            Bloc& blocActuel = chaine[i];
            Bloc& blocPrecedent = chaine[i - 1];

            // Verification 1 : le hash recalcule doit correspondre au hash stocke
            if (blocActuel.hash != blocActuel.calculerHash()) {
                cout << "[ALERTE] Falsification detectee : les donnees du bloc " << blocActuel.index << " ont ete modifiees !" << endl;
                return false;
            }

            // Verification 2 : le chainage des hashes doit etre preserve
            if (blocActuel.precedentHash != blocPrecedent.hash) {
                cout << "[ALERTE] Rupture de chaine detectee au bloc " << blocActuel.index << " !" << endl;
                return false;
            }
        }
        return true;
    }

    // Methode de test pour simuler une falsification frauduleuse
    void falsifierDonneesBloc(int indexBloc, string fausseTransaction) {
        if (indexBloc < chaine.size()) {
            cout << "\n[!] Simulation d'une attaque : modification non autorisee du bloc " << indexBloc << "..." << endl;
            chaine[indexBloc].transactions.push_back(fausseTransaction);
            // La transaction est alteree sans recalculer le hash de maniere valide
        }
    }
    
    void simulerAttaque51() {
        cout << "\n[!]SIMULATION D'UNE ATTAQUE DES 51%" << endl;
        if (chaine.size() > 1) {
            
            int cibleIndex = chaine.size() - 1;
            cout << "L'attaquant tente de re-miner le bloc " << cibleIndex << " avec de fausses transactions" << endl;
            chaine[cibleIndex].transactions.push_back("Attaquant : Vol de fonds par Double Spending");
            cout << "Le hash stocke ne correspond plus au hash recalcule !" << endl;
        }
    }
};

int main() {
    cout << " Deploiement et Test de la Blockchain " << endl;
    cout  << endl;

    Blockchain maBlockchain(2); 

    // 1. Ajout de blocs legitimes
    cout << "\n Ajout de blocs legitimes " << endl;
    
    vector<string> txsBloc1;
    txsBloc1.push_back("Khadija envoie 100 DH a Omar");
    txsBloc1.push_back("Salma envoie 50 DH a Youssef");
    maBlockchain.ajouterBlocPoW(txsBloc1);

    vector<string> txsBloc2;
    txsBloc2.push_back("Amine envoie 200 DH a Mehdi");
    maBlockchain.ajouterBlocPoS(txsBloc2, "Validateur_Hajar");

    // 2. Verification de l'integrite initiale
    cout << "\nTest 1 : Verification de l'integrite du reseau " << endl;
    if (maBlockchain.verifierIntegrite()) {
        cout << "Resultat : La blockchain est 100% VALIDE et intacte." << endl;
    } else {
        cout << "Resultat : La blockchain est CORROMPUE." << endl;
    }

    // 3. Test de falsification pour valider la detection d'attaque
    cout << "\nTest 2 : Tentative d'attaque par falsification de transactions " << endl;
    maBlockchain.falsifierDonneesBloc(1, "Transaction Frauduleuse : Vol de 10000 DH");

    // 4. Verification suite a l'attaque
    if (maBlockchain.verifierIntegrite()) {
        cout << "Resultat : La blockchain est VALIDE." << endl;
    } else {
        cout << "Resultat : Attaque interceptee avec succes ! La modification a ete rejetee." << endl;
    }

    
    maBlockchain.simulerAttaque51();
    
    if (maBlockchain.verifierIntegrite()) {
        cout << "Resultat :Blockchain intacte." << endl;
    } else {
        cout << "Resultat : [ALERTE] Attaque detectee et bloquee avec succes !" << endl;
    }

    return 0;
}