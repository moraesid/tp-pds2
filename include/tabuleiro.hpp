#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <string>
#include <utility>
#include <vector>
#include <stack>
#include <algorithm>
#include <SFML/Graphics.hpp>

namespace local {

/* class TileMatriz representa cada posicao (i,j) de uma matriz NxM*/
    class TileMatriz {
    private:
        bool ativo; /* posicao ocupada por uma peca ou nao */
        unsigned int referenciaJogador; /* especifica qual jogador ocupa a posicao */

    public:
        TileMatriz();

        bool getAtivo() const;
        void setAtivo();
        unsigned int getReferencia() const;
        void setReferencia(unsigned int numJogador);
    };

    class JogoTabuleiro {
    private:
        TileMatriz **tabuleiro;
        int dimX, dimY; /* dimensoes do tabuleiro */
        int tilesAtivos; /* numero de posicoes ocupadas no tabuleiro */
        int idJogo;

    public:
        JogoTabuleiro();
        JogoTabuleiro(unsigned int x, unsigned int y, unsigned int id);
        ~JogoTabuleiro();

        unsigned int getID() const;
        unsigned int getX() const;
        unsigned int getY() const;
        TileMatriz &getElemento(unsigned int x, unsigned y) const;

        void adicionaTilesAtivos();
        void adicionaTilesAtivos(unsigned int valor);
        unsigned int getTilesAtivos() const;

        virtual std::pair<bool,std::vector<std::pair<int,int>>>
            verificaLinha(int x, int y, int referencia) = 0;
        virtual std::pair<bool,std::vector<std::pair<int,int>>>
            verificaColuna(int x, int y, int referencia) = 0;
        virtual std::pair<bool,std::vector<std::pair<int,int>>>
            verificaDiagonalPrincipal(int x, int y, int referencia) = 0;
        virtual std::pair<bool,std::vector<std::pair<int,int>>>
            verificaDiagonalSecundaria(int x, int y, int referencia) = 0;
    };

    class LigQuatro : public JogoTabuleiro {
    private:
        virtual std::pair<bool,std::vector<std::pair<int,int>>>
            verificaLinha(int x, int y, int referencia) override;
        virtual std::pair<bool,std::vector<std::pair<int,int>>>
            verificaColuna(int x, int y, int referencia) override;
        virtual std::pair<bool,std::vector<std::pair<int,int>>>
            verificaDiagonalPrincipal(int x, int y, int referencia) override;
        virtual std::pair<bool,std::vector<std::pair<int,int>>>
            verificaDiagonalSecundaria(int x, int y, int referencia) override;

        int verificaJogada(int posicao) const;
        void imprimeTileVitoria(unsigned int x, unsigned int y,
                                sf::Sprite &token, sf::RenderWindow &window);

    public:
        LigQuatro();
        LigQuatro(unsigned int x, unsigned int y, unsigned int id);

        int executaJogada(int coluna);
        bool verificaFimJogo(unsigned  int x, unsigned int y, unsigned int referencia,
                                sf::Sprite &token, sf::RenderWindow &window);
        void imprimeTabuleiroSFML(sf::RenderWindow &window,
                                  sf::Sprite &tokenRed, sf::Sprite &tokenYellow);
        void imprimeTabuleiroEfeito(sf::RenderWindow &window, sf::Sprite &board,
                                    sf::Sprite &background, sf::Sprite &tokenRed,
                                    sf::Sprite &tokenYellow, sf::Text &versusText,
                                    sf::Text &currentPlayer, unsigned int x,
                                    unsigned int y, unsigned int referencia);
    };

    class Reversi : public JogoTabuleiro {
    private:
        virtual std::pair<bool, std::vector<std::pair < int, int>>>
            verificaLinha(int x, int y, int referencia) override;
        virtual std::pair<bool, std::vector<std::pair < int, int>>>
            verificaColuna(int x, int y, int referencia) override;
        virtual std::pair<bool, std::vector<std::pair < int, int>>>
            verificaDiagonalPrincipal(int x, int y, int referencia) override;
        virtual std::pair<bool, std::vector<std::pair < int, int>>>
            verificaDiagonalSecundaria(int x, int y, int referencia) override;

        std::pair<bool, std::vector<std::pair<int,int>>>
            verificaPosicoesValidas(int referencia);

        bool verificaJogada(int x, int y, int referencia);
        void virarPecas(std::vector <std::pair<int, int>> posicoes, int referencia);

    public:
        Reversi();
        Reversi(unsigned int x, unsigned int y, unsigned int id);

        int getPontos(int referencia) const;
        bool executaJogada(int x, int y, int referencia);
        std::pair<bool,int> passarVez(int referencia);
        std::pair<bool, int> verificaFimJogo() const;
        void imprimePosicoesValidasSFML(sf::RenderWindow &window, sf::Sprite &tokenBlack,
                              sf::Sprite &tokenWhite, int referencia);
    };

}

#endif