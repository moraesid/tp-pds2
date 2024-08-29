//
// Created by luis on 23/08/24.
//

#include "menu.hpp"
#include "midia.hpp"
#include "sistema.hpp"

// Definicoes class Menu
const float local::Menu::alturaJanela = 720.f;
const float local::Menu::larguraJanela = 1280.f;

sf::ContextSettings* local::Menu::propriedadesJanela = new sf::ContextSettings();
sf::RenderWindow* local::Menu::janela = new sf::RenderWindow();

std::vector<Jogador*> local::Menu::cadastrosArr;

local::Menu::Menu() {
    local::Menu::propriedadesJanela->depthBits = 24;
    local::Menu::propriedadesJanela->stencilBits = 8;
    local::Menu::propriedadesJanela->antialiasingLevel = 8;

    local::Menu::janela->create(sf::VideoMode(local::Menu::larguraJanela,
                                       local::Menu::alturaJanela), "Jogos de Tabuleiro",
                         sf::Style::Default, *this->propriedadesJanela);

    this->visualizacao = new sf::View(sf::FloatRect(0, 0, local::Menu::larguraJanela,
                                                    local::Menu::alturaJanela));
    this->visualizacao->setCenter(sf::Vector2f(local::Menu::larguraJanela / 2.f,
                                               local::Menu::alturaJanela / 2.f));

    //this->janela->setView(*this->visualizacao);

    this->fonte = new sf::Font();
    local::carregaRecursos(*this->fonte, "../recursos/chalkboard.ttf");
}

local::Menu::~Menu() {
    delete local::Menu::propriedadesJanela;
    delete local::Menu::janela;
    delete this->fonte;
    delete this->visualizacao;
}

sf::Font* local::Menu::getFonte() const {
    return this->fonte;
}

void local::Menu::fecharJanela() { this->janela->close(); }
void local::Menu::atualizarJanela() { this->janela->display(); }
void local::Menu::limparJanela(const sf::Color& cor) { this->janela->clear(cor); }

bool local::Menu::capturaEvento(sf::Event &evento) {
    return local::Menu::janela->pollEvent(evento);
}

template<class T>
void local::Menu::desenharObjeto(const T& objetoDesenhavel) {
    local::Menu::janela->draw(objetoDesenhavel);
}

void local::Menu::redimensionarJanela(const sf::Event &evento) {
    sf::FloatRect areaVisivel(0.f, 0.f, evento.size.width, evento.size.height);
    local::Menu::janela->setView(sf::View(areaVisivel));
}

sf::Vector2i local::Menu::getPosicaoMouse() const {
    return sf::Mouse::getPosition(*this->janela);
}

bool local::Menu::janelaAberta() { return local::Menu::janela->isOpen(); }

sf::Vector2u local::Menu::tamanhoJanela() const { return this->janela->getSize(); }
// fim class Menu

// Definicoes class MenuInical, heranca de class Menu
local::MenuInicial::MenuInicial() : Menu() {
    this->textura = new sf::Texture();
    local::carregaRecursos(*this->textura, "../recursos/base-menu-texto_6.png");

    this->baseTexto = new sf::Sprite();
    this->baseTexto->setTexture(*this->textura);
    this->baseTexto->setOrigin(sf::Vector2f(361.f / 2.f, 342.f / 2.f));
    this->baseTexto->setPosition(sf::Vector2f(local::Menu::larguraJanela / 2.f,
                                              local::Menu::alturaJanela / 2.f));

    this->posicaoMouse = {0, 0};

    this->opcoesMenu = {L"Jogar", L"Histórico", L"Sair"};
    this->textos.resize(3);
    this->posicaoVerticalOpcoes = {-120, 0, 120};
    this->origemOpcoes = {{153.f / 2.f, 76.f / 2.f},
                          {235.f / 2.f, 76.f / 2.f},
                          {109.f / 2.f, 76.f / 2.f}};
    this->tamanhoOpcoes = 60;

    for (size_t i=0; i<this->textos.size(); i++) {
        this->textos[i].setFont(*this->getFonte());
        this->textos[i].setString(this->opcoesMenu[i]);
        this->textos[i].setCharacterSize(this->tamanhoOpcoes);
        this->textos[i].setFillColor(sf::Color(254, 254, 253));
        this->textos[i].setOutlineColor(sf::Color(251, 225, 0));
        this->textos[i].setOrigin({this->origemOpcoes[i]});
        this->textos[i].setPosition(local::Menu::larguraJanela / 2.f,
                                    (local::Menu::alturaJanela / 2.f) +
                                    this->posicaoVerticalOpcoes[i]);
    }
    this->textos[0].setOutlineThickness(4);
    this->posicaoAtual = 0;
    this->posicaoAnterior = 0;

    this->poicaoVerticalMapaMouse = {-107, 0, 107};
    this->mapaMouse.resize(3);

    for (size_t i=0; i< this->mapaMouse.size(); i++) {
        this->mapaMouse[i].setSize(sf::Vector2f(360.f, 107.f));
        this->mapaMouse[i].setOrigin(sf::Vector2f(360.f / 2.f, 107.f / 2.f));
        this->mapaMouse[i].setPosition(sf::Vector2f(local::Menu::larguraJanela / 2.f,
                                                    (local::Menu::alturaJanela / 2.f) +
                                                    this->poicaoVerticalMapaMouse[i]));
        this->mapaMouse[i].setFillColor(sf::Color::Transparent);
    }

    this->historico = new local::MenuHistorico();
    this->nomes = new local::MenuNomes();
    this->jogos = new local::MenuJogos();
}

local::MenuInicial::~MenuInicial() {
    delete this->textura;
    delete this->baseTexto;
    delete this->historico;
    delete this->nomes;
    delete this->jogos;
}

void local::MenuInicial::loopEventos() {
    sf::Event evento;
    while (this->capturaEvento(evento)) {
        switch (evento.type) {
            case sf::Event::Closed:
                this->fecharJanela();
                break;
            case sf::Event::Resized:
                this->redimensionarJanela(evento);
                this->redimensionarBaseTexto(evento);
                this->redimesionarMapaMouse(evento);
                this->redimensionarTexto(evento);
                break;
            case sf::Event::MouseMoved:
                this->posicaoMouse = this->getPosicaoMouse();
                for (size_t i=0; i<this->mapaMouse.size(); i++) {
                    if (this->mapaMouse[i].getGlobalBounds().contains(this->posicaoMouse.x,
                                                                      this->posicaoMouse.y)) {
                        this->posicaoAnterior = this->posicaoAtual;
                        this->posicaoAtual = i;
                        if (this->posicaoAnterior != this->posicaoAtual) {
                            this->textos[this->posicaoAtual].setOutlineThickness(4);
                            this->textos[this->posicaoAnterior].setOutlineThickness(0);
                        }
                    }
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (evento.mouseButton.button == sf::Mouse::Left) {
                    for (auto rect : this->mapaMouse) {
                        if (rect.getGlobalBounds().contains(this->posicaoMouse.x,
                                                            this->posicaoMouse.y)) {
                            switch (this->posicaoAtual) {
                                case 0: {
                                    this->nomes->executarMenu();
                                    this->jogos->executarMenu();

                                    sf::Event data;
                                    sf::Vector2u tamanho = this->tamanhoJanela();
                                    data.type = sf::Event::Resized;
                                    data.size.width = tamanho.x;
                                    data.size.height = tamanho.y;
                                    this->redimensionarJanela(data);
                                    this->redimensionarTexto(data);
                                    this->redimesionarMapaMouse(data);
                                    this->redimensionarBaseTexto(data);
                                }
                                    break;
                                case 1: {
                                    lerCadastros("teste.csv", local::Menu::cadastrosArr);
                                    this->historico->executarMenu();
                                    sf::Event data;
                                    sf::Vector2u tamanho = this->tamanhoJanela();
                                    data.type = sf::Event::Resized;
                                    data.size.width = tamanho.x;
                                    data.size.height = tamanho.y;
                                    this->redimensionarJanela(data);
                                    this->redimensionarTexto(data);
                                    this->redimesionarMapaMouse(data);
                                    this->redimensionarBaseTexto(data);
                                }
                                    break;
                                case 2:
                                    this->fecharJanela();
                                    break;
                            }
                            break;
                        }
                    }
                }
                break;
            case sf::Event::KeyPressed:
                if (evento.key.code == sf::Keyboard::Up && this->posicaoAtual > 0) {
                    this->posicaoAnterior = this->posicaoAtual;
                    this->posicaoAtual--;
                    this->textos[this->posicaoAtual].setOutlineThickness(4);
                    this->textos[this->posicaoAtual + 1].setOutlineThickness(0);
                }
                else if (evento.key.code == sf::Keyboard::Down && this->posicaoAtual < 2) {
                    this->posicaoAnterior = this->posicaoAtual;
                    this->posicaoAtual++;
                    this->textos[this->posicaoAtual].setOutlineThickness(4);
                    this->textos[this->posicaoAtual - 1].setOutlineThickness(0);
                }
                else if (evento.key.code == sf::Keyboard::Enter) {
                    switch (this->posicaoAtual) {
                        case 0: {
                            this->nomes->executarMenu();
                            this->jogos->executarMenu();

                            sf::Event data;
                            sf::Vector2u tamanho = this->tamanhoJanela();
                            data.type = sf::Event::Resized;
                            data.size.width = tamanho.x;
                            data.size.height = tamanho.y;
                            this->redimensionarJanela(data);
                            this->redimensionarTexto(data);
                            this->redimesionarMapaMouse(data);
                            this->redimensionarBaseTexto(data);
                        }
                            break;
                        case 1: {
                            lerCadastros("teste.csv", local::Menu::cadastrosArr);
                            this->historico->executarMenu();
                            sf::Event data;
                            sf::Vector2u tamanho = this->tamanhoJanela();
                            data.type = sf::Event::Resized;
                            data.size.width = tamanho.x;
                            data.size.height = tamanho.y;
                            this->redimensionarJanela(data);
                            this->redimensionarTexto(data);
                            this->redimesionarMapaMouse(data);
                            this->redimensionarBaseTexto(data);
                        }
                            break;
                        case 2:
                            this->fecharJanela();
                            break;
                    }
                }
                break;
        }
    }
}

void local::MenuInicial::redimensionarBaseTexto(const sf::Event &evento) {
    this->baseTexto->setPosition(sf::Vector2f(evento.size.width / 2.f,
                                              evento.size.height / 2.f));
}

void local::MenuInicial::redimensionarTexto(const sf::Event &evento) {
    for (size_t i=0; i<this->textos.size(); i++) {
        textos[i].setPosition(evento.size.width / 2.f,
                              (evento.size.height / 2.f) + this->posicaoVerticalOpcoes[i]);
    }
}

void local::MenuInicial::redimesionarMapaMouse(const sf::Event &evento) {
    for (size_t i=0; i<this->mapaMouse.size(); i++) {
        mapaMouse[i].setPosition(evento.size.width / 2.f,
                              (evento.size.height / 2.f) + this->poicaoVerticalMapaMouse[i]);
    }
}

void local::MenuInicial::desenharTela() {
    this->limparJanela(sf::Color(21, 54, 59));
    this->desenharObjeto(*this->baseTexto);
    for (auto texto : textos) {
        this->desenharObjeto(texto);
    }
    this->atualizarJanela();
}

void local::MenuInicial::executarMenu() {
    while (this->janelaAberta()) {
        this->loopEventos();
        this->desenharTela();
    }
}
// fim class MenuInicial

// Definicoes class MenuHistorico, heranca de class Menu
const unsigned int local::MenuHistorico::maxCadastros = 100;

local::MenuHistorico::MenuHistorico() {
    this->cabecalho = new sf::Text();
    this->cabecalho->setFont(*this->getFonte());
    this->cabecalho->setStyle(sf::Text::Bold);
    this->cabecalho->setFillColor(sf::Color(251, 225, 0));
    this->cabecalho->setCharacterSize(45);
    this->cabecalho->setPosition(sf::Vector2f(310.f, 20.f));
    this->cabecalho->setString("Apelido" + std::string(28, ' ') +
                               "Reversi" + std::string(28, ' ') + "Lig4");

    this->statsNome = new sf::Text();
    this->statsNome->setFont(*this->getFonte());
    this->statsNome->setStyle(sf::Text::Bold);
    this->statsNome->setCharacterSize(40);
    this->statsNome->setFillColor(sf::Color(251, 225, 0));

    this->statsReversi = new sf::Text();
    this->statsReversi->setFont(*this->getFonte());
    this->statsReversi->setStyle(sf::Text::Bold);
    this->statsReversi->setCharacterSize(40);
    this->statsReversi->setFillColor(sf::Color(251, 225, 0));

    this->statsLig = new sf::Text();
    this->statsLig->setFont(*this->getFonte());
    this->statsLig->setStyle(sf::Text::Bold);
    this->statsLig->setCharacterSize(40);
    this->statsLig->setFillColor(sf::Color(251, 225, 0));

    this->linhaDivisoria = new sf::RectangleShape(sf::Vector2f(1820.f, 2.f));
    this->linhaDivisoria->setFillColor(sf::Color(251, 225, 0));
    this->linhaDivisoria->setPosition(sf::Vector2f(50.f, 90.f));

    this->numeroJogadores.resize(local::MenuHistorico::maxCadastros);
    for (size_t i=0; i<this->numeroJogadores.size(); i++) {
        this->numeroJogadores[i].setFont(*this->getFonte());
        this->numeroJogadores[i].setFillColor(sf::Color(251, 225, 0));
        this->numeroJogadores[i].setCharacterSize(50);
        this->numeroJogadores[i].setStyle(sf::Text::Bold);
    }

    this->topoRegistros = 0;

    this->vetorVazio = new sf::Text();
    this->vetorVazio->setFont(*this->getFonte());
    this->vetorVazio->setFillColor(sf::Color(251, 225, 0));
    this->vetorVazio->setCharacterSize(120);
    this->vetorVazio->setStyle(sf::Text::Bold);
    this->vetorVazio->setString(L"Histórico Vazio");
    this->vetorVazio->setOrigin(sf::Vector2f(790.f/ 2.f, 153.f / 2.f));
    this->vetorVazio->setPosition(local::Menu::larguraJanela / 2.f,
                                  local::Menu::alturaJanela / 2.f);

}

local::MenuHistorico::~MenuHistorico() {
    delete this->cabecalho;
    delete this->linhaDivisoria;
    delete this->vetorVazio;
    delete this->statsNome;
    delete this->statsReversi;
    delete this->statsLig;
}

bool local::MenuHistorico::loopEventos() {
    sf::Event evento;
    while (this->capturaEvento(evento)) {
        switch (evento.type) {
            case sf::Event::Closed:
                this->fecharJanela();
                break;
            case sf::Event::Resized:
                this->redimensionarTexto(evento);
                this->redimensionarJanela(evento);
                break;
            case sf::Event::KeyPressed:
                if (evento.key.code == sf::Keyboard::Up && this->topoRegistros > 0)
                    this->topoRegistros--;
                else if (evento.key.code == sf::Keyboard::Down &&
                            this->topoRegistros <= 90)
                    this->topoRegistros++;
                else if (evento.key.code == sf::Keyboard::Escape) return false;
                break;
            case sf::Event::MouseWheelScrolled:
                if (evento.mouseWheelScroll.delta < 0 && this->topoRegistros <= 90)
                    this->topoRegistros++;
                else if (evento.mouseWheelScroll.delta > 0 && this->topoRegistros > 0)
                    this->topoRegistros--;
                break;
        }
    }
    return true;
}

void local::MenuHistorico::desenharTela() {
    this->limparJanela(sf::Color(64, 64, 64));
    if (local::Menu::cadastrosArr.size() > 0) {
        this->desenharObjeto(*this->cabecalho);
        this->desenharObjeto(*this->linhaDivisoria);
        unsigned int iterador = 0;
        for (size_t i=this->topoRegistros; i<local::MenuHistorico::maxCadastros; i++) {
            if (i < local::Menu::cadastrosArr.size()) {
                this->statsNome->setString(local::Menu::cadastrosArr[i]->getNome());
                this->statsLig->setString(std::to_string(
                        local::Menu::cadastrosArr[i]->getVitoriasLig4()));
                this->statsReversi->setString(std::to_string(
                        local::Menu::cadastrosArr[i]->getVitoriasReversi()));

                this->statsNome->setPosition(sf::Vector2f(320.f, 120.f + (100.f * iterador)));
                this->statsReversi->setPosition(
                        sf::Vector2f( 320.f + 780.f -
                        (this->statsReversi->getLocalBounds()).width,
                        120.f + (100.f * iterador)) );
                this->statsLig->setPosition(
                        sf::Vector2f(320.f + 1400.f -
                        (this->statsLig->getLocalBounds()).width,
                        120.f + (100.f * iterador)) );

                this->desenharObjeto(*this->statsNome);
                this->desenharObjeto(*this->statsLig);
                this->desenharObjeto(*this->statsReversi);
            }
            this->numeroJogadores[i].setPosition(100.f, 120.f + (100.f * iterador));
            this->numeroJogadores[i].setString(std::to_string(i+1) + '.');
            this->desenharObjeto(this->numeroJogadores[i]);
            iterador++;
        }
    }
    else {
        this->desenharObjeto(*this->vetorVazio);
    }
    this->atualizarJanela();
}

void local::MenuHistorico::redimensionarTexto(const sf::Event& evento) {
    this->vetorVazio->setPosition(sf::Vector2f(evento.size.width / 2.f ,
                                               evento.size.height / 2.f));
}

void local::MenuHistorico::executarMenu() {
    while (this->janelaAberta()) {
        if (!this->loopEventos()) return;
        this->desenharTela();
    }
}
// fim class MenuHistorico

// Definicoes class MenuNomes, heranca de class Menu
local::MenuNomes::MenuNomes() {
    this->jogador1 = new Jogador();
    this->jogador2 = new Jogador();
    this->nomeDigitado = new sf::Text();
    this->primeiroJogador = new sf::Text();
    this->segundoJogador = new sf::Text();

    this->primeiroJogador->setFont(*this->getFonte());
    this->primeiroJogador->setFillColor(sf::Color(251, 225, 0));
    this->primeiroJogador->setCharacterSize(60);
    this->primeiroJogador->setStyle(sf::Text::Italic);
    this->primeiroJogador->setOrigin(sf::Vector2f(501.f / 2.f, 76.f / 2.f));
    this->primeiroJogador->setString(" Primeiro Jogador: ");
    this->primeiroJogador->setPosition(sf::Vector2f(local::Menu::larguraJanela / 2.f -
                                            this->primeiroJogador->getLocalBounds().width / 2.f,
                                                    local::Menu::alturaJanela / 2.f - 2 * 76.f));

    this->segundoJogador->setFont(*this->getFonte());
    this->segundoJogador->setFillColor(sf::Color(251, 225, 0));
    this->segundoJogador->setCharacterSize(60);
    this->segundoJogador->setStyle(sf::Text::Italic);
    this->segundoJogador->setOrigin(sf::Vector2f(465.f / 2.f, 76.f / 2.f));
    this->segundoJogador->setString("Segundo Jogador: ");
    this->segundoJogador->setPosition(sf::Vector2f(local::Menu::larguraJanela / 2.f -
                                        this->segundoJogador->getLocalBounds().width / 2.f,
                                                     local::Menu::alturaJanela / 2.f + 76.f));

    this->nomeDigitado->setFont(*this->getFonte());
    this->nomeDigitado->setFillColor(sf::Color::White);
    this->nomeDigitado->setStyle(sf::Text::Underlined);
    this->nomeDigitado->setCharacterSize(60);
    this->nomeDigitado->setOrigin(sf::Vector2f(231.f / 2.f, 74.f / 2.f));
    this->nomeDigitado->setPosition(sf::Vector2f(local::Menu::larguraJanela / 2.f -
                                        this->primeiroJogador->getLocalBounds().width / 2.f +
                                        (465.f / 1.2f),
                                        local::Menu::alturaJanela / 2.f - 2 * 76.f));
}

local::MenuNomes::~MenuNomes() {
    delete this->jogador1;
    delete this->jogador2;
    delete this->nomeDigitado;
    delete this->primeiroJogador;
    delete this->segundoJogador;
}

bool local::MenuNomes::loopEventos() {
    sf::Event evento;
    while (this->capturaEvento(evento)) {
        switch (evento.type) {
            case sf::Event::Closed:
                this->fecharJanela();
                break;
            case sf::Event::Resized:
                this->redimensionarJanela(evento);
                this->redimensionarTexto(evento);
                break;
            case sf::Event::KeyPressed:
                if (evento.key.code == sf::Keyboard::BackSpace && !this->buffer.empty()) {
                    this->buffer.pop_back();
                    this->nomeDigitado->setString(this->buffer);
                }
                else if (evento.key.code == sf::Keyboard::Enter) {
                    if (this->buffer.size() >= 3) {
                        if (this->jogador1->getNome().size() == 0) {
                            this->jogador1->setNome(this->buffer);
                            this->buffer.clear();
                            this->nomeDigitado->setString(this->buffer);
                            this->primeiroJogador->setString(" Primeiro Jogador: " +
                                                             this->jogador1->getNome());
                            this->nomeDigitado->setPosition(sf::Vector2f(
                                    local::Menu::larguraJanela / 2.f -
                                    this->segundoJogador->getLocalBounds().width / 2.f +
                                    (465.f / 1.2f), local::Menu::alturaJanela / 2.f + 76.f));

                            sf::Event data;
                            sf::Vector2u tamanho = this->tamanhoJanela();
                            data.type = sf::Event::Resized;
                            data.size.width = tamanho.x;
                            data.size.height = tamanho.y;
                            this->redimensionarJanela(data);
                            this->redimensionarTexto(data);
                        }
                        else if (this->jogador2->getNome().size() == 0 && this->buffer !=
                                    this->jogador1->getNome()) {
                            this->jogador2->setNome(this->buffer);
                            this->buffer.clear();
                            this->nomeDigitado->setString(this->buffer);
                            this->segundoJogador->setString("Segundo Jogador: " +
                                                             this->jogador2->getNome());

                            sf::Event data;
                            sf::Vector2u tamanho = this->tamanhoJanela();
                            data.type = sf::Event::Resized;
                            data.size.width = tamanho.x;
                            data.size.height = tamanho.y;
                            this->redimensionarJanela(data);
                            this->redimensionarTexto(data);

                            return false;
                        }
                    }
                }
                else if (evento.key.code == sf::Keyboard::Escape) return false;
                break;
            case sf::Event::TextEntered:
                if ((evento.text.unicode <= 122 && evento.text.unicode >= 97) ||
                    (evento.text.unicode >= 48 && evento.text.unicode <= 57)) {
                    if (this->buffer.size() < 8) {
                        this->buffer += evento.text.unicode;
                        this->nomeDigitado->setString(buffer);
                    }
                }
                break;
        }
    }
    return true;
}

void local::MenuNomes::desenharTela() {
    this->limparJanela(sf::Color(39, 40 ,34));
    this->desenharObjeto(*this->nomeDigitado);
    this->desenharObjeto(*this->primeiroJogador);
    this->desenharObjeto(*this->segundoJogador);
    if (this->jogador1->getNome().size() > 0 && this->jogador1->getNome() == this->buffer) {
        sf::Text nomesIguais;
        nomesIguais.setFont(*this->getFonte());
        nomesIguais.setCharacterSize(60);
        nomesIguais.setFillColor(sf::Color(251, 225, 0));
        nomesIguais.setStyle(sf::Text::Italic);
        nomesIguais.setOrigin(sf::Vector2f(359.f / 2.f, 76.f / 2.f));
        nomesIguais.setPosition(sf::Vector2f(this->tamanhoJanela().x / 2.f,
                                             this->tamanhoJanela().y - 76.f / 2.f));
        nomesIguais.setString("Nomes Iguais!");

        this->desenharObjeto(nomesIguais);
    }
    this->atualizarJanela();
}

void local::MenuNomes::executarMenu() {
    while (this->janelaAberta()) {
        if (!this->loopEventos()) return;
        this->desenharTela();
    }
}

void local::MenuNomes::redimensionarTexto(const sf::Event& evento) {
    if (this->jogador1->getNome().size() == 0) {
        this->primeiroJogador->setPosition(
                sf::Vector2f(evento.size.width / 2.f -
                this->primeiroJogador->getLocalBounds().width /
                2.f, evento.size.height / 2.f - 2 * 76.f) );
    }
    else {
        this->primeiroJogador->setPosition(
                sf::Vector2f(evento.size.width / 2.f -
                             this->primeiroJogador->getLocalBounds().width /
                             4.f, evento.size.height / 2.f - 2 * 76.f) );
    }

    if (this->jogador2->getNome().size() == 0) {
        this->segundoJogador->setPosition(
                sf::Vector2f(evento.size.width / 2.f -
                this->segundoJogador->getLocalBounds().width /
                2.f, evento.size.height / 2.f + 76.f) );
    }
    else {
        this->segundoJogador->setPosition(
                sf::Vector2f(evento.size.width / 2.f -
                             this->segundoJogador->getLocalBounds().width /
                             4.f, evento.size.height / 2.f + 76.f) );
    }

    if (this->jogador1->getNome().size() == 0) {
        this->nomeDigitado->setPosition(
                sf::Vector2f(evento.size.width / 2.f -
                this->primeiroJogador->getLocalBounds().width /
                2.f + (465.f / 1.2f), evento.size.height / 2.f - 2 * 76.f) );
    }
    else {
        this->nomeDigitado->setPosition(
                sf::Vector2f( evento.size.width / 2.f -
                this->segundoJogador->getLocalBounds().width /
                2.f + (465.f / 1.2f), evento.size.height / 2.f + 76.f) );
    }

}
// fim class MenuNomes

// Definicoes class MenuJogos, heranca de class Menu
local::MenuJogos::MenuJogos() {
    this->textura = new sf::Texture();
    local::carregaRecursos(*this->textura, "../recursos/base-menu_3_composto.png");

    this->baseTexto = new sf::Sprite();
    this->baseTexto->setTexture(*this->textura);
    this->baseTexto->setOrigin(sf::Vector2f(383.f / 2.f, 473.f / 2.f));
    this->baseTexto->setPosition(sf::Vector2f(local::Menu::larguraJanela / 2.f,
                                              local::Menu::alturaJanela / 2.f));

    this->reversi = new sf::Text();
    this->lig = new sf::Text();
    this->opcoesJogo = new sf::Text();

    this->reversi->setFont(*this->getFonte());
    this->reversi->setFillColor(sf::Color(254, 254, 253));
    this->reversi->setOutlineThickness(4);
    this->reversi->setOutlineColor(sf::Color::Black);
    this->reversi->setCharacterSize(60);
    this->reversi->setOrigin(sf::Vector2f(241.f / 2.f, 76.f / 2.f));
    this->reversi->setPosition(sf::Vector2f(local::Menu::larguraJanela / 2.f,
                                             local::Menu::alturaJanela / 2.f + (473.f / 3.2f)));
    this->reversi->setString("REVERSI");

    this->lig->setFont(*this->getFonte());
    this->lig->setFillColor(sf::Color(254, 254, 253));
    this->lig->setOutlineThickness(4);
    this->lig->setOutlineColor(sf::Color::Black);
    this->lig->setCharacterSize(60);
    this->lig->setOrigin(sf::Vector2f(159.f / 2.f, 76.f / 2.f));
    this->lig->setPosition(sf::Vector2f(local::Menu::larguraJanela / 2.f,
                                        local::Menu::alturaJanela / 2.f - (473.f / 3.2f)));
    this->lig->setString("LIG-4");

    this->opcoesJogo->setFont(*this->getFonte());
    this->opcoesJogo->setFillColor(sf::Color(254, 254, 253));
    this->opcoesJogo->setCharacterSize(60);
    this->opcoesJogo->setOrigin(sf::Vector2f(814.f / 2.f, 76.f / 2.f));
    this->opcoesJogo->setPosition(sf::Vector2f(local::Menu::larguraJanela / 2.f,
                                               local::Menu::alturaJanela - (76.f / 2.f)));
    this->opcoesJogo->setString("Digite: 1 - Lig-4 / 2 - Reversi");
}

local::MenuJogos::~MenuJogos() {
    delete this->textura;
    delete this->baseTexto;
    delete this->reversi;
    delete this->lig;
    delete this->opcoesJogo;
}

bool local::MenuJogos::loopEventos() {
    sf::Event evento;
    while (this->capturaEvento(evento)) {
        switch (evento.type) {
            case sf::Event::Closed:
                this->fecharJanela();
                break;
            case sf::Event::Resized:
                this->redimensionarJanela(evento);
                this->redimensionarTexto(evento);
                this->redimensionarBaseTexto(evento);
                break;
            case sf::Event::KeyPressed:
                if (evento.key.code == sf::Keyboard::Escape) return false;
                break;
            case sf::Event::TextEntered:
                if (evento.key.code == 114) local::reversi();
                else if (evento.key.code == 108) local::ligQuatro();
                break;
        }
    }

    return true;
}

void local::MenuJogos::desenharTela() {
    this->limparJanela(sf::Color(39, 40 ,34));
    this->desenharObjeto(*this->baseTexto);
    this->desenharObjeto(*this->reversi);
    this->desenharObjeto(*this->lig);
    this->desenharObjeto(*this->opcoesJogo);
    this->atualizarJanela();
}

void local::MenuJogos::executarMenu() {
    while (this->janelaAberta()) {
        if (!this->loopEventos()) return;
        this->desenharTela();
    }
}

void local::MenuJogos::redimensionarBaseTexto(const sf::Event &evento) {
    this->baseTexto->setPosition(sf::Vector2f(evento.size.width / 2.f,
                                              evento.size.height / 2.f));
}

void local::MenuJogos::redimensionarTexto(const sf::Event& evento) {
    this->reversi->setPosition(sf::Vector2f(evento.size.width / 2.f,
                                            evento.size.height / 2.f + (473.f / 3.2f)));

    this->lig->setPosition(sf::Vector2f(evento.size.width / 2.f,
                                            evento.size.height / 2.f - (473.f / 3.2f)));

    this->opcoesJogo->setPosition(sf::Vector2f(evento.size.width / 2.f,
                                               evento.size.height - (76.f / 1.5f)));
}
