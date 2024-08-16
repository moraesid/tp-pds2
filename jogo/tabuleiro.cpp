#include <iostream>
#include <vector>
#include <string>

const int LINHAS_LIG4  = 6;
const int COLUNAS_LIG4 = 7;
const int VITORIA_LIG4  = 4;

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
    void setPosicaoAtiva(unsigned int x, unsigned y) { this->tabuleiro[x][y].setAtivo();}
    void adicionaTilesAtivos() { this->tilesAtivos++; }
    //std::string nome() const { return this->nomeJogo; }

    virtual int executaJogada(int posicao, int referencia) = 0;
    virtual int verificaJogada(int posicao) = 0;
    virtual int verificaFimJogo(int x, int y, int referencia) = 0;
    virtual void imprimeTabuleiro() = 0;

};

class LigQuatro : public JogoTabuleiro {
private:
    int verificaLinha(int x, int y, int referencia) {
        int j = y + 1, k = y - 1;
        int count = 1;
        while (j < this->getY()) {
            if ((this->getElemento(x,j).getAtivo()) &&
                (this->getElemento(x,j).getReferencia() == referencia))
                j++;
            else
                break;
            count++;
        }
        while (k >= 0) {
            if ((this->getElemento(x,k).getAtivo()) &&
                (this->getElemento(x,k).getReferencia() == referencia))
                k--;
            else
                break;
            count++;
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
                i++;
            else
                break;
            count++;
        }
        if (count == VITORIA_LIG4) return 1;
        return 0;
    }
    int verificaDiagonal(int x, int y, int referencia) {
        return -1;
    }
public:
    LigQuatro(unsigned int x, unsigned int y, unsigned int id) : JogoTabuleiro(x, y, id) {}

    int verificaJogada(int posicao) override {
        if ((posicao >= this->getY()) || (posicao < 0)) return -1;
        for (size_t i=this->getX() - 1; i>0; i--) {
           if (!this->getElemento(i,posicao).getAtivo()) return i;
        }
        return -1;
    }

    int executaJogada(int coluna, int referencia) override {
        int verificaPos = this->verificaJogada(coluna);
        if (verificaPos == -1) return -1;
        this->getElemento(verificaPos,coluna).setAtivo();
        this->getElemento(verificaPos,coluna).setReferencia(referencia);
        return verificaPos;
    }

    int verificaFimJogo(int x, int y, int referencia) override {
        if (verificaColuna(x,y,referencia) || verificaLinha(x,y,referencia)) return 1;
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


int main() {
    LigQuatro lig4(LINHAS_LIG4, COLUNAS_LIG4, 1);

    int refJogador = 1;
    int posicao;
    while (true) {
        std::cout << "\nVEZ: JOGADOR " << refJogador << "\n" << std::endl;
        std::cin >> posicao;
        int linha = lig4.executaJogada(posicao, refJogador);
        if (linha == -1) continue;
        if (lig4.verificaFimJogo(linha, posicao, refJogador)) {
            std::cout << "\nJOGADOR " << refJogador << " VENCEU!!!" << "\n" << std::endl;
            lig4.imprimeTabuleiro();
            std::cout << "\n!!!!!!!!";
            return 0;
        }
        if (refJogador == 1) refJogador = 2;
        else refJogador = 1;
        lig4.imprimeTabuleiro();
    }

    return 0;
}