# SnakeGame

一个完整度较高的控制台贪吃蛇小游戏，包含开始界面、暂停、重新开始、最高分记录以及随着得分提升的速度变化。

## 功能特点

- 开始界面与游戏结束界面
- 使用 WASD 或方向键控制
- 空格键暂停/继续
- R 键重新开始
- 记录历史最高分
- 每得 50 分提升一次速度

## 运行方式

### Windows

在项目根目录执行：

```powershell
g++ src\main.cpp src\snake_game.cpp -Iinclude -o snake.exe
.\snake.exe
```

## 操作说明

- W / A / S / D 或方向键：移动
- 空格：暂停/继续
- R：重新开始
- Esc：退出
