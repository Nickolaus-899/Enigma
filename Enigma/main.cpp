#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Rotor {
public:
    vector<char> normalesAlphabet;
    vector<char> verschlusseltesAlphabet;
    bool dieEinstellung;
    int dieDrehungen;
    explicit Rotor(vector<char>& alphabet) : verschlusseltesAlphabet(alphabet) {
        for (char i = 65; i < 91; ++i) {
            normalesAlphabet.push_back(i);
        }
        for (char i = 48; i < 58; ++i) {
            normalesAlphabet.push_back(i);
        }
        dieDrehungen = 0;
        dieEinstellung = true;
    }
    void drehen() {
        char erste = verschlusseltesAlphabet[0];
        for (int i = 0; i < 36; ++i) {
            verschlusseltesAlphabet[i] = verschlusseltesAlphabet[i + 1];
        }
        verschlusseltesAlphabet[35] = erste;
        dieDrehungen++;
        if (!dieEinstellung) {
            cout << "*der Rotor hat sich gedreht\n";
        }
    }
};

class Ende : public exception {};

class Enigma {
private:
    class Leitung {
    public:
        char alter, neuer;
        Leitung(char alter, char neuer) : alter(alter), neuer(neuer) {}
    };
    class Schalttafel {
    private:
        vector<Leitung> draht;
    public:
        Schalttafel() = default;
        void hinzufugen(char alter, char neuer) {
            if (alter == neuer) {
                return;
            }
            for (auto& i : draht) {
                if (i.alter == alter || i.alter == neuer || i.neuer == alter || i.neuer == neuer) {
                    cout << "Die Draehte sind bereits verbunden\n";
                    return;
                }
            }
            draht.emplace_back(alter, neuer);
        }
        char ersetzen(char derBuchstaben) {
            bool gefunden = false;
            for (auto& i : draht) {
                if (i.alter == derBuchstaben) {
                    return i.neuer;
                }
                if (i.neuer == derBuchstaben) {
                    return i.alter;
                }
            }
            return derBuchstaben;
        }
    };

    class Reflektor {
    private:

        vector<Leitung> draht;
        vector<char> verschlusseltesAlphabet;
        bool enthalt(char derBuchstabe) {
            for (auto& i : draht) {
                if (i.alter == derBuchstabe || i.neuer == derBuchstabe) {
                    return true;
                }
            }
            return false;
        }
    public:
        Reflektor() {
            verschlusseltesAlphabet = {'C', 'A', '0', 'H', 'M', 'N', '8', 'L', '2', 'V', 'K', 'D', '3', 'Q', 'P',
                                       'T', '4', 'G', '7', 'U', 'J', '9', '6', '5', 'Y', 'X', 'E', 'R', 'Z', 'S',
                                       'B', 'I', 'O', 'W', '1', 'F'};
            for (int i = 0; i < 18; ++i) {
                draht.emplace_back(verschlusseltesAlphabet[i], verschlusseltesAlphabet[35 - i]);
            }
        }
        char ersetzen(char derBuchstaben) {
            bool gefunden = false;
            for (auto& i : draht) {
                if (i.alter == derBuchstaben) {
                    return i.neuer;
                }
                if (i.neuer == derBuchstaben) {
                    return i.alter;
                }
            }
            return derBuchstaben;
        }
        void drucken() {
            for (auto& i : draht) {
                cout << i.alter << " <> " << i.neuer << endl;
            }
        }
    };

    vector<Rotor> alleRotoren;
    vector<Rotor> installierteRotoren;
    const int anzahlDerRotoren = 3;
    Schalttafel dieSchalttafel;
    Reflektor derReflektor;
    char aktuellerBuchstabe;

    void ersetzen(char derBuchstabe) {
        if (derBuchstabe < 48 || derBuchstabe > 90 || (derBuchstabe > 57 && derBuchstabe < 65)) {
            cout << "Ungultiges Zeichen!\n";
            return;
        }
        installierteRotoren[0].drehen();
        for (int i = 1; i < anzahlDerRotoren; ++i) {
            if (installierteRotoren[i - 1].dieDrehungen < 36) {
                break;
            } else {
                installierteRotoren[i - 1].dieDrehungen = 0;
                installierteRotoren[i].drehen();
            }
        }
        char neuer = dieSchalttafel.ersetzen(derBuchstabe);
        for (int i = 0; i < anzahlDerRotoren; ++i) {
            if (neuer >= 65) {
                neuer = installierteRotoren[i].verschlusseltesAlphabet[neuer - 65];
            } else {
                neuer = installierteRotoren[i].verschlusseltesAlphabet[neuer - 48 + 26];
            }
        }
        neuer = derReflektor.ersetzen(neuer);
        for (int i = anzahlDerRotoren - 1; i >= 0; --i) {
            int index = 0;
            for (int j = 0; j < installierteRotoren[i].verschlusseltesAlphabet.size(); ++j) {
                if (installierteRotoren[i].verschlusseltesAlphabet[j] == neuer) {
                    index = j;
                    break;
                }
            }
            if (index < 26) {
                neuer = char(index + 65);
            } else {
                neuer = char(index + 48 - 26);
            }
        }
        neuer = dieSchalttafel.ersetzen(neuer);

        aktuellerBuchstabe = neuer;
    }
public:
    Enigma() {
        vector<char> erste = {'X', 'Y', 'P', 'N', '2', 'A', 'G', 'E', 'M', 'K', 'L', '3', 'Z', '8', 'O',
                               'F', 'B', 'V', 'I', '9', 'Q', 'D', 'C', 'J', '6', 'S', 'W', 'H', 'U', '1', '5',
                               'T', 'R','0', '7', '4'};
        vector<char> zweite = {'R', '4', 'H', 'W', 'X', 'S', 'P', 'K', 'N', 'G', '9', 'A', 'I', 'Z', 'V',
                               'L', '1', 'C', 'O', '6', 'E', 'Q', '8', '3', '7', 'Y', 'U', 'J', 'T', 'D', 'M',
                               '0', '2', '5', 'B', 'F'};
        vector<char> dritte = {'U', 'G', 'S', 'L', 'T', '6', 'K', 'N', 'B', 'A', 'P', '9', 'Q', 'V', 'W',
                               'J', 'M', 'O', '2', 'R', 'Y', '3', '8', 'Z', 'F', 'I', '7', 'H', '1', '5', 'X',
                               'C', 'E', '0', 'D', '4'};
        vector<char> vierte = {'B', 'Q', 'V', '2', '4', '3', '7', '8', 'M', 'S', 'W', 'K', 'I', 'O', '5', 'Y',
                               'C', 'E', 'F', 'H', '0', 'D', 'J', 'G', 'A', 'P', 'N', 'Z', '1', '6', 'X', 'R',
                               'U', '9', 'T', 'L'};
        vector<char> funfte = {'H', '9', 'O', '8', '0', 'E', 'Z', 'F', 'S', '1', 'A', 'N', '3', 'X', 'U',
                               'P', 'M', 'Y', '5', 'T', 'J', 'G', 'C', 'W', 'R', 'B', '6', 'D', 'I', 'K', '4',
                               'V', '7', 'L', 'Q', '2'};
        alleRotoren.emplace_back(erste);
        alleRotoren.emplace_back(zweite);
        alleRotoren.emplace_back(dritte);
        alleRotoren.emplace_back(vierte);
        alleRotoren.emplace_back(funfte);
        aktuellerBuchstabe = 32;
    }

    void einstellen() {
        ofstream konf;
        konf.open("./konf.txt");
        cout << "Das Alphabet und die Rotoren sind:\n";
        cout << "[A]: ";
        for (auto& i : alleRotoren[0].normalesAlphabet) {
            cout << i << " ";
        }
        cout << endl << endl;
        for (int i = 0; i < alleRotoren.size(); ++i) {
            cout << "[" << i + 1 << "]: ";
            for (auto& j : alleRotoren[i].verschlusseltesAlphabet) {
                cout << j << " ";
            }
            cout << endl;
        }

        cout << "Geben Sie " << anzahlDerRotoren << " Rotoren fur die Installation ein:\n";
        for (int i = 0; i < anzahlDerRotoren; ++i) {
            int rotor;
            cin >> rotor;
            konf << rotor << " ";
            installierteRotoren.push_back(alleRotoren[rotor - 1]);
        }
        konf << "\n";
        cout << "Geben Sie die Konfiguration fur jeden Rotor ein:\n";
        for (int i = 0; i < anzahlDerRotoren; ++i) {
            int konfiguration;
            cin >> konfiguration;
            konfiguration = konfiguration % 36;
            konf << konfiguration << " ";
            for (int j = 0; j < konfiguration; ++j) {
                installierteRotoren[i].drehen();
            }
        }
        konf << "\n";
        cout << "Wie viele Buchstaben wollen Sie ersetzen?\n";
        int n;
        cin >> n;
        konf << n << "\n";
        for (int i = 0; i < n; ++i) {
            cout << "Geben Sie ein paar Buchstaben ein:\n";
            char alter, neuer;
            cin >> alter >> neuer;
            konf << alter << " <-> " << neuer << "\n";
            dieSchalttafel.hinzufugen(alter, neuer);
        }
        for (auto& i : installierteRotoren) {
            i.dieEinstellung = false;
        }
        konf.close();
    }

    friend istream& operator>>(istream &in, Enigma &dieEnigma) {
        char derBuchstabe;
        cout << "\nGeben Sie einen Buchstaben oder '.' ein, um die Eingabe zu beenden:\n";
        in >> derBuchstabe;
        if (derBuchstabe == '.') {
            throw Ende();
        }
        dieEnigma.ersetzen(derBuchstabe);
        return in;
    }
    friend ostream& operator<<(ostream &out, Enigma &dieEnigma) {
        out << dieEnigma.aktuellerBuchstabe;
        return out;
    }
};

int main() {
    Enigma enigma;
    ofstream out;
    out.open("./output.txt");

    enigma.einstellen();
    while (true) {
        try {
            cin >> enigma;
        } catch (Ende& dasEnde) {
            break;
        }
        cout << enigma << endl;
        out << enigma;
    }
    out << "\n";
    out.close();
    return 0;
}
