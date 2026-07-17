// copyright Senghak Heng

#include <fstream>
#include <sstream>
#include "Sokoban.hpp"

namespace SB {

Sokoban::Sokoban() {}

Sokoban::Sokoban(const std::string& filename) {
    std::ifstream file(filename);
    if (file) file >> *this;
}

unsigned int Sokoban::height() const { return grid.size(); }
unsigned int Sokoban::width() const { return grid.empty() ? 0 : grid[0].size(); }
unsigned int Sokoban::pixelHeight() const { return height() * TILE_SIZE; }
unsigned int Sokoban::pixelWidth() const { return width() * TILE_SIZE; }
sf::Vector2u Sokoban::playerLoc() const { return playerPosition; }
unsigned int Sokoban::getMoveCount() const { return moveCount; }

bool Sokoban::isWon() const {
    for (const auto& row : grid) {
        for (char c : row) {
            if (c == 'A') return false;  // Box not on target
        }
    }
    return true;  // No boxes off target
}

void Sokoban::movePlayer(Direction dir) {
    lastDirection = dir;

    int dx = 0, dy = 0;
    switch (dir) {
        case Direction::Up:    dy = -1; break;
        case Direction::Down:  dy =  1; break;
        case Direction::Left:  dx = -1; break;
        case Direction::Right: dx =  1; break;
    }

    int px = static_cast<int>(playerPosition.x);
    int py = static_cast<int>(playerPosition.y);
    int tx = px + dx;
    int ty = py + dy;

    if (tx < 0 || ty < 0 || ty >= static_cast<int>(grid.size()) ||
        tx >= static_cast<int>(grid[0].size())) return;

    char target = grid[ty][tx];

    auto isStorage = [](char c) { return c == 'a'; };
    auto isFloor   = [isStorage](char c) { return c == '.' || isStorage(c); };
    auto isWall    = [](char c) { return c == '#'; };
    auto isBox     = [](char c) { return c == 'A' || c == '1'; };

    if (isWall(target)) return;

    Move move;
    move.playerFrom = playerPosition;
    move.playerTo   = {static_cast<unsigned>(tx), static_cast<unsigned>(ty)};

    if (isBox(target)) {
        if (target == '1') return;

        int bx = tx + dx;
        int by = ty + dy;

        if (bx < 0 || by < 0 || by >= static_cast<int>(grid.size()) ||
            bx >= static_cast<int>(grid[0].size())) return;

        char beyond = grid[by][bx];
        if (!isFloor(beyond)) return;

        move.boxFrom     = {static_cast<unsigned>(tx), static_cast<unsigned>(ty)};
        move.boxTo       = {static_cast<unsigned>(bx), static_cast<unsigned>(by)};
        move.boxPrevTile = beyond;
        move.boxNewTile  = (isStorage(beyond) ? '1' : 'A');

        grid[by][bx] = move.boxNewTile;
        grid[ty][tx] = (grid[ty][tx] == '1' ? 'a' : '.');
    } else if (!isFloor(target)) {
        return;
    }

    grid[py][px] = (grid[py][px] == '1' ? 'a' : '.');
    grid[ty][tx] = '@';
    playerPosition = {static_cast<unsigned>(tx), static_cast<unsigned>(ty)};
    moveCount++;

    moveHistory.push_back(move);
    redoHistory.clear();

    if (isWon() && !hasPlayedWinSound) {
        winSound.play();
        hasPlayedWinSound = true;
    }
}

void Sokoban::undo() {
    if (moveHistory.empty()) return;

    Move last = moveHistory.back();
    moveHistory.pop_back();
    redoHistory.push_back(last);  // ✅ Save for redo

    grid[last.playerTo.y][last.playerTo.x] = (grid[last.playerTo.y][last.playerTo.x] ==
        '@' ? '.' : grid[last.playerTo.y][last.playerTo.x]);
    grid[last.playerFrom.y][last.playerFrom.x] = '@';
    playerPosition = last.playerFrom;

    if (last.boxFrom && last.boxTo) {
        grid[last.boxTo->y][last.boxTo->x] = last.boxPrevTile;
        grid[last.boxFrom->y][last.boxFrom->x] = (isWon() ? '1' : 'A');
    }

    if (moveCount > 0) moveCount--;
}

void Sokoban::redo() {
    if (redoHistory.empty()) return;

    Move next = redoHistory.back();
    redoHistory.pop_back();
    moveHistory.push_back(next);  // ✅ Restore to undo stack

    grid[next.playerFrom.y][next.playerFrom.x] = (grid[next.playerFrom.y][next.playerFrom.x] ==
        '@' ? '.' : grid[next.playerFrom.y][next.playerFrom.x]);
    grid[next.playerTo.y][next.playerTo.x] = '@';
    playerPosition = next.playerTo;

    if (next.boxFrom && next.boxTo) {
        grid[next.boxFrom->y][next.boxFrom->x] =
            (grid[next.boxFrom->y][next.boxFrom->x] == '1' ? 'a' : '.');
        grid[next.boxTo->y][next.boxTo->x] = next.boxNewTile;
    }

    moveCount++;
    if (isWon() && !hasPlayedWinSound) {
        winSound.play();
        hasPlayedWinSound = true;
    }
}

void Sokoban::reset() {
    grid = originalGrid;
    playerPosition = originalPlayerPosition;
    moveCount = 0;
    hasPlayedWinSound = false;
    moveHistory.clear();
    redoHistory.clear();
}

void Sokoban::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (unsigned int y = 0; y < height(); ++y) {
        for (unsigned int x = 0; x < width(); ++x) {
            sf::Sprite sprite;
            char tile = grid[y][x];
            sf::Vector2f pos(x * TILE_SIZE, y * TILE_SIZE);

            switch (tile) {
                case '#': sprite.setTexture(wallTexture); break;
                case '.': sprite.setTexture(floorTexture); break;
                case 'a': sprite.setTexture(storageTexture); break;
                case 'A': sprite.setTexture(boxTexture); break;
                case '1': sprite.setTexture(boxOnStorageTexture); break;
                case '@':
                    switch (lastDirection) {
                        case Direction::Up: sprite.setTexture(playerUpTexture); break;
                        case Direction::Down: sprite.setTexture(playerDownTexture); break;
                        case Direction::Left: sprite.setTexture(playerLeftTexture); break;
                        case Direction::Right: sprite.setTexture(playerRightTexture); break;
                    }
                    break;
                default: sprite.setTexture(floorTexture); break;
            }

            sprite.setPosition(pos);
            target.draw(sprite, states);
        }
    }
}

std::istream& operator>>(std::istream& in, Sokoban& s) {
    unsigned int h, w;
    in >> h >> w;
    in.ignore();

    s.grid.clear();
    s.grid.resize(h);

    for (unsigned int y = 0; y < h; ++y) {
        std::getline(in, s.grid[y]);
        for (unsigned int x = 0; x < s.grid[y].size(); ++x) {
            if (s.grid[y][x] == '@') {
                s.playerPosition = {x, y};
                s.originalPlayerPosition = {x, y};
            }
        }
    }

    s.originalGrid = s.grid;

    s.wallTexture.loadFromFile("sokoban/block_06.png");
    s.floorTexture.loadFromFile("sokoban/ground_01.png");
    s.storageTexture.loadFromFile("sokoban/ground_04.png");
    s.boxTexture.loadFromFile("sokoban/crate_03.png");
    s.boxOnStorageTexture.loadFromFile("sokoban/crate_03.png");

    s.playerUpTexture.loadFromFile("sokoban/player_08.png");
    s.playerDownTexture.loadFromFile("sokoban/player_05.png");
    s.playerLeftTexture.loadFromFile("sokoban/player_20.png");
    s.playerRightTexture.loadFromFile("sokoban/player_17.png");
    s.lastDirection = Direction::Down;

    s.winBuffer.loadFromFile("victory.wav");
    s.winSound.setBuffer(s.winBuffer);
    s.hasPlayedWinSound = false;


    return in;
}

std::ostream& operator<<(std::ostream& out, const Sokoban& s) {
    out << s.height() << " " << s.width() << "\n";
    for (const auto& row : s.grid) out << row << "\n";
    return out;
}
}  // namespace SB
