//
// Created by luis on 24/08/24.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "cadastro.hpp"

Jogador::Jogador() {
    this->vitoriaReversi = 0;
    this->vitoriaLig4 = 0;
    this->vencedor = false;

}
Jogador::Jogador(std::string nome, unsigned int lig4, unsigned int reversi) : apelido(nome),
vitoriaLig4(lig4), vitoriaReversi(reversi), vencedor(false) {}

Jogador::~Jogador() {}

unsigned int Jogador::getVitoriasLig4() const { return this->vitoriaLig4; }
unsigned int Jogador::getVitoriasReversi() const { return this->vitoriaReversi; }
std::string Jogador::getNome() const { return this->apelido; }
bool Jogador::getVencedor() const { return this->vencedor; }

void Jogador::setNome(const std::string& nome) { this->apelido = nome;}
void Jogador::setVitoriasLig4(unsigned int pontos) { this->vitoriaLig4 += pontos; }
void Jogador::setVitoriasReversi(unsigned int pontos) { this->vitoriaReversi += pontos; }
void Jogador::setVencedor() { this->vencedor = true; }

bool comparaJogador(Jogador *j1, Jogador *j2) {
    if ((j1->getVitoriasLig4() + j1->getVitoriasReversi()) ==
        (j2->getVitoriasLig4() + j2->getVitoriasReversi()))
        return j1->getNome() < j2->getNome();

    return ( j1->getVitoriasLig4() + j1->getVitoriasReversi() <
             j2->getVitoriasLig4() + j2->getVitoriasReversi() ) ? false : true;
}


void lerCadastros(const std::string &path_for_file, std::vector<Jogador*> &cadastrosArr) {
    std::ifstream entrada;
    try {
        entrada.open(path_for_file, std::fstream::in);
        unsigned int pos1 = 0, pos2 = 0;
        std::string linha;
        while (entrada >> linha) {
            pos1 = linha.find(',');
            pos2 = linha.find(',', pos1+1);
            Jogador *aux = new Jogador(linha.substr(0,pos1),
                                       std::stoi(linha.substr(pos1+1,(pos2-1)-pos1)),
                                       std::stoi(linha.substr(pos2+1,(linha.size()-1)-pos2)));
            cadastrosArr.push_back(aux);
        }
        entrada.close();
    }
    catch (...) {
        entrada.close();
        return;
    }
    if (cadastrosArr.size() > 1) std::sort(cadastrosArr.begin(), cadastrosArr.end(), comparaJogador);
}

void atualizarCadastros(Jogador *j1, Jogador *j2, std::vector<Jogador*> &cadastradosArr) {
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

    std::sort(cadastradosArr.begin(), cadastradosArr.end(), comparaJogador);
}

void salvaCadastros(const std::string &path_for_file, std::vector<Jogador*> &cadastradosArr) {
    std::ofstream saida;
    try {
        saida.open(path_for_file, std::fstream::out);
        for (size_t i=0; i<cadastradosArr.size(); i++) {
            if (i == cadastradosArr.size()-1) {
                saida << cadastradosArr[i]->getNome() << "," <<
                      cadastradosArr[i]->getVitoriasLig4() << "," <<
                      cadastradosArr[i]->getVitoriasReversi();
            } else {
                saida << cadastradosArr[i]->getNome() << "," <<
                      cadastradosArr[i]->getVitoriasLig4() << "," <<
                      cadastradosArr[i]->getVitoriasReversi() << std::endl;
            }
        }
        saida.close();
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        saida.close();
        std::exit(EXIT_FAILURE);
    }
}

void imprimeCadastros(std::vector<Jogador*> &cadastradosArr) {
    for (size_t i=0; i<cadastradosArr.size(); i++) {
        std::cout << cadastradosArr[i]->getNome() << std::endl;
    }
}