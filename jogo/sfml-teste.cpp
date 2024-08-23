#include <SFML/Graphics.hpp>
#include <vector>

static const float VIEW_HEIGHT = 720.f;
static const float VIEW_WIDHT  = 720.f;
static const float BOARD_DIMENSION = 722.f;
static const float FIX_POSITION_WIDHT  = (VIEW_WIDHT - BOARD_DIMENSION) / 2.f;
static const float FIX_POSITION_HEIGHT = (VIEW_HEIGHT - BOARD_DIMENSION) / 2.f;
static const float FIX_POSITION_TOKEN = 5.f; /* board scale 0.7 */
static const unsigned int NUM_TILES = 8;
static const float DIMENSION_TILE = BOARD_DIMENSION / float (NUM_TILES);
static const float SCALE_TILE = 0.7; /* window dimension 720x720 */

void ResizeView(sf::Window &window, sf::View &view) {
    float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
    view.setSize(VIEW_WIDHT * aspectRatio, VIEW_HEIGHT * aspectRatio);
}

int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.antialiasingLevel = 8;
    settings.stencilBits = 8;
    sf::RenderWindow window(sf::VideoMode(VIEW_WIDHT, VIEW_HEIGHT),
                            "SFML works!", sf::Style::Default, settings);
    sf::View view(sf::Vector2f(0.f, 0.f), sf::Vector2f(VIEW_WIDHT, VIEW_WIDHT));
    sf::Color color(0,0,0);

    sf::Texture boardTexture, tokenBlackTexture, tokenWhiteTexture;
    sf::Sprite  board, tokenBlack, tokenWhite;

    sf::Sprite tokensArray[NUM_TILES][NUM_TILES];

    boardTexture.loadFromFile("board.gif");
    board.setTexture(boardTexture);
    board.setScale(sf::Vector2f(SCALE_TILE, SCALE_TILE));
    board.setOrigin(sf::Vector2f(0.f, 0.f));
    board.setPosition(sf::Vector2f(50.f, 100.f));

    tokenWhiteTexture.loadFromFile("Tokens.png", sf::IntRect(0, 0, 56, 56));
    tokenBlackTexture.loadFromFile("Tokens.png", sf::IntRect (56, 0, 112, 56));
    tokenWhite.setTexture(tokenWhiteTexture);
    tokenBlack.setTexture(tokenBlackTexture);

    //tokenBlack.setPosition(sf::Vector2f(55.f ,105.f + (DIMENSION_TILE * SCALE_TILE * 4)));

    for (size_t i=0; i<NUM_TILES; i++) {
        for (size_t j=0; j<NUM_TILES; j++) {
            tokensArray[i][j] = tokenWhite;
            tokensArray[i][j].setPosition(sf::Vector2f(55.f + (DIMENSION_TILE * SCALE_TILE * i),
                                            105.f + (DIMENSION_TILE * SCALE_TILE * j)));
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized)
                ResizeView(window, view);
        }
        window.clear(color);
        window.draw(board);
        for (size_t i=0; i<NUM_TILES; i++) {
            for (size_t j=0; j<NUM_TILES; j++) {
                window.draw(tokensArray[i][j]);
            }
        }
        //window.setView(view);
        window.display();
    }

    return 0;
}