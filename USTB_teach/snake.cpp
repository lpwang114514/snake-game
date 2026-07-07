#include <iostream>
#include <vector>
#include <random>
#include <windows.h>
#include <conio.h>

using namespace std;

struct Position {
    int x;
    int y;
};

enum Direction { UP, DOWN, LEFT, RIGHT };

class SnakeGame {
private:
    static constexpr int WIDTH = 28;
    static constexpr int HEIGHT = 24;

    vector<Position> snake;
    Position food;
    Direction dir;
    Direction nextDir;
    bool running;
    bool paused;
    int score;
    int speedMs;

public:
    SnakeGame() {
        SetConsoleOutputCP(65001);
        SetConsoleCP(65001);
        reset();
    }

    void reset() {
        snake.clear();
        snake.push_back({WIDTH / 2, HEIGHT / 2});
        snake.push_back({WIDTH / 2 + 1, HEIGHT / 2});
        snake.push_back({WIDTH / 2 + 2, HEIGHT / 2});
        dir = LEFT;
        nextDir = LEFT;
        running = true;
        paused = false;
        score = 0;
        speedMs = 120;
        spawnFood();
    }

    void run() {
        while (running) {
            handleInput();
            if (!running) {
                break;
            }
            if (!paused) {
                update();
                draw();
                Sleep(speedMs);
            }
        }
        drawGameOver();
    }

private:
    void spawnFood() {
        static random_device rd;
        static mt19937 gen(rd());
        uniform_int_distribution<int> distX(1, WIDTH - 2);
        uniform_int_distribution<int> distY(1, HEIGHT - 2);

        do {
            food = {distX(gen), distY(gen)};
        } while (isOccupy(food));
    }

    bool isOccupy(const Position& p) const {
        for (const auto& seg : snake) {
            if (seg.x == p.x && seg.y == p.y) {
                return true;
            }
        }
        return false;
    }

    bool isSelfCollision(const Position& nextHead, bool willEat) const {
        if (snake.size() <= 1) {
            return false;
        }
        if (!willEat && snake.back().x == nextHead.x && snake.back().y == nextHead.y) {
            return false;
        }
        for (size_t i = 1; i < snake.size() - (willEat ? 0 : 1); ++i) {
            if (snake[i].x == nextHead.x && snake[i].y == nextHead.y) {
                return true;
            }
        }
        return false;
    }

    void handleInput() {
        if (!_kbhit()) {
            return;
        }

        int key = _getch();
        if (key == 27) {
            running = false;
            return;
        }

        if (key == 224) {
            key = _getch();
            switch (key) {
                case 72:
                    if (dir != DOWN) nextDir = UP;
                    break;
                case 80:
                    if (dir != UP) nextDir = DOWN;
                    break;
                case 75:
                    if (dir != RIGHT) nextDir = LEFT;
                    break;
                case 77:
                    if (dir != LEFT) nextDir = RIGHT;
                    break;
            }
        } else {
            switch (key) {
                case 'w':
                case 'W':
                    if (dir != DOWN) nextDir = UP;
                    break;
                case 's':
                case 'S':
                    if (dir != UP) nextDir = DOWN;
                    break;
                case 'a':
                case 'A':
                    if (dir != RIGHT) nextDir = LEFT;
                    break;
                case 'd':
                case 'D':
                    if (dir != LEFT) nextDir = RIGHT;
                    break;
                case ' ':
                    paused = !paused;
                    break;
                case 'r':
                case 'R':
                    reset();
                    break;
            }
        }
    }

    void update() {
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
            running = false;
            return;
        }

        snake.insert(snake.begin(), head);
        if (willEat) {
            score += 10;
            if (score % 50 == 0 && speedMs > 50) {
                speedMs -= 10;
            }
            spawnFood();
        } else {
            snake.pop_back();
        }
    }

    void draw() const {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD cursor = {0, 0};
        SetConsoleCursorPosition(hConsole, cursor);

        cout << "======================== 贪吃蛇 ========================\n";
        cout << "方向键/WASD 移动  空格暂停  R重开  Esc退出\n";
        cout << "得分: " << score << "\n";

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                    cout << "#";
                } else if (x == food.x && y == food.y) {
                    cout << "@";
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
                        cout << (isHead ? "O" : "o");
                    } else {
                        cout << " ";
                    }
                }
            }
            cout << '\n';
        }
    }

    void drawGameOver() const {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD cursor = {0, 0};
        SetConsoleCursorPosition(hConsole, cursor);

        cout << "======================== 贪吃蛇 ========================\n";
        cout << "游戏结束！\n";
        cout << "最终得分: " << score << "\n";
        cout << "按 R 重新开始，或直接关闭窗口。\n";
    }
};

int main() {
    SnakeGame game;
    game.run();
    return 0;
}

