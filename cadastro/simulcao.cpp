//
// Created by luis on 22/08/24.
//

menuInicial(Jogador&, Jogador&, vector&) {

}

/* atualizar historico, e sair ? exit() : return */
/* ou menuFinal return bool e tratar no loop do main*/
menuFinal(Jogador&, Jogador&, vector&) {

}

void jogar(Jogador&, Jogador&) {
}

int main() {
    Jogador jogador1, jogador2; /* adicionar atributo bool para identificar vitoria */
    vector cadastrosArr;

    lerCadastros(__path__for__file__, cadastrosArr);

    while (true) {
        menuInicial(jogador1, jogador2, cadastrosArr/* para tela de historico*/);
        jogar(jogador1, jogador2);
        menuFinal(jogador1, jogador2, cadastrosArr);
    }
    return 0;
}