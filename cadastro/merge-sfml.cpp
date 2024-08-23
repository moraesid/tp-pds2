//
// Created by luis on 23/08/24.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

static const unsigned int MAX_CADASTROS = 6;

static const float VIEW_HEIGHT = 720.f;
static const float VIEW_WIDHT  = 1280.f;

class Jogador {
private:
    string apelido;
    unsigned int vitoriaLig4;
    unsigned int vitoriaReversi;
    bool vencedor;

public:
    Jogador() {}
    Jogador(string nome, unsigned int lig4, unsigned int reversi) : apelido(nome),
                                                                    vitoriaLig4(lig4), vitoriaReversi(reversi), vencedor(false) {}
    ~Jogador() {}

    unsigned int getVitoriasLig4() const { return this->vitoriaLig4; }
    unsigned int getVitoriasReversi() const { return this->vitoriaReversi; }
    string getNome() const { return this->apelido; }
    bool getVencedor() const { return this->vencedor; }

    void setVitoriasLig4(unsigned int pontos) { this->vitoriaLig4 += pontos; }
    void setVitoriasReversi(unsigned int pontos) { this->vitoriaReversi += pontos; }
    void setVencedor() { this->vencedor = true; }
};

bool comparaJogador(Jogador *j1, Jogador *j2) {
    if ((j1->getVitoriasLig4() + j1->getVitoriasReversi()) ==
        (j2->getVitoriasLig4() + j2->getVitoriasReversi()))
        return j1->getNome() < j2->getNome();

    return ( j1->getVitoriasLig4() + j1->getVitoriasReversi() <
             j2->getVitoriasLig4() + j2->getVitoriasReversi() ) ? false : true;
}


void lerCadastros(const string &path_for_file, vector<Jogador*> &cadastrosArr) {
    ifstream entrada;
    try {
        entrada.open(path_for_file, fstream::in);
        unsigned int pos1 = 0, pos2 = 0;
        string linha;
        while (entrada >> linha) {
            pos1 = linha.find(',');
            pos2 = linha.find(',', pos1+1);
            Jogador *aux = new Jogador(linha.substr(0,pos1),
                                       stoi(linha.substr(pos1+1,(pos2-1)-pos1)),
                                       stoi(linha.substr(pos2+1,(linha.size()-1)-pos2)));
            cadastrosArr.push_back(aux);
        }
        entrada.close();
    }
    catch (...) {
        entrada.close();
        return;
    }
    if (cadastrosArr.size() > 1) sort(cadastrosArr.begin(), cadastrosArr.end(), comparaJogador);
}

void atualizarCadastros(Jogador *j1, Jogador *j2, vector<Jogador*> &cadastradosArr) {
    bool j1Find = false, j2Find = false;
    for (size_t i=0; i<cadastradosArr.size(); i++) {
        if (cadastradosArr[i]->getNome() == j1->getNome()) {
            cadastradosArr[i]->setVitoriasLig4(j1->getVitoriasLig4());
            cadastradosArr[i]->setVitoriasReversi(j1->getVitoriasReversi());
            j1Find = true;
        }
        else if (cadastradosArr[i]->getNome() == j2->getNome()) {
            cadastradosArr[i]->setVitoriasLig4(j2->getVitoriasLig4());
            cadastradosArr[i]->setVitoriasReversi(j2->getVitoriasReversi());
            j2Find = true;
        }
    }
    if (!j1Find || !j2Find) {
        if ((MAX_CADASTROS - cadastradosArr.size()) >= 2) {
            if (!j1Find) cadastradosArr.push_back(j1);
            if (!j2Find) cadastradosArr.push_back(j2);
        } else {
            switch (MAX_CADASTROS - cadastradosArr.size()) {
                case 0:
                    if (!j1Find && !j2Find) {
                        cadastradosArr[MAX_CADASTROS-1] = j1;
                        cadastradosArr[MAX_CADASTROS-2] = j2;
                    }
                    else if (!j1Find) cadastradosArr[MAX_CADASTROS-1] = j1;
                    else if (!j2Find) cadastradosArr[MAX_CADASTROS-1] = j2;
                    break;
                case 1:
                    if (!j1Find && !j2Find) {
                        cadastradosArr[MAX_CADASTROS-2] = j2;
                        cadastradosArr.push_back(j1);
                    }
                    else if (!j1Find) cadastradosArr.push_back(j1);
                    else if (!j2Find) cadastradosArr.push_back(j2);
                    break;
            }
        }
    }

    sort(cadastradosArr.begin(), cadastradosArr.end(), comparaJogador);
}

void salvaCadastros(const string &path_for_file, vector<Jogador*> &cadastradosArr) {
    ofstream saida;
    try {
        saida.open(path_for_file, fstream::out);
        for (size_t i=0; i<cadastradosArr.size(); i++) {
            if (i == cadastradosArr.size()-1) {
                saida << cadastradosArr[i]->getNome() << "," <<
                      cadastradosArr[i]->getVitoriasLig4() << "," <<
                      cadastradosArr[i]->getVitoriasReversi();
            } else {
                saida << cadastradosArr[i]->getNome() << "," <<
                      cadastradosArr[i]->getVitoriasLig4() << "," <<
                      cadastradosArr[i]->getVitoriasReversi() << endl;
            }
        }
        saida.close();
    }
    catch (exception &e) {
        cout << e.what() << endl;
        saida.close();
        exit(EXIT_FAILURE);
    }
}

void imprimeCadastros(vector<Jogador*> &cadastradosArr) {
    for (size_t i=0; i<cadastradosArr.size(); i++) {
        cout << cadastradosArr[i]->getNome() << endl;
    }
}

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.antialiasingLevel = 8;
    settings.stencilBits = 8;
    sf::RenderWindow window(sf::VideoMode(VIEW_WIDHT, VIEW_HEIGHT),
                            "texto", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    sf::Color color(sf::Color::Black);


    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text texto;
    texto.setFont(font);
    texto.setCharacterSize(80);
    texto.setFillColor(sf::Color::White);
    texto.setStyle(sf::Text::Bold);
    texto.setPosition(sf::Vector2f(VIEW_WIDHT / 2.f, VIEW_HEIGHT / 2.f));
    texto.setString(" ");

    string saida = texto.getString();

    window.setKeyRepeatEnabled(false);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case Event::KeyPressed:
                    if (event.key.code == Keyboard::BackSpace && !saida.empty()) {
                        saida.pop_back();
                        texto.setString(saida);
                    }
                    break;
                case Event::TextEntered:
                    if ( (event.text.unicode <= 122 && event.text.unicode >= 97) ||
                            (event.text.unicode >= 48 && event.text.unicode <= 57) ) {
                        if (saida.length() <= 5) {
                            saida += event.text.unicode;
                            texto.setString(saida);
                        }
                    }
                    break;

            }
        }

        window.clear(color);
        window.draw(texto);
        window.display();
    }

    return 0;
}