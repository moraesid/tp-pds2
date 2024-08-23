#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <stack>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <SFML/Graphics.hpp>

static const int LINHAS_LIG4     = 6;
static const int COLUNAS_LIG4    = 7;
static const int VITORIA_LIG4    = 4;
static const int LINHAS_REVERSI  = 8;
static const int COLUNAS_REVERSI = 8;

static const float VIEW_HEIGHT = 720.f;
static const float VIEW_WIDHT  = 1280.f;
static const float BOARD_DIMENSION = 722.f;
static const float FIX_POSITION_WIDHT  = (VIEW_WIDHT - BOARD_DIMENSION) / 2.f;
static const float FIX_POSITION_HEIGHT = (VIEW_HEIGHT - BOARD_DIMENSION) / 2.f;
static const float FIX_POSITION_TOKEN = 5.f; /* board scale 0.7 */
static const unsigned int NUM_TILES = 8;
static const float DIMENSION_TILE = BOARD_DIMENSION / float (NUM_TILES);
static const float SCALE_TILE = 0.7; /* window dimension 720x720 */

static const float BOARD_LIG_HEIGHT = 135.f * 6.f;
static const float BOARD_LIG_WIDHT = 135.f * 7.f;
static const unsigned int COLUNAS_LIG = 7;
static const unsigned int LINHAS_LIG  = 6;
static const float DIM_TILE_LIG = BOARD_LIG_WIDHT / float (COLUNAS_LIG4);

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
        std::cout << "DESTRUCTOR" << std::endl;
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
    //virtual void imprimeTabuleiro() = 0;

};

class Reversi : public JogoTabuleiro {
private:
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
                else break; /* !!!!!!!!!!!!!!!!!!!! */
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

    std::pair<bool, std::vector<std::pair<int,int>>> verificaPosicoesValidas(int referencia) {
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

    bool verificaJogada(int x, int y, int referencia) {
        if ((x < 0) || (x >= this->getX()) || (y < 0) || (y >= this->getY())) return false;
        if (this->getElemento(x,y).getAtivo()) return false;
        std::pair<int,int> jogada = {x,y};
        std::pair<bool, std::vector<std::pair<int,int>>>
                    posicoesValidas = verificaPosicoesValidas(referencia);
        if (!posicoesValidas.first) return false;
        if (std::find(posicoesValidas.second.begin(), posicoesValidas.second.end(), jogada)
            != posicoesValidas.second.end())
            return true;
        return false;
    }

    void virarPecas(std::vector<std::pair<int,int>> posicoes, int referencia) {
        for (auto pares : posicoes) {
            this->getElemento(pares.first, pares.second).setReferencia(referencia);
        }
    }

public:
    Reversi(unsigned int x, unsigned int y, unsigned int id) : JogoTabuleiro(x, y, id) {

        adicionaTilesAtivos();
        adicionaTilesAtivos();
        adicionaTilesAtivos();
        adicionaTilesAtivos();

        this->getElemento(3,3).setAtivo();
        this->getElemento(3,3).setReferencia(1);
        this->getElemento(4,3).setAtivo();
        this->getElemento(4,3).setReferencia(2);
        this->getElemento(3,4).setAtivo();
        this->getElemento(3,4).setReferencia(2);
        this->getElemento(4,4).setAtivo();
        this->getElemento(4,4).setReferencia(1);

        /*this->getElemento(0,0).setReferencia(1);
        this->getElemento(0,0).setAtivo();
        this->getElemento(0,1).setReferencia(2);
        this->getElemento(0,1).setAtivo();*/
    }

    int getPontos(int referencia) const {
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

    bool executaJogada(int x, int y, int referencia) {
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

    std::pair<bool,int> passarVez(int referencia) {
        unsigned int referenciaOposta;
        if (referencia == 1) referenciaOposta = 2;
        else referenciaOposta = 1;
        bool verificaReferencia = this->verificaPosicoesValidas(referencia).first;
        bool verificaReferenciaOposta = this->verificaPosicoesValidas(referenciaOposta).first;

        if ((!verificaReferencia) && (!verificaReferenciaOposta)) return std::make_pair(true, -1);
        if (!referencia) return std::make_pair(true, referenciaOposta);
        return std::make_pair(false, -1);
    }

    std::pair<bool,int> verificaFimJogo() {
        if (this->getTilesAtivos() == this->getY() * this->getX()) {
            if (this->getPontos(1) > this->getPontos(2)) return std::make_pair(true, 1);
            return std::make_pair(true,2);
        }
        return std::make_pair(false,-1);
    }

    void imprimePosicoesValidas(int referencia) {
        std::pair<bool, std::vector<std::pair<int,int>>>
                posicoesValidas = verificaPosicoesValidas(referencia);
        if (!posicoesValidas.first) return;
        for (size_t i=0; i<this->getX(); i++) {
            for (size_t j=0; j<this->getY(); j++) {
                std::pair<int,int> indices = {i,j};
                if (std::find(posicoesValidas.second.begin(), posicoesValidas.second.end(), indices)
                    != posicoesValidas.second.end()) {
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

    void imprimeTabuleiro() {
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

    void imprimePosicoesValidasSFML(sf::RenderWindow &window, sf::Sprite tokenBlack,
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

    void imprimeSFML(sf::RenderWindow &window, sf::Sprite tokenBlack, sf::Sprite &tokenWhite) {
        for (size_t i=0; i<this->getX(); i++) {
            for (size_t j=0; j<this->getY(); j++) {
                if (this->getElemento(i,j).getAtivo()) {
                    if (this->getElemento(i, j).getReferencia() == 1) {
                        tokenBlack.setPosition(
                                sf::Vector2f(55.f + (DIMENSION_TILE * SCALE_TILE * i),
                                             105.f + (DIMENSION_TILE * SCALE_TILE * j)));
                        window.draw(tokenBlack);
                    }
                    else if (this->getElemento(i, j).getReferencia() == 2) {
                        tokenWhite.setPosition(
                                sf::Vector2f(150.f + 17.125 + (DIMENSION_TILE * i),
                                             150.f + 17.125 + (DIMENSION_TILE * j)));
                        window.draw(tokenWhite);
                    }
                }
            }
        }
    }

    int get() { return this->getTilesAtivos();}

    void set(int x, int y, int referencia) {
        this->getElemento(x, y).setAtivo();
        this->getElemento(x, y).setReferencia(referencia);
    }
};

void ResizeView(sf::Window &window, sf::View &view) {
    float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
    view.setSize(VIEW_WIDHT * aspectRatio, VIEW_HEIGHT * aspectRatio);
}

int reversiRun() {
    Reversi reversi(LINHAS_REVERSI, COLUNAS_REVERSI, 2);

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.antialiasingLevel = 8;
    settings.stencilBits = 8;
    sf::RenderWindow window(sf::VideoMode(VIEW_WIDHT, VIEW_HEIGHT),
                            "Reversi", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    sf::View view(sf::Vector2f(0.f, 0.f), sf::Vector2f(VIEW_WIDHT, VIEW_WIDHT));
    sf::Color color(0,0,0);

    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text versusText;
    versusText.setFont(font);
    versusText.setCharacterSize(65);
    versusText.setFillColor(sf::Color::White);
    versusText.setStyle(sf::Text::Bold);
    versusText.setPosition(sf::Vector2f(BOARD_DIMENSION + 400.f, 150.f));
    versusText.setString("Player 1 vs Player 2");

    sf::Text currentPlayer;
    currentPlayer.setFont(font);
    currentPlayer.setCharacterSize(60);
    currentPlayer.setFillColor(sf::Color::White);
    currentPlayer.setStyle(sf::Text::Bold);
    currentPlayer.setPosition(sf::Vector2f(BOARD_DIMENSION + 500.f, BOARD_DIMENSION + 150.f));
    currentPlayer.setString("Jogue Player1 (Black)");

    sf::Text score1, score2;
    score1.setFont(font);
    score1.setCharacterSize(55);
    score1.setFillColor(sf::Color::White);
    score1.setStyle(sf::Text::Bold);
    score1.setPosition(sf::Vector2f(BOARD_DIMENSION + 475.f, 400.f));

    score2.setFont(font);
    score2.setCharacterSize(55);
    score2.setFillColor(sf::Color::White);
    score2.setStyle(sf::Text::Bold);
    score2.setPosition(sf::Vector2f(BOARD_DIMENSION + 475.f, 525.f));

    sf::Texture boardTexture, tokenBlackTexture, tokenWhiteTexture, backgroundTexture;
    sf::Sprite  board, tokenBlack, tokenWhite, background;
    sf::Sprite tokensArray[NUM_TILES][NUM_TILES];

    backgroundTexture.loadFromFile("wood3.jpg");
    background.setTexture(backgroundTexture);
    background.setOrigin(sf::Vector2f(0.f, 0.f));
    background.setPosition(sf::Vector2f(0.f, 0.f));

    boardTexture.loadFromFile("board.gif");
    board.setTexture(boardTexture);
    board.setColor(sf::Color(0,138,0));
    //board.setScale(sf::Vector2f(SCALE_TILE, SCALE_TILE));
    board.setOrigin(sf::Vector2f(0.f, 0.f));
    board.setPosition(sf::Vector2f(150.f, 150.f));

    tokenWhiteTexture.loadFromFile("Tokens.png", sf::IntRect(0, 0, 56, 56));
    tokenBlackTexture.loadFromFile("Tokens.png", sf::IntRect (56, 0, 112, 56));
    tokenWhite.setTexture(tokenWhiteTexture);
    tokenBlack.setTexture(tokenBlackTexture);

    int referencyPlayer = 1;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized: {
                    //ResizeView(window, view);
                    sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
                    window.setView(sf::View(visibleArea));
                }
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if ((event.mouseButton.x > 150.f) &&
                            (event.mouseButton.x < BOARD_DIMENSION + 150.f) &&
                            (event.mouseButton.y > 150.f) &&
                            (event.mouseButton.y < BOARD_DIMENSION + 150.f)) {
                            unsigned int i, j;
                            j = (event.mouseButton.x - 150) / (BOARD_DIMENSION / NUM_TILES);
                            i = (event.mouseButton.y - 150) / (BOARD_DIMENSION / NUM_TILES);
                            if (reversi.executaJogada(i, j, referencyPlayer)) {
                                //reversi.imprimePosicoesValidas(referencyPlayer);
                                if (referencyPlayer == 1) {
                                    referencyPlayer = 2;
                                    currentPlayer.setString("Jogue Player2 (White)");
                                }
                                else {
                                    referencyPlayer = 1;
                                    currentPlayer.setString("Jogue Player1 (Black)");
                                }
                                //std::cout << std::endl;
                                //reversi.imprimeTabuleiro();
                            }
                        }
                        std::cout << "\n\nPASSEI\n\n";
                    }
                    break;
                default:
                    break;
            }
        }

        std::pair<bool,int> endGame;

        endGame = reversi.verificaFimJogo();
        if (endGame.first) {
            std::cout << "\n\nVENCEU!!!!!" << endGame.second << std::endl;
            return 0;
        }

        std::stringstream strScore1, strScore2;
        strScore1 << reversi.getPontos(1);
        strScore2 << reversi.get();
        score1.setString("Player1: " + strScore1.str() + " pontos");
        score2.setString("Player2: " + strScore2.str() + " pontos");

        //std::cout << "\nFALSE\n";
        window.clear(color);
        window.draw(background);
        window.draw(board);
        window.draw(versusText);
        window.draw(currentPlayer);
        window.draw(score1);
        window.draw(score2);
        std::cout << "\n\nPASSEI ANTES\n\n";
        std::pair<bool,int> passarVez = reversi.passarVez(referencyPlayer);
        if (passarVez.first) {
            if (passarVez.second == -1) return 0;
            referencyPlayer = passarVez.second;
        }
        reversi.imprimePosicoesValidasSFML(window, tokenBlack, tokenWhite, referencyPlayer);
        //reversi.imprimePosicoesValidas(referencyPlayer);
        //reversi.imprimeTabuleiro();
        std::cout << "\n\nPASSEI DE NOVO\n\n";
        window.display();
    }
    return 0;
}

class LigQuatro : public JogoTabuleiro {
private:
    std::pair<bool,std::vector<std::pair<int,int>>>
        verificaLinha(int x, int y, int referencia) {
            int j = y + 1;
            int count = 1;
            std::vector<std::pair<int,int>> retorno;
            /* verificar a direita de posicao (x,y) */
            while ((j < this->getY()) && (count < VITORIA_LIG4)) {
                if ((this->getElemento(x,j).getAtivo()) &&
                    (this->getElemento(x,j).getReferencia() == referencia)) {
                    count++;
                    retorno.push_back(std::make_pair(x,j));
                }
                else
                    break;
                j++;
            }
            if (count == VITORIA_LIG4) return std::make_pair(true, retorno);

            j = y - 1;
            /* verificar a esquerda de posicao (x,y) */
            while ((j >= 0) && (count < VITORIA_LIG4)) {
                if ((this->getElemento(x,j).getAtivo()) &&
                    (this->getElemento(x,j).getReferencia() == referencia)) {
                    count++;
                    retorno.push_back(std::make_pair(x,j));
                }
                else
                    break;
                j--;
            }
            if (count == VITORIA_LIG4) return std::make_pair(true, retorno);
            return std::make_pair(false, retorno);
    }

    std::pair<bool,std::vector<std::pair<int,int>>>
        verificaColuna(int x, int y, int referencia) {
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
            if (count == VITORIA_LIG4) return std::make_pair(true, retorno);
            return std::make_pair(false, retorno);
    }
    std::pair<bool,std::vector<std::pair<int,int>>>
        verificaDiagonalPrincipal(int x, int y, int referencia) {
            int i = x + 1, j = y + 1;
            int count = 1;
            std::vector<std::pair<int,int>> retorno;
            /* verificar abaixo da posicao (x,y) a direita */
            while ((i<this->getX()) && (j<this->getY()) && (count<VITORIA_LIG4)) {
                if ((this->getElemento(i,j).getAtivo()) &&
                    (this->getElemento(i,j).getReferencia() == referencia)) {
                    count++;
                    retorno.push_back(std::make_pair(i,j));
                }
                else
                    break;
                i++; j++;
            }
            if (count == VITORIA_LIG4) return std::make_pair(true, retorno);

            i = x - 1, j = y - 1;
            /* verificar acima da posicao (x,y) a esquerda */
            while ((i>=0) && (j>=0) && (count<VITORIA_LIG4)) {
                if ((this->getElemento(i,j).getAtivo()) &&
                    (this->getElemento(i,j).getReferencia() == referencia)) {
                    count++;
                    retorno.push_back(std::make_pair(i,j));
                }
                else
                    break;
                i--; j--;
            }
            if (count == VITORIA_LIG4) return std::make_pair(true, retorno);
            return std::make_pair(false, retorno);
    }

    std::pair<bool,std::vector<std::pair<int,int>>>
        verificaDiagonalSecundaria(int x, int y, int referencia) {
            int i = x + 1, j = y - 1;
            int count = 1;
            std::vector<std::pair<int,int>> retorno;
            /* verificar abaixo da posicao (x,y) a esquerda */
            while ((i<this->getX()) && (j>=0) && (count<VITORIA_LIG4)) {
                if ((this->getElemento(i,j).getAtivo()) &&
                    (this->getElemento(i,j).getReferencia() == referencia)) {
                    count++;
                    retorno.push_back(std::make_pair(i,j));
                }
                else
                    break;
                i++; j--;
            }
            if (count == VITORIA_LIG4) return std::make_pair(true, retorno);

            i = x - 1, j = y + 1;
            /* verificar acima da posicao (x,y) a direita */
            while ((i>=0) && (j<this->getY()) && (count<VITORIA_LIG4)) {
                if ((this->getElemento(i,j).getAtivo()) &&
                    (this->getElemento(i,j).getReferencia() == referencia)) {
                    count++;
                    retorno.push_back(std::make_pair(i,j));
                }
                else
                    break;
                i--; j++;
            }
            if (count == VITORIA_LIG4) return std::make_pair(true, retorno);
            return std::make_pair(false, retorno);
    }

    int verificaJogada(int posicao) {
        if ((posicao >= this->getY()) || (posicao < 0)) return -1;
        for (int i=this->getX()-1; i>=0; i--) {
            if (!this->getElemento(i,posicao).getAtivo()) return i;
        }
        return -1;
    }

    void imprimeTileVitoria(unsigned int x, unsigned int y,
                            sf::Sprite &token, sf::RenderWindow &window) {
        token.setPosition((sf::Vector2f(150.f + (DIM_TILE_LIG * y), 100.f + float(x * 135))));
        window.draw(token);
    }

public:
    LigQuatro(unsigned int x, unsigned int y, unsigned int id) : JogoTabuleiro(x, y, id) {}

    int executaJogada(int coluna, unsigned int referencia) {
        int verificaPos = this->verificaJogada(coluna);
        if (verificaPos == -1) return -1;
        /*this->getElemento(verificaPos,coluna).setAtivo();
        this->getElemento(verificaPos,coluna).setReferencia(referencia);*/
        this->adicionaTilesAtivos();
        return verificaPos;
    }

    bool verificaFimJogo(unsigned  int x, unsigned int y, unsigned int referencia,
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

    void imprimeTabuleiro() {
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

    void imprimeTabuleiroSFML(sf::RenderWindow &window,
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
    
    void imprimeTabuleiroEfeito(sf::RenderWindow &window, sf::Sprite &board,
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
};

int main()
{
    LigQuatro lig4(LINHAS_LIG4, COLUNAS_LIG4, 1);

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.antialiasingLevel = 8;
    settings.stencilBits = 8;
    sf::RenderWindow window(sf::VideoMode(VIEW_WIDHT, VIEW_HEIGHT),
                            "Reversi", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    sf::Color color(sf::Color::Black);

    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text versusText;
    versusText.setFont(font);
    versusText.setCharacterSize(80);
    versusText.setFillColor(sf::Color::White);
    versusText.setStyle(sf::Text::Bold);
    versusText.setPosition(sf::Vector2f(BOARD_DIMENSION + 600.f, 150.f));
    versusText.setString("   Player 1 \n        vs \n   Player 2");

    sf::Text currentPlayer;
    currentPlayer.setFont(font);
    currentPlayer.setCharacterSize(60);
    currentPlayer.setFillColor(sf::Color::White);
    currentPlayer.setStyle(sf::Text::Bold);
    currentPlayer.setPosition(sf::Vector2f(BOARD_LIG_WIDHT + 300.f, BOARD_LIG_HEIGHT));
    currentPlayer.setString("Jogue Player1 (Black)");

    sf::Texture boardTexture, backgroundTexture;
    sf::Texture tokenYellowTexture, tokenRedTexture, tokenYellowXTexture, tokenRedXTexture;
    sf::Sprite  board, background, tokenYellow, tokenRed, tokenYellowX, tokenRedX;
    sf::Sprite tokensArray[NUM_TILES][NUM_TILES];

    backgroundTexture.loadFromFile("wood3.jpg");
    background.setTexture(backgroundTexture);
    background.setOrigin(sf::Vector2f(0.f, 0.f));
    background.setPosition(sf::Vector2f(0.f, 0.f));

    tokenRedTexture.loadFromFile("lig4-tokens-sem-efeito.png", sf::IntRect(0, 0, 135, 135));
    tokenYellowTexture.loadFromFile("lig4-tokens-sem-efeito.png", sf::IntRect(135, 0 ,270, 135));
    tokenRedXTexture.loadFromFile("lig4-tokens-com-efeito.png", sf::IntRect(0, 0, 135, 135));
    tokenYellowXTexture.loadFromFile("lig4-tokens-com-efeito.png", sf::IntRect(135, 0, 270, 135));

    tokenRed.setTexture(tokenRedTexture);
    tokenYellow.setTexture(tokenYellowTexture);
    tokenRedX.setTexture(tokenRedXTexture);
    tokenYellowX.setTexture(tokenYellowXTexture);

    boardTexture.loadFromFile("lig4-board_4.png");
    board.setTexture(boardTexture);
    board.setColor(sf::Color(215,215,215));
    board.setOrigin(sf::Vector2f(0.f, 0.f));
    board.setPosition(sf::Vector2f(150.f, 100.f));

    unsigned int referencyPlayer = 1;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized: {
                    sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
                    window.setView(sf::View(visibleArea));
                }
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if ((event.mouseButton.x > 150.f) &&
                            (event.mouseButton.x < BOARD_LIG_WIDHT + 150.f) &&
                            (event.mouseButton.y > 100.f) &&
                            (event.mouseButton.y < BOARD_LIG_HEIGHT + 100.f)) {
                            int coluna, linha;
                            coluna = (event.mouseButton.x - 150.f) / DIM_TILE_LIG;
                            linha = lig4.executaJogada(coluna, referencyPlayer);
                            std::cout << "\n\n";
                            lig4.imprimeTabuleiro();
                            if (linha != -1) {
                                lig4.imprimeTabuleiroEfeito(window, board, background,
                                                                tokenRed, tokenYellow,
                                                                    versusText, currentPlayer,
                                                                        linha, coluna,
                                                                            referencyPlayer);
                                if (referencyPlayer == 1) {
                                    if (lig4.verificaFimJogo(linha, coluna, referencyPlayer,
                                                                tokenRedX, window))
                                        return 0;
                                    referencyPlayer = 2;
                                    currentPlayer.setString("Jogue Player2 (White)");
                                } else {
                                    if (lig4.verificaFimJogo(linha, coluna, referencyPlayer,
                                                                    tokenYellowX, window))
                                        return 0;
                                    referencyPlayer = 1;
                                    currentPlayer.setString("Jogue Player1 (Black)");
                                }
                            }
                        }
                    }
                    break;
            }
        }
        window.clear(color);
        window.draw(background);
        window.draw(versusText);
        window.draw(currentPlayer);
        lig4.imprimeTabuleiroSFML(window, tokenRed, tokenYellow);
        window.draw(board);
        window.display();
    }
    return 0;
}