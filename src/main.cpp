#include "menu.hpp"
#include <cstdlib>

int main() {
    local::MenuInicial *menuInicial = new local::MenuInicial();
    menuInicial->executarMenu();
    delete menuInicial;
    return EXIT_SUCCESS;
}