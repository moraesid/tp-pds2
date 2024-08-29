//
// Created by luis on 24/08/24.
//

#ifndef CADS_H
#define CADS_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

static const unsigned int MAX_CADASTROS = 100;

class Jogador {
private:
    std::string apelido;
    unsigned int vitoriaLig4;
    unsigned int vitoriaReversi;
    bool vencedor;

public:
    Jogador();
    Jogador(std::string nome, unsigned int lig4, unsigned int reversi);
    ~Jogador();

    unsigned int getVitoriasLig4() const;
    unsigned int getVitoriasReversi() const;
    std::string getNome() const;
    bool getVencedor() const;

    void setNome(const std::string& nome);
    void setVitoriasLig4(unsigned int pontos);
    void setVitoriasReversi(unsigned int pontos);
    void setVencedor();
};

void lerCadastros(const std::string &path_for_file, std::vector<Jogador*> &cadastrosArr);
void atualizarCadastros(Jogador *j1, Jogador *j2, std::vector<Jogador*> &cadastradosArr);
void salvaCadastros(const std::string &path_for_file, std::vector<Jogador*> &cadastradosArr);
void imprimeCadastros(std::vector<Jogador*> &cadastradosArr);

#endif