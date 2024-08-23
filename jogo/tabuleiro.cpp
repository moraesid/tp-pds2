#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <stack>
#include <algorithm>
#include <SFML/Graphics.hpp>

static const int LINHAS_LIG4     = 6;
static const int COLUNAS_LIG4    = 7;
static const int VITORIA_LIG4    = 4;
static const int LINHAS_REVERSI  = 8;
static const int COLUNAS_REVERSI = 8;

class TileMatriz {
private:
    bool ativo; /* posicao ocupada por uma peca ou nao */
    int referenciaJogador; /* especifica qual jogador ocupa a posicao */

public:
    TileMatriz() : ativo(false), referenciaJogador(-1) {}

    bool getAtivo() const { return this->ativo; }
    void setAtivo() { this->ativo = true; }

    int getReferencia() const { return this->referenciaJogador; }
    void setReferencia(unsigned int numJogador) { this->referenciaJogador = numJogador; }
};

class JogoTabuleiro {
private:
    // std::vector<std::vector<TileMatriz>> tabuleiro;
    TileMatriz **tabuleiro;
    int dimX, dimY; /* dimensoes do tabuleiro */
    int tilesAtivos; /* numero de posicoes ocupadas no tabuleiro */
    //std::string nomeJogo;
    int idJogo;

protected:
    JogoTabuleiro() {
        this->tabuleiro = nullptr;
        this->dimX = -1;
        this->dimY = -1;
        this->tilesAtivos = -1;
        this->idJogo = -1;
    }
    JogoTabuleiro(unsigned int x, unsigned int y, unsigned int id) {
        this->tabuleiro = new TileMatriz*[x];
        for (size_t i=0; i<x; i++) {
            this->tabuleiro[i] = new TileMatriz[y];
        }
        this->dimX = x;
        this->dimY = y;
        this->tilesAtivos = 0;
        this->idJogo = id;
    }
    virtual ~JogoTabuleiro() {
        for (size_t i=0; i<this->dimX; i++) {
            delete[] this->tabuleiro[i];
        }
        delete[] this->tabuleiro;
    }

    int getID() const { return this->idJogo; }
    int getX()  const { return this->dimX; }
    int getY()  const { return this->dimY; }
    TileMatriz& getElemento(unsigned int x, unsigned y) const { return this->tabuleiro[x][y]; }
    //void setPosicaoAtiva(unsigned int x, unsigned y) { this->tabuleiro[x][y].setAtivo();}
    void adicionaTilesAtivos() { this->tilesAtivos++; }
    int getTilesAtivos() { return this->tilesAtivos; }
    //std::string nome() const { return this->nomeJogo; }

//    virtual int executaJogada(int x, int y=-1, int referencia) = 0;
//    virtual int verificaJogada(int x, int y=-1, int referencia=-1) = 0;
//    virtual int verificaFimJogo(int x, int y, int referencia) = 0;
    virtual void imprimeTabuleiro() = 0;

};

class LigQuatro : public JogoTabuleiro {
private:
    int verificaLinha(int x, int y, int referencia) {
        int j = y + 1;
        int count = 1;
        /* verificar a direita de posicao (x,y) */
        while ((j < this->getY()) && (count < VITORIA_LIG4)) {
            if ((this->getElemento(x,j).getAtivo()) &&
                (this->getElemento(x,j).getReferencia() == referencia))
                count++;
            else
                break;
            j++;
        }
        if (count == VITORIA_LIG4) return 1;

        j = y - 1;
        /* verificar a esquerda de posicao (x,y) */
        while ((j >= 0) && (count < VITORIA_LIG4)) {
            if ((this->getElemento(x,j).getAtivo()) &&
                (this->getElemento(x,j).getReferencia() == referencia))
                count++;
            else
                break;
            j--;
        }
        if (count == VITORIA_LIG4) return 1;
        return 0;
    }

    int verificaColuna(int x, int y, int referencia) {
        int i = x + 1;
        int count = 1;
        while (i < this->getX()) {
            if ((this->getElemento(i,y).getAtivo()) &&
                (this->getElemento(i,y).getReferencia() == referencia))
                count++;
            else
                break;
            i++;
        }
        if (count == VITORIA_LIG4) return 1;
        return 0;
    }
    int verificaDiagonalPrincipal(int x, int y, int referencia) {
        int i = x + 1, j = y + 1;
        int count = 1;
        /* verificar abaixo da posicao (x,y) a direita */
        while ((i<this->getX()) && (j<this->getY()) && (count<VITORIA_LIG4)) {
            if ((this->getElemento(i,j).getAtivo()) &&
                (this->getElemento(i,j).getReferencia() == referencia))
                count++;
            else
                break;
            i++; j++;
        }
        if (count == VITORIA_LIG4) return 1;

        i = x - 1, j = y - 1;
        /* verificar acima da posicao (x,y) a esquerda */
        while ((i>=0) && (j>=0) && (count<VITORIA_LIG4)) {
            if ((this->getElemento(i,j).getAtivo()) &&
                (this->getElemento(i,j).getReferencia() == referencia))
                count++;
            else
                break;
            i--; j--;
        }
        if (count == VITORIA_LIG4) return 1;
        return 0;
    }

    int verificaDiagonalSecundaria(int x, int y, int referencia) {
        int i = x + 1, j = y - 1;
        int count = 1;
        /* verificar abaixo da posicao (x,y) a esquerda */
        while ((i<this->getX()) && (j>=0) && (count<VITORIA_LIG4)) {
            if ((this->getElemento(i,j).getAtivo()) &&
                (this->getElemento(i,j).getReferencia() == referencia))
                count++;
            else
                break;
            i++; j--;
        }
        if (count == VITORIA_LIG4) return 1;

        i = x - 1, j = y + 1;
        /* verificar acima da posicao (x,y) a direita */
        while ((i>=0) && (j<this->getY()) && (count<VITORIA_LIG4)) {
            if ((this->getElemento(i,j).getAtivo()) &&
                (this->getElemento(i,j).getReferencia() == referencia))
                count++;
            else
                break;
            i--; j++;
        }
        if (count == VITORIA_LIG4) return 1;
        return 0;
    }

    int verificaJogada(int posicao) {
        if ((posicao >= this->getY()) || (posicao < 0)) return -1;
        for (int i=this->getX()-1; i>=0; i--) {
            if (!this->getElemento(i,posicao).getAtivo()) return i;
        }
        return -1;
    }

public:
    LigQuatro(unsigned int x, unsigned int y, unsigned int id) : JogoTabuleiro(x, y, id) {}

    int executaJogada(int coluna, int referencia) {
        int verificaPos = this->verificaJogada(coluna);
        if (verificaPos == -1) return -1;
        this->getElemento(verificaPos,coluna).setAtivo();
        this->getElemento(verificaPos,coluna).setReferencia(referencia);
        this->adicionaTilesAtivos();
        return verificaPos;
    }

    int verificaFimJogo(int x, int y, int referencia) {
        if (verificaColuna(x,y,referencia) || verificaLinha(x,y,referencia) ||
            verificaDiagonalPrincipal(x,y,referencia) ||
            verificaDiagonalSecundaria(x,y,referencia))
            return 1;
        if (this->getTilesAtivos() == this->getY() * this->getX()) return -1;
        return 0;
    }

    void imprimeTabuleiro() override {
        for (size_t i=0; i<this->getX(); i++) {
            for (size_t j=0; j<this->getY(); j++) {
                if (this->getElemento(i,j).getAtivo())
                    std::cout << this->getElemento(i,j).getReferencia() << " ";
                else
                    std::cout << "X ";
            }
            std::cout << "\n";
        }
    }
};

class Reversi : public JogoTabuleiro {
private:
    int pontosJogador1 = 0;
    int pontosJogador2 = 0;

    std::pair<bool,std::vector<std::pair<int,int>>> verificaLinha(int x, int y, int referencia) {
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

    std::pair<bool,std::vector<std::pair<int,int>>> verificaColuna(int x, int y, int referencia) {
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

    std::pair<bool,std::vector<std::pair<int,int>>>
            verificaDiagonalPrincipal(int x, int y, int referencia) {
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

    std::pair<bool,std::vector<std::pair<int,int>>>
            verificaDiagonalSecundaria(int x, int y, int referencia) {
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

    std::vector<std::pair<int,int>> verificaPosicoesValidas(int referencia) {
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
            if (this->verificaLinha(aux.first, aux.second, referencia).first)
                retorno.push_back(aux);
            else if (this->verificaColuna(aux.first, aux.second, referencia).first)
                retorno.push_back(aux);
            else if (this->verificaDiagonalPrincipal(aux.first, aux.second,
                                                                        referencia).first)
                retorno.push_back(aux);
            else if (this->verificaDiagonalSecundaria(aux.first, aux.second,
                                                                       referencia).first)
                retorno.push_back(aux);
            candidatos.pop();
        }
        return retorno;
    }

    bool verificaJogada(int x, int y, int referencia) {
        if ((x < 0) || (x >= this->getX()) || (y < 0) || (y >= this->getY())) return false;
        if (this->getElemento(x,y).getAtivo()) return false;
        std::pair<int,int> jogada = {x,y};
        std::vector<std::pair<int,int>> posicoesValidas = verificaPosicoesValidas(referencia);
        if (std::find(posicoesValidas.begin(), posicoesValidas.end(), jogada)
                != posicoesValidas.end())
            return true;
        return false;
    }

    void virarPecas(std::vector<std::pair<int,int>> posicoes, int referencia) {
        for (auto pares : posicoes) {
            this->getElemento(pares.first, pares.second).setReferencia(referencia);
        }
    }

    void setPontos(int pontos, int referencia) {
        switch (referencia) {
            case 1:
                this->pontosJogador1 += pontos;
                break;
            case 2:
                this->pontosJogador2 += pontos;
                break;
        }
    }

public:
    Reversi(unsigned int x, unsigned int y, unsigned int id) : JogoTabuleiro(x, y, id) {
        this->pontosJogador1 = 2;
        this->pontosJogador2 = 2;
        this->getElemento(3,3).setAtivo();
        this->getElemento(3,3).setReferencia(1);
        this->getElemento(4,3).setAtivo();
        this->getElemento(4,3).setReferencia(2);
        this->getElemento(3,4).setAtivo();
        this->getElemento(3,4).setReferencia(2);
        this->getElemento(4,4).setAtivo();
        this->getElemento(4,4).setReferencia(1);
    }

    int getPontos(int referencia) const {
        switch (referencia) {
            case 1:
                return this->pontosJogador1;
            case 2:
                return this->pontosJogador2;
        }
        return -1;
    }

    bool executaJogada(int x, int y, int referencia) {
        if (!this->verificaJogada(x,y,referencia)) {
            std::cout << "\n\n!!!!!!!!!!!! POSICAO INVALIDA !!!!!!!!!!!!!!!\n\n" << std::endl;
            return false;
        }
        else {
            std::pair<int,int> aux = {x,y};
            std::pair<bool,std::vector<std::pair<int,int>>> linha =
                    this->verificaLinha(aux.first, aux.second, referencia);
            std::pair<bool,std::vector<std::pair<int,int>>> coluna =
                    this->verificaColuna(aux.first, aux.second, referencia);
            std::pair<bool,std::vector<std::pair<int,int>>> diagonalPrincipal =
                    this->verificaDiagonalPrincipal(aux.first, aux.second, referencia);
            std::pair<bool,std::vector<std::pair<int,int>>> diagonalSecundaria =
                    this->verificaDiagonalSecundaria(aux.first, aux.second, referencia);

            if (linha.first) {
                this->virarPecas(linha.second, referencia);
                this->setPontos(linha.second.size(), referencia);
            }
            if (coluna.first) {
                this->virarPecas(coluna.second, referencia);
                this->setPontos(linha.second.size(), referencia);
            }
            if (diagonalPrincipal.first) {
                this->virarPecas(diagonalSecundaria.second, referencia);
                this->setPontos(linha.second.size(), referencia);
            }
            if (diagonalSecundaria.first) {
                this->virarPecas(diagonalSecundaria.second, referencia);
                this->setPontos(linha.second.size(), referencia);
            }
            this->adicionaTilesAtivos();
            this->getElemento(x,y).setAtivo();
            this->getElemento(x,y).setReferencia(referencia);
            return true;
        }
    }

    std::pair<bool,int> verificaFimJogo() {
        if (this->getTilesAtivos() == this->getY() * this->getX()) {
            if (this->getPontos(1) > this->getPontos(2)) return std::make_pair(true, 1);
            return std::make_pair(true,2);
        }
        return std::make_pair(false,-1);
    }

    void imprimePosicoesValidas(int referencia) {
        std::vector<std::pair<int,int>> posicoesValidas = verificaPosicoesValidas(referencia);
        for (size_t i=0; i<this->getX(); i++) {
            for (size_t j=0; j<this->getY(); j++) {
                std::pair<int,int> indices = {i,j};
                if (std::find(posicoesValidas.begin(), posicoesValidas.end(), indices)
                    != posicoesValidas.end()) {
                    std::cout << "0 ";
                }
                else {
                    if (this->getElemento(i, j).getAtivo())
                        std::cout << this->getElemento(i, j).getReferencia() << " ";
                    else
                        std::cout << "X ";
                }
            }
            std::cout << "\n";
        }
        std::cout << std::endl;
    }
    
    void imprimeTabuleiro() override {
        for (size_t i=0; i<this->getX(); i++) {
            for (size_t j=0; j<this->getY(); j++) {
                if (this->getElemento(i,j).getAtivo())
                    std::cout << this->getElemento(i,j).getReferencia() << " ";
                else
                    std::cout << "X ";
            }
            std::cout << "\n";
        }
        std::cout << "\n\n";
    }
};


int main() {
    Reversi reversi(LINHAS_REVERSI, COLUNAS_REVERSI, 2);
    int refJogador = 1;
    int x = 0, y = 0;
    while (true) {
        std::cout << "\nVEZ: JOGADOR " << refJogador << "\n" << std::endl;
        reversi.imprimePosicoesValidas(refJogador);
        std::cin >> x >> y;
        while (!reversi.executaJogada(x,y,refJogador)){
            reversi.imprimePosicoesValidas(refJogador);
            std::cin >> x >> y;
        }
        std::cout << "\n\n=================" << std::endl;
        reversi.imprimeTabuleiro();
        std::cout << "=================" << std::endl;
        if (reversi.verificaFimJogo().first) {
            std::cout << "\n\n" << reversi.verificaFimJogo().second <<
                                    " VENCEU !!!!!!!!!" << std::endl;
            return 0;
        }
        if (refJogador == 1) refJogador = 2;
        else refJogador = 1;
    }
}