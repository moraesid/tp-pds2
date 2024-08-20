#include "midia.hpp"
#include <iostream>
#include <cstdlib>

void local::carregaRecursos(sf::Font &fonte, std::string caminho) {
    try {
        fonte.loadFromFile(caminho);
    }
    catch (std::exception &e){
        std::cout << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void local::carregaRecursos(sf::Texture &fonte, std::string caminho) {
    try {
        fonte.loadFromFile(caminho);
    }
    catch (std::exception &e){
        std::cout << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void local::carregaRecursos(sf::Texture &fonte, sf::IntRect faixa, std::string caminho) {
    try {
        fonte.loadFromFile(caminho, faixa);
    }
    catch (std::exception &e){
        std::cout << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}