#include <iostream>
#include <vector>

using namespace std;

bool jeSiguran(vector<vector<int>> &alokacija, vector<vector<int>> &potreba, vector<int> &dostupno, int brojProcesa, int brojResursa, vector<int> &siguranRedosled) {
    vector<bool> zavrsen(brojProcesa, false);
    vector<int> rad = dostupno;

    for (int k = 0; k < brojProcesa; k++) {
        bool pronadjen = false;
        for (int i = 0; i < brojProcesa; i++) {
            if (!zavrsen[i]) {
                bool moguce = true;
                for (int j = 0; j < brojResursa; j++) {
                    if (potreba[i][j] > rad[j]) {
                        moguce = false;
                        break;
                    }
                }
                if (moguce) {
                    for (int y = 0; y < brojResursa; y++) {
                        rad[y] += alokacija[i][y];
                    }
                    siguranRedosled.push_back(i);
                    zavrsen[i] = true;
                    pronadjen = true;
                    break;
                }
            }
        }
        if (!pronadjen) {
            return false; 
        }
    }

    return true;
}

bool zahtevajResurse(vector<vector<int>> &alokacija, vector<vector<int>> &potreba, vector<int> &dostupno, vector<int> &zahtev, int idProcesa, int brojResursa) {
    for (int i = 0; i < brojResursa; i++) {
        if (zahtev[i] > potreba[idProcesa][i]) {
            cout << "Greska: proces je prekoracio svoj maksimalni zahtev." << endl;
            return false;
        }
        if (zahtev[i] > dostupno[i]) {
            cout << "Resursi nisu dostupni." << endl;
            return false;
        }
    }

    vector<vector<int>> tempAlokacija = alokacija;
    vector<vector<int>> tempPotreba = potreba;
    vector<int> tempDostupno = dostupno;

    for (int i = 0; i < brojResursa; i++) {
        tempDostupno[i] -= zahtev[i];
        tempAlokacija[idProcesa][i] += zahtev[i];
        tempPotreba[idProcesa][i] -= zahtev[i];
    }

    vector<int> siguranRedosled;
    bool siguran = jeSiguran(tempAlokacija, tempPotreba, tempDostupno, alokacija.size(), brojResursa, siguranRedosled);

    return siguran;
}

void unesiPodatke(int &brojProcesa, int &brojResursa, vector<int> &dostupno, vector<vector<int>> &maksimum, vector<vector<int>> &alokacija) {
    cout << "Unesite broj procesa: ";
    cin >> brojProcesa;
    cout << "Unesite broj resursa: ";
    cin >> brojResursa;

    dostupno.resize(brojResursa);
    cout << "Unesite dostupne resurse: ";
    for (int i = 0; i < brojResursa; i++) {
        cin >> dostupno[i];
    }

    maksimum.resize(brojProcesa, vector<int>(brojResursa));
    cout << "Unesite maksimalni zahtev svakog procesa: \n";
    for (int i = 0; i < brojProcesa; i++) {
        for (int j = 0; j < brojResursa; j++) {
            cin >> maksimum[i][j];
        }
    }

    alokacija.resize(brojProcesa, vector<int>(brojResursa));
    cout << "Unesite alokaciju svakog procesa: \n";
    for (int i = 0; i < brojProcesa; i++) {
        for (int j = 0; j < brojResursa; j++) {
            cin >> alokacija[i][j];
        }
    }
}

void prikaziPotrebe(vector<vector<int>> &potreba, int brojProcesa, int brojResursa) {
    cout << "Matrica potreba je: \n";
    for (int i = 0; i < brojProcesa; i++) {
        for (int j = 0; j < brojResursa; j++) {
            cout << potreba[i][j] << " ";
        }
        cout << endl;
    }
}

void obradiZahteve(int brojZahteva, int brojResursa, vector<vector<int>> &alokacija, vector<vector<int>> &potreba, vector<int> &dostupno) {
    for (int k = 0; k < brojZahteva; k++) {
        int idProcesa;
        cout << "Unesite ID procesa koji postavlja zahtev: ";
        cin >> idProcesa;
        vector<int> zahtev(brojResursa);
        cout << "Unesite zahtev za svaki resurs: ";
        for (int i = 0; i < brojResursa; i++) {
            cin >> zahtev[i];
        }

        vector<vector<int>> tempAlokacija = alokacija;
        vector<vector<int>> tempPotreba = potreba;
        vector<int> tempDostupno = dostupno;

        if (zahtevajResurse(tempAlokacija, tempPotreba, tempDostupno, zahtev, idProcesa, brojResursa)) {
            cout << "Zahtev može biti odobren.\n";
        } else {
            cout << "Zahtev ne može biti odobren.\n";
        }
    }
}

int main() {
    int brojProcesa, brojResursa;
    vector<int> dostupno;
    vector<vector<int>> maksimum, alokacija, potreba;

    unesiPodatke(brojProcesa, brojResursa, dostupno, maksimum, alokacija);

    potreba.resize(brojProcesa, vector<int>(brojResursa));
    for (int i = 0; i < brojProcesa; i++) {
        for (int j = 0; j < brojResursa; j++) {
            potreba[i][j] = maksimum[i][j] - alokacija[i][j];
        }
    }

    prikaziPotrebe(potreba, brojProcesa, brojResursa);

    vector<int> siguranRedosled;
    if (jeSiguran(alokacija, potreba, dostupno, brojProcesa, brojResursa, siguranRedosled)) {
        cout << "Sistem je u sigurnom stanju.\nSiguran redosled je: ";
        for (int i = 0; i < siguranRedosled.size(); i++) {
            cout << "P" << siguranRedosled[i] << " ";
        }
        cout << endl;
    } else {
        cout << "Sistem nije u sigurnom stanju.\n";
    }

    int brojZahteva;
    cout << "Unesite broj zahteva: ";
    cin >> brojZahteva;

    obradiZahteve(brojZahteva, brojResursa, alokacija, potreba, dostupno);

    return 0;
}
