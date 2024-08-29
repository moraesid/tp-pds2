#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "cadastro.hpp"

namespace local {
    class Menu {
    private:
        static sf::ContextSettings *propriedadesJanela;
        static sf::RenderWindow *janela;

        sf::Font *fonte;
        sf::View *visualizacao;

    protected:
        static const float alturaJanela; /* altura da janela */
        static const float larguraJanela; /* largura da janela */

        static std::vector<Jogador*> cadastrosArr; /* vetor de jogadores registrados */

        Menu();
        virtual ~Menu();

        sf::Font* getFonte() const;

        void fecharJanela();
        void limparJanela(const sf::Color&);
        void atualizarJanela();

        sf::Vector2u tamanhoJanela() const;

        sf::Vector2i getPosicaoMouse() const;

        bool capturaEvento(sf::Event&);
        bool janelaAberta();
        void redimensionarJanela(const sf::Event&);

        template<class T>
        void desenharObjeto(const T&);

        virtual void desenharTela() = 0;

    public:
        virtual void executarMenu() = 0;
    };

    class MenuHistorico; /* classe definida abaixo */
    class MenuNomes; /* classe definida abaixo */
    class MenuJogos; /* classe definida abaixo */

    class MenuInicial : public Menu {
    private:
        unsigned int posicaoAtual;
        unsigned int posicaoAnterior;
        sf::Texture *textura;
        sf::Sprite *baseTexto;
        sf::Vector2i posicaoMouse;
        std::vector<std::wstring> opcoesMenu;
        std::vector<int> posicaoVerticalOpcoes;
        std::vector<sf::Vector2f> origemOpcoes;
        unsigned int tamanhoOpcoes;
        std::vector<sf::Text> textos;

        std::vector<int> poicaoVerticalMapaMouse;
        std::vector<sf::RectangleShape> mapaMouse;

        MenuHistorico *historico;
        MenuNomes *nomes;
        MenuJogos *jogos;

    protected:
        void redimensionarBaseTexto(const sf::Event&);
        void redimesionarMapaMouse(const sf::Event&);
        void redimensionarTexto(const sf::Event&);
        void loopEventos();
        virtual void desenharTela() override;

    public:
        MenuInicial();
        virtual ~MenuInicial();

        virtual void executarMenu() override;
    };

    class MenuHistorico : public Menu {
    private:
        std::vector<sf::Text> numeroJogadores; /* 1-100 */
        sf::Text *cabecalho; /* Nome, Reversi, Lig4*, respectivamente */
        sf::Text *statsNome; /* nome por jogador */
        sf::Text *statsReversi; /* vitorias Reversi por jogador */
        sf::Text *statsLig; /* vitorias Lig4 por jogador */
        sf::RectangleShape *linhaDivisoria;

        unsigned int topoRegistros;
        sf::Text *vetorVazio; /* texto "Historico Vazio", caso nao tenha jogadores cadastrados */

    protected:
        static const unsigned int maxCadastros;

        void redimensionarTexto(const sf::Event&);

        bool loopEventos();
        virtual void desenharTela() override;

    public:
        MenuHistorico();
        virtual ~MenuHistorico();

        virtual void executarMenu() override;
    };

    class MenuNomes : public Menu {
    private:
        Jogador *jogador1;
        Jogador *jogador2;
        std::string buffer;
        sf::Text *nomeDigitado;
        sf::Text *primeiroJogador;
        sf::Text *segundoJogador;

    protected:
        void redimensionarTexto(const sf::Event&);

        bool loopEventos();
        virtual void desenharTela() override;

    public:
        MenuNomes();
        virtual ~MenuNomes();

        virtual void executarMenu() override;
    };

    class MenuJogos : public Menu {
    private:
        sf::Texture *textura;
        sf::Sprite *baseTexto;

        sf::Text *reversi;
        sf::Text *lig;
        sf::Text *opcoesJogo;

    protected:
        void redimensionarBaseTexto(const sf::Event&);
        void redimesionarMapaMouse(const sf::Event&);
        void redimensionarTexto(const sf::Event&);
        bool loopEventos();
        virtual void desenharTela() override;

    public:
        MenuJogos();
        virtual ~MenuJogos();

        virtual void executarMenu() override;
    };
}
#endif