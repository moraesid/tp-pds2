#ifndef SISTEMA_H
#define SISTEMA_H

#include <utility>

namespace local {
    std::pair<int,int> reversi();
    std::pair<int,int> ligQuatro();
    void jogar();
    void menu();
}

#endif