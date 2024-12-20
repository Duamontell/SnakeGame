#include <iostream>
#include <vector>
#include <conio.h>
#include <Windows.h>

const int  ROWS = 16;
const int  COLS = 8;
const char EMPTY_CELL = '.';
const char SNAKE_BODY_CELL = 'O';
const char SNAKE_HEAD_CELL = '@';
const char APPLE_CELL = '*';
const char TOP_DIRECTION = 'w';
const char LEFT_DIRECTION = 'a';
const char RIGHT_DIRECTION = 'd';
const char DOWN_DIRECTION = 's';
const int  SLEEP_TIMER = 400;

struct Apple {
	int x, y;
	Apple(int x, int y) : x(x), y(y) {}
};

std::vector<std::vector<char>> createField() {
	return std::vector<std::vector<char>>(ROWS, std::vector<char>(COLS, EMPTY_CELL));
}

std::string getFieldAsString(const std::vector<std::vector<char>>& field) {
	system("cls");
	std::string result;
	for (const auto& row : field) {
		for (char cell : row) {
			result += cell;
			result += ' ';
		}
		result += '\n';
	}
	return result;
}

std::vector<std::vector<char>> addAppleToField(const std::vector<std::vector<char>>& field, const Apple& apple) {
	auto updatedField = field;
	updatedField[apple.x][apple.y] = APPLE_CELL;
	return updatedField;
}

std::vector<std::vector<char>> addSnakeToField(const std::vector<std::vector<char>>& field, const std::vector<std::pair<int, int>>& snake) {
	auto updatedField = field;
	if (!snake.empty()) {
		updatedField[snake.front().first][snake.front().second] = SNAKE_HEAD_CELL;
		for (size_t i = 1; i < snake.size(); ++i) {
			updatedField[snake[i].first][snake[i].second] = SNAKE_BODY_CELL;
		}
	}
	return updatedField;
}

Apple generateApple(const std::vector<std::pair<int, int>>& snake) {
	if (snake.size() == ROWS * COLS) {
		return Apple(-1, -1);
	}
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

	int nx = (x + 1 < ROWS) ? x + 1 : x - 1;
	return { {x, y}, {nx, y} };
	/*if (ROWS > 1) {
		int nx = (x + 1 < ROWS) ? x + 1 : x - 1;
		return { {x, y}, {nx, y} };
	} else if (COLS > 1) {
		int ny = (y + 1 < COLS) ? y + 1 : y - 1;
		return { {x, y}, {x, ny} };
	}*/
}

bool moveSnake(std::vector<std::pair<int, int>>& snake, char direction, const Apple& apple, bool& ateApple) {
	int dx = 0, dy = 0;
	if (direction == TOP_DIRECTION) dx = -1;
	if (direction == DOWN_DIRECTION) dx = 1;
	if (direction == LEFT_DIRECTION) dy = -1;
	if (direction == RIGHT_DIRECTION) dy = 1;

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

	srand(static_cast<unsigned>(time(0)));

	Apple apple = generateApple({});
	std::vector<std::pair<int, int>> snake = generateSnake();
	auto field = createField();

	bool gameRunning = true;
	bool ateApple = false;
	char direction = 'd';

	while (gameRunning) {
		field = createField();
		field = addAppleToField(field, apple);
		field = addSnakeToField(field, snake);
		std::cout << getFieldAsString(field);

		Sleep(SLEEP_TIMER);

		if (snake.size() == ROWS * COLS) {
			std::cout << "Вы победили!\n";
			break;
		}

		if (_kbhit()) {
			char newDirection = _getch();
			if ((direction == TOP_DIRECTION && newDirection != DOWN_DIRECTION) ||
				(direction == DOWN_DIRECTION && newDirection != TOP_DIRECTION) ||
				(direction == LEFT_DIRECTION && newDirection != RIGHT_DIRECTION) ||
				(direction == RIGHT_DIRECTION && newDirection != LEFT_DIRECTION)) {
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
			if (apple.x == -1 && apple.y == -1) {
				std::cout << "Вы победили!\n";
				break;
			}
		}
	}

	return 0;
}