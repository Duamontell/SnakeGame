#include <iostream>
#include <vector>
#include <conio.h>
#include <Windows.h>

// Константы
const int ROWS = 4;
const int COLS = 2;
const char EMPTY_CELL = '.';
const char SNAKE_BODY_CELL = 'O';
const char SNAKE_HEAD_CELL = '@';
const char APPLE_CELL = '*';
const int SLEEP_TIMER = 400;

struct Apple {
	int x, y;
	Apple(int x, int y) : x(x), y(y) {}
};

std::vector<std::vector<char>> createField() {
	return std::vector<std::vector<char>>(ROWS, std::vector<char>(COLS, EMPTY_CELL));
}

void printField(const std::vector<std::vector<char>>& field) {
	system("cls");
	for (const auto& row : field) {
		for (char cell : row) {
			std::cout << cell << ' ';
		}
		std::cout << '\n';
	}
}

void placeApple(std::vector<std::vector<char>>& field, const Apple& apple) {
	field[apple.x][apple.y] = APPLE_CELL;
}

void placeSnake(std::vector<std::vector<char>>& field, const std::vector<std::pair<int, int>>& snake) {
	if (!snake.empty()) {
		field[snake.front().first][snake.front().second] = SNAKE_HEAD_CELL;
		for (size_t i = 1; i < snake.size(); ++i) {
			field[snake[i].first][snake[i].second] = SNAKE_BODY_CELL;
		}
	}
}

Apple generateApple(const std::vector<std::pair<int, int>>& snake) {
	int x, y;
	do {
		x = rand() % ROWS;
		y = rand() % COLS;
	} while (std::find(snake.begin(), snake.end(), std::make_pair(x, y)) != snake.end());
	return Apple(x, y);
}

std::vector<std::pair<int, int>> generateSnake() {
	int x = rand() % ROWS;
	int y = rand() % COLS;
	// Сегмент головы и первый сегмент тела
	int nx = (x + 1 < ROWS) ? x + 1 : x - 1;
	return { {x, y}, {nx, y} };
}

// Движение змейки
bool moveSnake(std::vector<std::pair<int, int>>& snake, char direction, const Apple& apple, bool& ateApple) {
	int dx = 0, dy = 0;
	if (direction == 'w') dx = -1;
	if (direction == 's') dx = 1;
	if (direction == 'a') dy = -1;
	if (direction == 'd') dy = 1;

	int newX = snake.front().first + dx;
	int newY = snake.front().second + dy;

	if (newX < 0 || newX >= ROWS || newY < 0 || newY >= COLS) return false;

	if (std::find(snake.begin(), snake.end(), std::make_pair(newX, newY)) != snake.end()) return false;

	snake.insert(snake.begin(), { newX, newY });
	ateApple = (newX == apple.x && newY == apple.y);

	if (!ateApple) {
		snake.pop_back();
	}

	return true;
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	srand(static_cast<unsigned>(time(nullptr)));  // Используем стандартный rand

	Apple apple = generateApple({});
	auto snake = generateSnake();

	auto field = createField();

	bool gameRunning = true;
	bool ateApple = false;
	char direction = 'd';

	while (gameRunning) {
		field = createField();
		placeApple(field, apple);
		placeSnake(field, snake);
		printField(field);

		Sleep(SLEEP_TIMER);

		if (snake.size() == ROWS * COLS) {
			std::cout << "Вы победили!\n";
			break;
		}

		if (_kbhit()) {
			char newDirection = _getch();
			if ((direction == 'w' && newDirection != 's') ||
				(direction == 's' && newDirection != 'w') ||
				(direction == 'a' && newDirection != 'd') ||
				(direction == 'd' && newDirection != 'a')) {
				direction = newDirection;
			}
		}

		if (!moveSnake(snake, direction, apple, ateApple)) {
			std::cout << "Вы проиграли!\n";
			gameRunning = false;
			break;
		}

		if (ateApple) {
			apple = generateApple(snake);
		}

	}

	return 0;
}
