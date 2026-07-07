#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <string>
#include <vector>

struct Position {
    int x;
    int y;
};

enum Direction { UP, DOWN, LEFT, RIGHT };
enum class GameState { MENU, PLAYING, PAUSED, GAME_OVER };

class SnakeGame {
public:
    SnakeGame();
    void run();

private:
    static constexpr int WIDTH = 32;
    static constexpr int HEIGHT = 24;
    static constexpr int INITIAL_SPEED_MS = 140;
    static constexpr int MIN_SPEED_MS = 55;

    std::vector<Position> snake;
    Position food;
    Direction dir;
    Direction nextDir;
    GameState state;
    bool exitRequested;
    int score;
    int bestScore;
    int speedMs;
    std::string highScoreFile;

    void reset();
    void loadBestScore();
    void saveBestScore() const;
    void spawnFood();
    bool isOccupy(const Position& p) const;
    bool isSelfCollision(const Position& nextHead, bool willEat) const;
    void handleMenuInput();
    void handlePlayInput();
    void handlePauseInput();
    void handleGameOverInput();
    void update();
    void draw() const;
    void drawMenu() const;
    void drawPause() const;
    void drawGameOver() const;
};

#endif
