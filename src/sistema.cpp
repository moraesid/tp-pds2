#include "sistema.hpp"
#include "midia.hpp"
#include "tabuleiro.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <stack>
#include <algorithm>
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
static const unsigned int NUM_TILES = 8;
static const float DIMENSION_TILE = BOARD_DIMENSION / float (NUM_TILES);

static const float BOARD_LIG_HEIGHT = 135.f * 6.f;
static const float BOARD_LIG_WIDHT = 135.f * 7.f;
static const unsigned int COLUNAS_LIG = 7;
static const unsigned int LINHAS_LIG  = 6;
static const float DIM_TILE_LIG = BOARD_LIG_WIDHT / float (COLUNAS_LIG4);

std::pair<int,int> local::reversi() {
    local::Reversi reversi(LINHAS_REVERSI, COLUNAS_REVERSI, 2);

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.antialiasingLevel = 8;
    settings.stencilBits = 8;
    sf::RenderWindow window(sf::VideoMode(VIEW_WIDHT, VIEW_HEIGHT),
                            "Reversi", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    sf::Color color(0, 0, 0);

    sf::Font font;
    local::carregaRecursos(font, "../recursos/arial.ttf");

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
    sf::Sprite board, tokenBlack, tokenWhite, background;
    sf::Sprite tokensArray[NUM_TILES][NUM_TILES];

    local::carregaRecursos(backgroundTexture, "../recursos/wood3.jpg");
    background.setTexture(backgroundTexture);
    background.setOrigin(sf::Vector2f(0.f, 0.f));
    background.setPosition(sf::Vector2f(0.f, 0.f));

    local::carregaRecursos(boardTexture, "../recursos/board.gif");
    board.setTexture(boardTexture);
    board.setColor(sf::Color(0, 138, 0));
    board.setOrigin(sf::Vector2f(0.f, 0.f));
    board.setPosition(sf::Vector2f(150.f, 150.f));

    local::carregaRecursos(tokenWhiteTexture, sf::IntRect(0, 0, 56, 56), "../recursos/Tokens.png");
    local::carregaRecursos(tokenBlackTexture, sf::IntRect(56, 0, 112, 56), "../recursos/Tokens.png");
    tokenWhite.setTexture(tokenWhiteTexture);
    tokenBlack.setTexture(tokenBlackTexture);

    int referencyPlayer = 1;

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
                                if (referencyPlayer == 1) {
                                    referencyPlayer = 2;
                                    currentPlayer.setString("Jogue Player2 (White)");
                                } else {
                                    referencyPlayer = 1;
                                    currentPlayer.setString("Jogue Player1 (Black)");
                                }
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        std::pair<bool, int> endGame;

        endGame = reversi.verificaFimJogo();
        if (endGame.first) return std::make_pair(reversi.getID(), endGame.second);

        std::stringstream strScore1, strScore2;
        strScore1 << reversi.getPontos(1);
        strScore2 << reversi.getPontos(2);
        score1.setString("Player1: " + strScore1.str() + " pontos");
        score2.setString("Player2: " + strScore2.str() + " pontos");

        window.clear(color);
        window.draw(background);
        window.draw(board);
        window.draw(versusText);
        window.draw(currentPlayer);
        window.draw(score1);
        window.draw(score2);
        std::pair<bool, int> passarVez = reversi.passarVez(referencyPlayer);
        if (passarVez.first) {
            if (passarVez.second == -1) return std::make_pair(-1, -1);
            referencyPlayer = passarVez.second;
        }
        reversi.imprimePosicoesValidasSFML(window, tokenBlack, tokenWhite, referencyPlayer);

        window.display();
    }
}

std::pair<int,int> local::ligQuatro() {
    local::LigQuatro lig4(LINHAS_LIG4, COLUNAS_LIG4, 1);

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.antialiasingLevel = 8;
    settings.stencilBits = 8;
    sf::RenderWindow window(sf::VideoMode(VIEW_WIDHT, VIEW_HEIGHT),
                            "Reversi", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    sf::Color color(sf::Color::Black);

    sf::Font font;
    font.loadFromFile("../recursos/arial.ttf");

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

    backgroundTexture.loadFromFile("../recursos/wood3.jpg");
    background.setTexture(backgroundTexture);
    background.setOrigin(sf::Vector2f(0.f, 0.f));
    background.setPosition(sf::Vector2f(0.f, 0.f));

    local::carregaRecursos(tokenRedTexture, sf::IntRect(0, 0, 135, 135),
                           "../recursos/lig4-tokens-sem-efeito.png");
    local::carregaRecursos(tokenYellowTexture, sf::IntRect(135, 0 ,270, 135),
                           "../recursos/lig4-tokens-sem-efeito.png");
    local::carregaRecursos(tokenRedXTexture, sf::IntRect(0, 0, 135, 135),
                           "../recursos/lig4-tokens-com-efeito.png");
    local::carregaRecursos(tokenYellowXTexture, sf::IntRect(135, 0, 270, 135),
                           "../recursos/lig4-tokens-com-efeito.png");

    tokenRed.setTexture(tokenRedTexture);
    tokenYellow.setTexture(tokenYellowTexture);
    tokenRedX.setTexture(tokenRedXTexture);
    tokenYellowX.setTexture(tokenYellowXTexture);

    local::carregaRecursos(boardTexture, "../recursos/lig4-board_4.png");
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
                            linha = lig4.executaJogada(coluna);
                            if (linha != -1) {
                                lig4.imprimeTabuleiroEfeito(window, board, background,
                                                            tokenRed, tokenYellow,
                                                            versusText, currentPlayer,
                                                            linha, coluna,
                                                            referencyPlayer);
                                if (referencyPlayer == 1) {
                                    if (lig4.verificaFimJogo(linha, coluna, referencyPlayer,
                                                             tokenRedX, window))
                                        return std::make_pair(lig4.getID(), referencyPlayer);
                                    referencyPlayer = 2;
                                    currentPlayer.setString("Jogue Player2 (White)");
                                } else {
                                    if (lig4.verificaFimJogo(linha, coluna, referencyPlayer,
                                                             tokenYellowX, window))
                                        return std::make_pair(lig4.getID(), referencyPlayer);
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
}