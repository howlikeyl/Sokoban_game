
// copyright Senghak Heng

#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace SB {

enum class Direction {
    Up, Down, Left, Right
};

class Sokoban : public sf::Drawable {
 public:
    static const int TILE_SIZE = 64;

    Sokoban();
    explicit Sokoban(const std::string& filename);

    unsigned int height() const;
    unsigned int width() const;
    unsigned int pixelHeight() const;
    unsigned int pixelWidth() const;
    sf::Vector2u playerLoc() const;
    unsigned int getMoveCount() const;
    bool isWon() const;
    void movePlayer(Direction dir);
    void reset();
    void undo();
    void redo();

 protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
    std::vector<std::string> grid;
    std::vector<std::string> originalGrid;
    sf::Vector2u playerPosition;
    sf::Vector2u originalPlayerPosition;
    sf::Texture wallTexture, floorTexture, storageTexture;
    sf::Texture boxTexture, boxOnStorageTexture;
    sf::Texture playerUpTexture, playerDownTexture, playerLeftTexture, playerRightTexture;
    Direction lastDirection;
    unsigned int moveCount = 0;

    friend std::istream& operator>>(std::istream& in, Sokoban& s);
    friend std::ostream& operator<<(std::ostream& out, const Sokoban& s);
    struct Move {
      sf::Vector2u playerFrom;
      sf::Vector2u playerTo;
      std::optional<sf::Vector2u> boxFrom;
      std::optional<sf::Vector2u> boxTo;
      char boxPrevTile;
      char boxNewTile;
    };

    std::vector<Move> moveHistory;
    sf::SoundBuffer winBuffer;
    sf::Sound winSound;
    bool hasPlayedWinSound = false;
    std::vector<Move> redoHistory;
};
std::istream& operator>>(std::istream& in, Sokoban& s);
std::ostream& operator<<(std::ostream& out, const Sokoban& s);

}  // namespace SB
