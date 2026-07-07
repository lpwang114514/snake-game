#include "snake_game.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <windows.h>
#include <conio.h>

using namespace std;

namespace {
    bool isOpposite(Direction a, Direction b) {
        return (a == UP && b == DOWN) || (a == DOWN && b == UP) || (a == LEFT && b == RIGHT) || (a == RIGHT && b == LEFT);
    }
}

SnakeGame::SnakeGame() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    highScoreFile = "snake_highscore.txt";
    loadBestScore();
    state = GameState::MENU;
    exitRequested = false;
    reset();
}

void SnakeGame::reset() {
    snake.clear();
    snake.push_back({WIDTH / 2, HEIGHT / 2});
    snake.push_back({WIDTH / 2 - 1, HEIGHT / 2});
    snake.push_back({WIDTH / 2 - 2, HEIGHT / 2});
    dir = LEFT;
    nextDir = LEFT;
    score = 0;
    speedMs = INITIAL_SPEED_MS;
    spawnFood();
}

void SnakeGame::loadBestScore() {
    ifstream input(highScoreFile);
    if (input) {
        input >> bestScore;
    } else {
        bestScore = 0;
    }
}

void SnakeGame::saveBestScore() const {
    ofstream output(highScoreFile, ios::trunc);
    if (output) {
        output << bestScore << '\n';
    }
}

void SnakeGame::run() {
    while (!exitRequested) {
        switch (state) {
        case GameState::MENU:
            drawMenu();
            handleMenuInput();
            break;
        case GameState::PLAYING:
            handlePlayInput();
            if (exitRequested) {
                break;
            }
            if (state == GameState::PLAYING) {
                update();
                draw();
                Sleep(speedMs);
            }
            break;
        case GameState::PAUSED:
            drawPause();
            handlePauseInput();
            break;
        case GameState::GAME_OVER:
            drawGameOver();
            handleGameOverInput();
            break;
        }
    }
}

void SnakeGame::spawnFood() {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> distX(1, WIDTH - 2);
    uniform_int_distribution<int> distY(1, HEIGHT - 2);

    do {
        food = {distX(gen), distY(gen)};
    } while (isOccupy(food));
}

bool SnakeGame::isOccupy(const Position& p) const {
    for (const auto& seg : snake) {
        if (seg.x == p.x && seg.y == p.y) {
            return true;
        }
    }
    return false;
}

bool SnakeGame::isSelfCollision(const Position& nextHead, bool willEat) const {
    if (snake.size() <= 1) {
        return false;
    }
    size_t limit = snake.size() - (willEat ? 0 : 1);
    for (size_t i = 1; i < limit; ++i) {
        if (snake[i].x == nextHead.x && snake[i].y == nextHead.y) {
            return true;
        }
    }
    return false;
}

void SnakeGame::handleMenuInput() {
    if (!_kbhit()) {
        return;
    }

    int key = _getch();
    if (key == 27) {
        exitRequested = true;
        return;
    }

    if (key == 13 || key == ' ' || key == 'w' || key == 'W' || key == 'a' || key == 'A' || key == 's' || key == 'S' || key == 'd' || key == 'D') {
        reset();
        state = GameState::PLAYING;
    }
}

void SnakeGame::handlePlayInput() {
    if (!_kbhit()) {
        return;
    }

    int key = _getch();
    if (key == 27) {
        exitRequested = true;
        return;
    }

    if (key == ' ' || key == 'p' || key == 'P') {
        state = GameState::PAUSED;
        return;
    }

    if (key == 'r' || key == 'R') {
        reset();
        state = GameState::PLAYING;
        return;
    }

    Direction newDir = dir;
    if (key == 224) {
        key = _getch();
        switch (key) {
        case 72: newDir = UP; break;
        case 80: newDir = DOWN; break;
        case 75: newDir = LEFT; break;
        case 77: newDir = RIGHT; break;
        default: return;
        }
    } else {
        switch (key) {
        case 'w':
        case 'W': newDir = UP; break;
        case 's':
        case 'S': newDir = DOWN; break;
        case 'a':
        case 'A': newDir = LEFT; break;
        case 'd':
        case 'D': newDir = RIGHT; break;
        default: return;
        }
    }

    if (!isOpposite(dir, newDir)) {
        nextDir = newDir;
    }
}

void SnakeGame::handlePauseInput() {
    if (!_kbhit()) {
        return;
    }

    int key = _getch();
    if (key == 27) {
        exitRequested = true;
        return;
    }

    if (key == ' ' || key == 'p' || key == 'P' || key == 13) {
        state = GameState::PLAYING;
    } else if (key == 'r' || key == 'R') {
        reset();
        state = GameState::PLAYING;
    }
}

void SnakeGame::handleGameOverInput() {
    if (!_kbhit()) {
        return;
    }

    int key = _getch();
    if (key == 27) {
        exitRequested = true;
        return;
    }

    if (key == 13 || key == ' ' || key == 'r' || key == 'R') {
        reset();
        state = GameState::PLAYING;
    }
}

void SnakeGame::update() {
    dir = nextDir;
    Position head = snake.front();
    switch (dir) {
    case UP:    --head.y; break;
    case DOWN:  ++head.y; break;
    case LEFT:  --head.x; break;
    case RIGHT: ++head.x; break;
    }

    bool willEat = (head.x == food.x && head.y == food.y);
    if (head.x <= 0 || head.x >= WIDTH - 1 || head.y <= 0 || head.y >= HEIGHT - 1 || isSelfCollision(head, willEat)) {
        if (score > bestScore) {
            bestScore = score;
            saveBestScore();
        }
        state = GameState::GAME_OVER;
        return;
    }

    snake.insert(snake.begin(), head);
    if (willEat) {
        score += 10;
        if (score > bestScore) {
            bestScore = score;
            saveBestScore();
        }
        if (score % 50 == 0 && speedMs > MIN_SPEED_MS) {
            speedMs -= 5;
        }
        spawnFood();
    } else {
        snake.pop_back();
    }
}

void SnakeGame::draw() const {
    system("cls");
    cout << "======================== 贪吃蛇 ========================\n";
    cout << "方向键/WASD 移动  空格暂停  R重开  Esc退出\n";
    cout << "得分: " << score << "  最高分: " << bestScore << "  速度: " << (INITIAL_SPEED_MS - speedMs + INITIAL_SPEED_MS) / 20 << "\n";

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                cout << '#';
            } else if (x == food.x && y == food.y) {
                cout << '@';
            } else {
                bool isSnake = false;
                bool isHead = false;
                for (size_t i = 0; i < snake.size(); ++i) {
                    if (snake[i].x == x && snake[i].y == y) {
                        isSnake = true;
                        isHead = (i == 0);
                        break;
                    }
                }
                if (isSnake) {
                    cout << (isHead ? 'O' : 'o');
                } else {
                    cout << ' ';
                }
            }
        }
        cout << '\n';
    }
}

void SnakeGame::drawMenu() const {
    system("cls");
    cout << "======================== 贪吃蛇 ========================\n";
    cout << "欢迎来到超级贪吃蛇!\n";
    cout << "\n";
    cout << "按 Enter 或空格 开始游戏\n";
    cout << "方向键 / WASD 控制蛇身\n";
    cout << "空格暂停，R重新开始，Esc退出\n";
    cout << "\n";
    cout << "当前最高分: " << bestScore << "\n";
}

void SnakeGame::drawPause() const {
    system("cls");
    cout << "======================== 贪吃蛇 ========================\n";
    cout << "游戏暂停\n";
    cout << "按空格继续，R重新开始，Esc退出\n";
    cout << "得分: " << score << "\n";
}

void SnakeGame::drawGameOver() const {
    system("cls");
    cout << "======================== 贪吃蛇 ========================\n";
    cout << "游戏结束!\n";
    cout << "本局得分: " << score << "\n";
    cout << "历史最高分: " << bestScore << "\n";
    cout << "按 Enter / 空格 / R 重新开始，Esc退出\n";
}
