
// copyright Senghak Heng

#include <SFML/Graphics.hpp>
#include "Sokoban.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./Sokoban <level_file>\n";
        return 1;
    }

    SB::Sokoban game(argv[1]);

    sf::RenderWindow window(sf::VideoMode(game.pixelWidth(), game.pixelHeight() + 40), "Sokoban");

    sf::Font font;
    if (!font.loadFromFile("Roboto.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    sf::Text moveText;
    moveText.setFont(font);
    moveText.setCharacterSize(24);
    moveText.setFillColor(sf::Color::White);
    moveText.setPosition(10, game.pixelHeight() + 5);

    sf::Text winText;
    winText.setFont(font);
    winText.setCharacterSize(32);
    winText.setFillColor(sf::Color::Yellow);
    winText.setString("Congratulation You've won!");
    winText.setPosition(game.pixelWidth() / 2 - 80, game.pixelHeight() + 5);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (!game.isWon()) {
                    switch (event.key.code) {
                        case sf::Keyboard::Up:
                        case sf::Keyboard::W:
                            game.movePlayer(SB::Direction::Up);
                            break;
                        case sf::Keyboard::Down:
                        case sf::Keyboard::S:
                            game.movePlayer(SB::Direction::Down);
                            break;
                        case sf::Keyboard::Left:
                        case sf::Keyboard::A:
                            game.movePlayer(SB::Direction::Left);
                            break;
                        case sf::Keyboard::Right:
                        case sf::Keyboard::D:
                            game.movePlayer(SB::Direction::Right);
                            break;
                        case sf::Keyboard::Z:
                            game.undo();
                            break;
                        case sf::Keyboard::Y:
                            game.redo();
                            break;
                        default:
                            break;
                    }
                }
                if (event.key.code == sf::Keyboard::R) {
                    game.reset();
                }
            }
        }
        moveText.setString("Moves: " + std::to_string(game.getMoveCount()));
        window.clear();
        window.draw(game);
        window.draw(moveText);
        if (game.isWon()) {
            window.draw(winText);
        }
        window.display();
    }
    return 0;
}
