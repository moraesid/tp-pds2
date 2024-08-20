#ifndef MIDIA_H
#define MIDIA_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <string>

namespace local {
    void carregaRecursos(sf::Texture &fonte, sf::IntRect faixa, std::string caminho);
    void carregaRecursos(sf::Font &fonte, std::string caminho);
    void carregaRecursos(sf::Texture &fonte, std::string caminho);
}

#endif