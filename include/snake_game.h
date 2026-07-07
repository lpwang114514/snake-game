#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <vector>
#include <cstddef>

struct Position {
    int x;
    int y;
};

enum Direction { UP, DOWN, LEFT, RIGHT };

class SnakeGame {
public:
    SnakeGame();
    void run();

private:
    static constexpr int WIDTH = 28;
    static constexpr int HEIGHT = 24;

    std::vector<Position> snake;
    Position food;
    Direction dir;
    Direction nextDir;
    bool running;
    bool paused;
    int score;
    int speedMs;

    void reset();
    void spawnFood();
    bool isOccupy(const Position& p) const;
    bool isSelfCollision(const Position& nextHead, bool willEat) const;
    void handleInput();
    void update();
    void draw() const;
    void drawGameOver() const;
};

#endif
