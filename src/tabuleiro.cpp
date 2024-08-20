#include "tabuleiro.hpp"
#include <iostream>

static const int LINHAS_LIG4     = 6;
static const int COLUNAS_LIG4    = 7;
static const int LINHAS_REVERSI  = 8;
static const int COLUNAS_REVERSI = 8;

static const float VIEW_HEIGHT = 720.f;
static const float VIEW_WIDHT  = 1280.f;
static const float BOARD_DIMENSION = 722.f;
static const unsigned int NUM_TILES = 8;
static const float DIMENSION_TILE = BOARD_DIMENSION / float (NUM_TILES);

static const float BOARD_LIG_HEIGHT = 135.f * 6.f;
static const float BOARD_LIG_WIDHT = 135.f * 7.f;
static const unsigned int COLUNAS_LIG = 7;
static const unsigned int LINHAS_LIG  = 6;
static const float DIM_TILE_LIG = BOARD_LIG_WIDHT / float (COLUNAS_LIG4);

// Definicoes class TileMatriz - composicao
local::TileMatriz::TileMatriz() : ativo(false), referenciaJogador(-1) {}
bool local::TileMatriz::getAtivo() const { return this->ativo; }
void local::TileMatriz::setAtivo() { this->ativo = true; }
unsigned int local::TileMatriz::getReferencia() const { return this->referenciaJogador; }
void local::TileMatriz::setReferencia(unsigned int numJogador) { this->referenciaJogador = numJogador; }
// fim class TileMatriz

// Definicoes class JogoTabuleiro - classe base
local::JogoTabuleiro::JogoTabuleiro() {
    this->tabuleiro = nullptr;
    this->dimX = -1;
    this->dimY = -1;
    this->tilesAtivos = -1;
    this->idJogo = -1;
}
local::JogoTabuleiro::JogoTabuleiro(unsigned int x, unsigned int y,
                                unsigned int id) : dimX(y), dimY(y), idJogo(id) {
    this->tabuleiro = new TileMatriz*[x];
    for (size_t i=0; i<x; i++) {
        this->tabuleiro[i] = new TileMatriz[y];
    }
    this->tilesAtivos = 0;
}
local::JogoTabuleiro::~JogoTabuleiro() {
    for (size_t i=0; i<this->dimX; i++) {
        delete[] this->tabuleiro[i];
    }
    delete[] this->tabuleiro;
}

unsigned int local::JogoTabuleiro::getID() const { return this->idJogo; }
unsigned int local::JogoTabuleiro::getX()  const { return this->dimX; }
unsigned int local::JogoTabuleiro::getY()  const { return this->dimY; }
unsigned int local::JogoTabuleiro::getTilesAtivos() const { return this->tilesAtivos; }

void local::JogoTabuleiro::adicionaTilesAtivos() { this->tilesAtivos++; }
void local::JogoTabuleiro::adicionaTilesAtivos(unsigned int valor) { this->tilesAtivos += valor;}
local::TileMatriz& local::JogoTabuleiro::getElemento(unsigned int x, unsigned y) const {
    return this->tabuleiro[x][y];
}
// fim class JogoTabuleiro

// Definicoes class LigQuatro - heranca JogoTabuleiro
local::LigQuatro::LigQuatro() : local::JogoTabuleiro() {}
local::LigQuatro::LigQuatro(unsigned int x, unsigned int y, unsigned int id)
    : local::JogoTabuleiro(x, y, id){}

std::pair<bool,std::vector<std::pair<int,int>>>
    local::LigQuatro::verificaLinha(int x, int y, int referencia) {
        int j = y + 1;
        int count = 1;
        std::vector<std::pair<int,int>> retorno;
        /* verificar a direita de posicao (x,y) */
        while ((j < this->getY()) && (count < 4)) {
            if ((this->getElemento(x,j).getAtivo()) &&
                (this->getElemento(x,j).getReferencia() == referencia)) {
                count++;
                retorno.push_back(std::make_pair(x,j));
            }
            else
                break;
            j++;
        }
        if (count == 4) return std::make_pair(true, retorno);

        j = y - 1;
        /* verificar a esquerda de posicao (x,y) */
        while ((j >= 0) && (count < 4)) {
            if ((this->getElemento(x,j).getAtivo()) &&
                (this->getElemento(x,j).getReferencia() == referencia)) {
                count++;
                retorno.push_back(std::make_pair(x,j));
            }
            else
                break;
            j--;
        }
        if (count == 4) return std::make_pair(true, retorno);
        return std::make_pair(false, retorno);
    }

std::pair<bool,std::vector<std::pair<int,int>>>
    local::LigQuatro::verificaColuna(int x, int y, int referencia) {
        int i = x + 1;
        int count = 1;
        std::vector<std::pair<int,int>> retorno;
        while (i < this->getX()) {
            if ((this->getElemento(i,y).getAtivo()) &&
                (this->getElemento(i,y).getReferencia() == referencia)) {
                count++;
                retorno.push_back(std::make_pair(i,y));
            }
            else
                break;
            i++;
        }
        if (count == 4) return std::make_pair(true, retorno);
        return std::make_pair(false, retorno);
    }

std::pair<bool,std::vector<std::pair<int,int>>>
    local::LigQuatro::verificaDiagonalPrincipal(int x, int y, int referencia) {
        int i = x + 1, j = y + 1;
        int count = 1;
        std::vector<std::pair<int,int>> retorno;
        /* verificar abaixo da posicao (x,y) a direita */
        while ((i<this->getX()) && (j<this->getY()) && (count<4)) {
            if ((this->getElemento(i,j).getAtivo()) &&
                (this->getElemento(i,j).getReferencia() == referencia)) {
                count++;
                retorno.push_back(std::make_pair(i,j));
            }
            else
                break;
            i++; j++;
        }
        if (count == 4) return std::make_pair(true, retorno);

        i = x - 1, j = y - 1;
        /* verificar acima da posicao (x,y) a esquerda */
        while ((i>=0) && (j>=0) && (count<4)) {
            if ((this->getElemento(i,j).getAtivo()) &&
                (this->getElemento(i,j).getReferencia() == referencia)) {
                count++;
                retorno.push_back(std::make_pair(i,j));
            }
            else
                break;
            i--; j--;
        }
        if (count == 4) return std::make_pair(true, retorno);
        return std::make_pair(false, retorno);
    }

std::pair<bool,std::vector<std::pair<int,int>>>
    local::LigQuatro::verificaDiagonalSecundaria(int x, int y, int referencia) {
        int i = x + 1, j = y - 1;
        int count = 1;
        std::vector<std::pair<int,int>> retorno;
        /* verificar abaixo da posicao (x,y) a esquerda */
        while ((i<this->getX()) && (j>=0) && (count<4)) {
            if ((this->getElemento(i,j).getAtivo()) &&
                (this->getElemento(i,j).getReferencia() == referencia)) {
                count++;
                retorno.push_back(std::make_pair(i,j));
            }
            else
                break;
            i++; j--;
        }
        if (count == 4) return std::make_pair(true, retorno);

        i = x - 1, j = y + 1;
        /* verificar acima da posicao (x,y) a direita */
        while ((i>=0) && (j<this->getY()) && (count<4)) {
            if ((this->getElemento(i,j).getAtivo()) &&
                (this->getElemento(i,j).getReferencia() == referencia)) {
                count++;
                retorno.push_back(std::make_pair(i,j));
            }
            else
                break;
            i--; j++;
        }
        if (count == 4) return std::make_pair(true, retorno);
        return std::make_pair(false, retorno);
    }

int local::LigQuatro::verificaJogada(int posicao) const{
    if ((posicao >= this->getY()) || (posicao < 0)) return -1;
    for (int i=this->getX()-1; i>=0; i--) {
        if (!this->getElemento(i,posicao).getAtivo()) return i;
    }
    return -1;
}

void local::LigQuatro::imprimeTileVitoria(unsigned int x, unsigned int y,
                        sf::Sprite &token, sf::RenderWindow &window) {
    token.setPosition((sf::Vector2f(150.f + (DIM_TILE_LIG * y), 100.f + float(x * 135))));
    window.draw(token);
}

int local::LigQuatro::executaJogada(int coluna) {
    int verificaPos = this->verificaJogada(coluna);
    if (verificaPos == -1) return -1;
    this->adicionaTilesAtivos();
    return verificaPos;
}

bool local::LigQuatro::verificaFimJogo(unsigned  int x, unsigned int y, unsigned int referencia,
                                sf::Sprite &token, sf::RenderWindow &window) {
    std::pair<bool,std::vector<std::pair<int,int>>>
        linha = this->verificaLinha(x,y,referencia);
    std::pair<bool,std::vector<std::pair<int,int>>>
        coluna = this->verificaColuna(x,y,referencia);
    std::pair<bool,std::vector<std::pair<int,int>>>
        diagonalPrincipal = this->verificaDiagonalPrincipal(x,y,referencia);
    std::pair<bool,std::vector<std::pair<int,int>>>
        diagonalSecundaria = this->verificaDiagonalSecundaria(x,y,referencia);

    if (linha.first) {
        for (auto pares : linha.second) {
            this->imprimeTileVitoria(pares.first, pares.second, token, window);
        }
        this->imprimeTileVitoria(x, y, token, window);
        window.display();
        sf::sleep(sf::seconds(4.0f));
        return true;
    }
    else if (coluna.first) {
        for (auto pares : coluna.second) {
            this->imprimeTileVitoria(pares.first, pares.second, token, window);
        }
        this->imprimeTileVitoria(x, y, token, window);
        window.display();
        sf::sleep(sf::seconds(4.0f));
        return true;
    }
    else if (diagonalPrincipal.first) {
        for (auto pares : diagonalPrincipal.second) {
            this->imprimeTileVitoria(pares.first, pares.second, token, window);
        }
        this->imprimeTileVitoria(x, y, token, window);
        window.display();
        sf::sleep(sf::seconds(4.0f));
        return true;
    }
    else if (diagonalSecundaria.first) {
        for (auto pares : diagonalSecundaria.second) {
            this->imprimeTileVitoria(pares.first, pares.second, token, window);
        }
        this->imprimeTileVitoria(x, y, token, window);
        window.display();
        sf::sleep(sf::seconds(4.0f));
        return true;
    }
    if (this->getTilesAtivos() == this->getY() * this->getX()) return true;
    return false;
}

void local::LigQuatro::imprimeTabuleiroSFML(sf::RenderWindow &window,
                                     sf::Sprite &tokenRed, sf::Sprite &tokenYellow) {
    for (size_t i=0; i<this->getX(); i++) {
        for (size_t j=0; j<this->getY(); j++) {
            TileMatriz aux = this->getElemento(i,j);
            if (aux.getAtivo()) {
                if (aux.getReferencia() == 1) {
                    tokenRed.setPosition(
                            sf::Vector2f(150.f + float (DIM_TILE_LIG * j),
                                         100.f + float (DIM_TILE_LIG * i)));
                    window.draw(tokenRed);
                }
                else {
                    tokenYellow.setPosition(
                            sf::Vector2f(150.f + float (DIM_TILE_LIG * j),
                                         100.f + float (DIM_TILE_LIG * i)));
                    window.draw(tokenYellow);
                }
            }
        }
    }
}

void local::LigQuatro::imprimeTabuleiroEfeito(sf::RenderWindow &window, sf::Sprite &board,
                                       sf::Sprite &background, sf::Sprite &tokenRed,
                                       sf::Sprite &tokenYellow, sf::Text &versusText,
                                       sf::Text &currentPlayer, unsigned int x,
                                       unsigned int y, unsigned int referencia) {
    sf::Sprite aux;
    if (referencia == 1) aux = tokenRed;
    else aux = tokenYellow;

    for (size_t i=0; i<=x; i++) {
        window.clear(sf::Color(200,200,200));
        window.draw(background);
        window.draw(versusText);
        window.draw(currentPlayer);
        aux.setPosition(sf::Vector2f(150.f + (DIM_TILE_LIG * y), 100.f + float(i * 135)));
        window.draw(aux);
        this->imprimeTabuleiroSFML(window, tokenRed, tokenYellow);
        window.draw(board);
        window.display();
        sf::sleep(sf::milliseconds(70.0f));
    }
    this->getElemento(x,y).setAtivo();
    this->getElemento(x,y).setReferencia(referencia);
}
// fim class LigQuatro

//Definicoes Reversi - heranca JogoTabuleiro
local::Reversi::Reversi() : JogoTabuleiro() {}
local::Reversi::Reversi(unsigned int x, unsigned int y, unsigned int id) : JogoTabuleiro(x, y, id) {
    this->adicionaTilesAtivos();
    this->adicionaTilesAtivos();
    this->adicionaTilesAtivos();
    this->adicionaTilesAtivos();

    this->getElemento(3,3).setAtivo();
    this->getElemento(3,3).setReferencia(1);
    this->getElemento(4,3).setAtivo();
    this->getElemento(4,3).setReferencia(2);
    this->getElemento(3,4).setAtivo();
    this->getElemento(3,4).setReferencia(2);
    this->getElemento(4,4).setAtivo();
    this->getElemento(4,4).setReferencia(1);
}

std::pair<bool, std::vector<std::pair < int, int>>>
    local::Reversi::verificaLinha(int x, int y, int referencia) {
        int j = 0;
        bool direcaoValida = false;
        std::vector<std::pair<int,int>> retorno;
        std::vector<std::pair<int,int>> posicoes;
        TileMatriz auxTile;

        /* verificar posicoes a direita de (x,y) */
        if ((y+1 < this->getY()) && (this->getElemento(x,y+1).getAtivo()) &&
            (this->getElemento(x,y+1).getReferencia() != referencia)) {
            j = y + 1;
            auxTile = this->getElemento(x,j);
            while ((auxTile.getAtivo()) && (auxTile.getReferencia() != referencia)) {
                posicoes.push_back(std::make_pair(x,j));
                j++;
                if (j < this->getY()) auxTile = this->getElemento(x,j);
                else break;
            }
            if ((auxTile.getAtivo()) && (auxTile.getReferencia() == referencia)) {
                for (auto pares : posicoes) {
                    retorno.push_back(pares);
                }
                direcaoValida = true;
            }
            posicoes.clear();
        }
        /* verificar posicoes a esquerda de (x,y) */
        if ((y-1 >= 0) && (this->getElemento(x,y-1).getAtivo()) &&
            (this->getElemento(x,y-1).getReferencia() != referencia)) {
            j = y - 1;
            auxTile = this->getElemento(x,j);
            while ((auxTile.getAtivo()) && (auxTile.getReferencia() != referencia)) {
                posicoes.push_back(std::make_pair(x,j));
                j--;
                if (j >= 0) auxTile = this->getElemento(x,j);
                else break;
            }
            if ((auxTile.getAtivo()) && (auxTile.getReferencia() == referencia)) {
                for (auto pares : posicoes) {
                    retorno.push_back(pares);
                }
                direcaoValida = true;
            }
        }
        return std::make_pair(direcaoValida, retorno);
}

std::pair<bool, std::vector<std::pair < int, int>>>
    local::Reversi::verificaColuna(int x, int y, int referencia) {
        int i = 0;
        bool direcaoValida = false;
        std::vector<std::pair<int,int>> retorno;
        std::vector<std::pair<int,int>> posicoes;
        TileMatriz auxTile;

        /* verificar posicoes acima de (x,y) */
        if ((x-1 >= 0) && (this->getElemento(x-1,y).getAtivo()) &&
            (this->getElemento(x-1,y).getReferencia() != referencia)) {
            i = x - 1;
            auxTile = this->getElemento(i,y);
            while ((auxTile.getAtivo()) && (auxTile.getReferencia() != referencia)) {
                posicoes.push_back(std::make_pair(i,y));
                i--;
                if (i >= 0) auxTile = this->getElemento(i,y);
                else break;
            }
            if ((auxTile.getAtivo()) && (auxTile.getReferencia() == referencia)) {
                for (auto pares : posicoes) {
                    retorno.push_back(pares);
                }
                direcaoValida = true;
            }
            posicoes.clear();
        }
        /* verificar posicoes abaixo de (x,y) */
        if ((x+1 < this->getX()) && (this->getElemento(x+1,y).getAtivo()) &&
            (this->getElemento(x+1,y).getReferencia() != referencia)) {
            i = x + 1;
            auxTile = this->getElemento(i,y);
            while ((auxTile.getAtivo()) && (auxTile.getReferencia() != referencia)) {
                posicoes.push_back(std::make_pair(i,y));
                i++;
                if (i < this->getX()) auxTile = this->getElemento(i,y);
                else break;
            }
            if ((auxTile.getAtivo()) && (auxTile.getReferencia() == referencia)) {
                for (auto pares : posicoes) {
                    retorno.push_back(pares);
                }
                direcaoValida = true;
            }
        }
        return std::make_pair(direcaoValida, retorno);
}

std::pair<bool, std::vector<std::pair < int, int>>>
    local::Reversi::verificaDiagonalPrincipal(int x, int y, int referencia) {
        int i = 0, j = 0;
        bool direcaoValida = false;
        std::vector<std::pair<int,int>> retorno;
        std::vector<std::pair<int,int>> posicoes;
        TileMatriz auxTile;

        /* verificar posicoes acima e a esquerda de (x,y) */
        if ((x-1 >= 0) && (y-1 >= 0) && (this->getElemento(x-1,y-1).getAtivo()) &&
            (this->getElemento(x-1,y-1).getReferencia() != referencia)) {
            i = x - 1; j = y - 1;
            auxTile = this->getElemento(i,j);
            while ((auxTile.getAtivo()) && (auxTile.getReferencia() != referencia)) {
                posicoes.push_back(std::make_pair(i,j));
                i--; j--;
                if ((i >= 0) && (j >= 0)) auxTile = this->getElemento(i,j);
                else break;
            }
            if ((auxTile.getAtivo()) && (auxTile.getReferencia() == referencia)) {
                for (auto pares : posicoes) {
                    retorno.push_back(pares);
                }
                direcaoValida = true;
            }
            posicoes.clear();
        }
        /* verificar posicoes abaixo e a direita de (x,y) */
        if ((x+1 < this->getX()) && (y+1 < this->getY()) &&
            (this->getElemento(x+1,y+1).getAtivo()) &&
            (this->getElemento(x+1,y+1).getReferencia() != referencia)) {
            i = x + 1; j = y + 1;
            auxTile = this->getElemento(i,j);
            while ((auxTile.getAtivo()) && (auxTile.getReferencia() != referencia)) {
                posicoes.push_back(std::make_pair(i,j));
                i++; j++;
                if ((i < this->getX()) && (j < this->getY())) auxTile = this->getElemento(i,j);
                else break;
            }
            if ((auxTile.getAtivo()) && (auxTile.getReferencia() == referencia)) {
                for (auto pares : posicoes) {
                    retorno.push_back(pares);
                }
                direcaoValida = true;
            }
        }
        return std::make_pair(direcaoValida, retorno);
}

std::pair<bool, std::vector<std::pair < int, int>>>
    local::Reversi::verificaDiagonalSecundaria(int x, int y, int referencia) {
        int i = 0, j = 0;
        bool direcaoValida = false;
        std::vector<std::pair<int,int>> retorno;
        std::vector<std::pair<int,int>> posicoes;
        TileMatriz auxTile;

        /* verificar posicoes acima e a direita de (x,y) */
        if ((x-1 >= 0) && (y+1 < this->getY()) && (this->getElemento(x-1,y+1).getAtivo()) &&
            (this->getElemento(x-1,y+1).getReferencia() != referencia)) {
            i = x - 1; j = y + 1;
            auxTile = this->getElemento(i,j);
            while ((auxTile.getAtivo()) && (auxTile.getReferencia() != referencia)) {
                posicoes.push_back(std::make_pair(i,j));
                i--; j++;
                if ((i >= 0) && (j < this->getY())) auxTile = this->getElemento(i,j);
                else break;
            }
            if ((auxTile.getAtivo()) && (auxTile.getReferencia() == referencia)) {
                for (auto pares : posicoes) {
                    retorno.push_back(pares);
                }
                direcaoValida = true;
            }
            posicoes.clear();
        }
        /* verificar posicoes abaixo e a esquerda de (x,y) */
        if ((x+1 < this->getX()) && (y-1 >= 0) && (this->getElemento(x+1,y-1).getAtivo()) &&
            (this->getElemento(x+1,y-1).getReferencia() != referencia)) {
            i = x + 1; j = y - 1;
            auxTile = this->getElemento(i,j);
            while ((auxTile.getAtivo()) && (auxTile.getReferencia() != referencia)) {
                posicoes.push_back(std::make_pair(i,j));
                i++; j--;
                if ((i < this->getX()) && (j >= 0)) auxTile = this->getElemento(i,j);
                else break;
            }
            if ((auxTile.getAtivo()) && (auxTile.getReferencia() == referencia)) {
                for (auto pares : posicoes) {
                    retorno.push_back(pares);
                }
                direcaoValida = true;
            }
        }
        return std::make_pair(direcaoValida, retorno);
}

std::pair<bool, std::vector<std::pair<int,int>>>
    local::Reversi::verificaPosicoesValidas(int referencia) {
        bool isNotEmpty = false;
        std::vector<std::pair<int,int>> vizinhos = {
                {0,1}, {0,-1}, {1,0}, {-1,0},
                {1,1}, {-1,-1}, {-1,1}, {1,-1}};
        std::vector<std::pair<int,int>> retorno;
        std::stack<std::pair<int,int>> candidatos;
        int u,v; /* indices auxialiares */
        for (size_t i=0; i<this->getX(); i++) {
            for (size_t j=0; j<this->getY(); j++) {
                if (!this->getElemento(i,j).getAtivo()) {
                    for (auto pares: vizinhos) {
                        u = i + pares.first;
                        v = j + pares.second;
                        if ((u >= 0) && (u < this->getX()) && (v >= 0) && (v < this->getY())) {
                            if ((this->getElemento(u,v).getAtivo()) &&
                                (this->getElemento(u,v).getReferencia() != referencia)) {
                                candidatos.push(std::make_pair(i,j));
                                break;
                            }
                        }
                    }
                }
            }
        }
        while (!candidatos.empty()) {
            std::pair<int,int> aux = candidatos.top();
            if (this->verificaLinha(aux.first, aux.second, referencia).first) {
                retorno.push_back(aux);
                isNotEmpty = true;
            }
            if (this->verificaColuna(aux.first, aux.second, referencia).first) {
                retorno.push_back(aux);
                isNotEmpty = true;
            }
            else if (this->verificaDiagonalPrincipal(aux.first, aux.second,
                                                     referencia).first) {
                retorno.push_back(aux);
                isNotEmpty = true;
            }
            else if (this->verificaDiagonalSecundaria(aux.first, aux.second,
                                                      referencia).first) {
                retorno.push_back(aux);
                isNotEmpty = true;
            }
            candidatos.pop();
        }
        return std::make_pair(isNotEmpty, retorno);
}

bool local::Reversi::verificaJogada(int x, int y, int referencia) {
    if ((x < 0) || (x >= this->getX()) || (y < 0) || (y >= this->getY())) return false;
    if (this->getElemento(x,y).getAtivo()) return false;
    std::pair<int,int> jogada = {x,y};
    std::pair<bool, std::vector<std::pair<int,int>>>
            posicoesValidas = this->verificaPosicoesValidas(referencia);
    if (!posicoesValidas.first) return false;
    if (std::find(posicoesValidas.second.begin(), posicoesValidas.second.end(), jogada)
        != posicoesValidas.second.end())
        return true;
    return false;
}

void local::Reversi::virarPecas(std::vector<std::pair<int,int>> posicoes, int referencia) {
    for (auto pares : posicoes) {
        this->getElemento(pares.first, pares.second).setReferencia(referencia);
    }
}

int local::Reversi::getPontos(int referencia) const {
    int total = 0;
    for (size_t i=0; i<this->getX(); i++) {
        for (size_t j=0; j<this->getY(); j++) {
            TileMatriz aux = this->getElemento(i,j);
            if ((aux.getAtivo()) && (aux.getReferencia() == referencia))
                total++;
        }
    }
    return total; 
}

bool local::Reversi::executaJogada(int x, int y, int referencia) {
    if (!this->verificaJogada(x,y,referencia)) return false;
    else {
        this->adicionaTilesAtivos();
        this->getElemento(x,y).setAtivo();
        this->getElemento(x,y).setReferencia(referencia);

        std::pair<int,int> aux = {x,y};
        std::pair<bool,std::vector<std::pair<int,int>>> linha =
                                                                this->verificaLinha(aux.first, aux.second, referencia);
        std::pair<bool,std::vector<std::pair<int,int>>> coluna =
                                                                this->verificaColuna(aux.first, aux.second, referencia);
        std::pair<bool,std::vector<std::pair<int,int>>> diagonalPrincipal =
                                                                this->verificaDiagonalPrincipal(aux.first, aux.second, referencia);
        std::pair<bool,std::vector<std::pair<int,int>>> diagonalSecundaria =
                                                                this->verificaDiagonalSecundaria(aux.first, aux.second, referencia);

        if (linha.first) this->virarPecas(linha.second, referencia);
        if (coluna.first) this->virarPecas(coluna.second, referencia);
        if (diagonalPrincipal.first) this->virarPecas(diagonalPrincipal.second, referencia);
        if (diagonalSecundaria.first) this->virarPecas(diagonalSecundaria.second, referencia);

        return true;
    }
}

std::pair<bool,int> local::Reversi::passarVez(int referencia) {
    unsigned int referenciaOposta;
    if (referencia == 1) referenciaOposta = 2;
    else referenciaOposta = 1;
    bool verificaReferencia = this->verificaPosicoesValidas(referencia).first;
    bool verificaReferenciaOposta = this->verificaPosicoesValidas(referenciaOposta).first;

    if ((!verificaReferencia) && (!verificaReferenciaOposta)) return std::make_pair(true, -1);
    if (!referencia) return std::make_pair(true, referenciaOposta);
    return std::make_pair(false, -1);
}

std::pair<bool, int> local::Reversi::verificaFimJogo() const {
    if (this->getTilesAtivos() == this->getY() * this->getX()) {
        if (this->getPontos(1) > this->getPontos(2)) return std::make_pair(true, 1);
        return std::make_pair(true,2);
    }
    return std::make_pair(false,-1);
}

void local::Reversi::imprimePosicoesValidasSFML(sf::RenderWindow &window, sf::Sprite &tokenBlack,
                          sf::Sprite &tokenWhite, int referencia) {
    std::pair<bool, std::vector<std::pair<int,int>>>
    posicoesValidas = verificaPosicoesValidas(referencia);
    if (!posicoesValidas.first) return;
    for (size_t i=0; i<this->getX(); i++) {
        for (size_t j=0; j<this->getY(); j++) {
            std::pair<int,int> indices = {i,j};
            if (std::find(posicoesValidas.second.begin(), posicoesValidas.second.end(), indices)
                != posicoesValidas.second.end()) {
                sf::CircleShape cirle(28.f);
                cirle.setFillColor(sf::Color(50,50,50));
                cirle.setPosition(sf::Vector2f(150.f + 17.125 + (DIMENSION_TILE * j),
                                               150.f + 17.125 + (DIMENSION_TILE * i)));
                //cirle.setOutlineThickness(10);
                //cirle.setOutlineColor(sf::Color::Transparent);
                window.draw(cirle);
            }
            else {
                if (this->getElemento(i, j).getAtivo()) {
                    if (this->getElemento(i, j).getReferencia() == 1) {
                        tokenBlack.setPosition(
                                sf::Vector2f(150.f + 17.125 + (DIMENSION_TILE * j),
                                             150.f + 17.125 + (DIMENSION_TILE * i)));
                        window.draw(tokenBlack);
                    }
                    else if (this->getElemento(i, j).getReferencia() == 2) {
                        tokenWhite.setPosition(
                                sf::Vector2f(150.f + 17.125 + (DIMENSION_TILE * j),
                                             150.f + 17.125 + (DIMENSION_TILE * i)));
                        window.draw(tokenWhite);
                    }
                }
            }
        }
    }
}
// fim class Reversi