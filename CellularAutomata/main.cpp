#include <iostream>
#include <array>
#include <thread>
#include <chrono>
#include <conio.h> // Windows specific.

const int WIDTH = 25, HEIGHT = 25;

namespace Color {
	static const std::string RESET = "\033[0m";
	static const std::string BLACK = "\033[40m";
	static const std::string RED = "\033[41m";
	static const std::string GREEN = "\033[42m";
	static const std::string YELLOW = "\033[43m";
	static const std::string BLUE = "\033[44m";
	static const std::string PURPLE = "\033[45m";
	static const std::string CYAN = "\033[46m";
	static const std::string WHITE = "\033[47m";
	static const std::string GRAY = "\033[100m";
};

std::string buffer;
std::array<std::array<int, WIDTH>, HEIGHT> board;
std::array<int, 2> cursorPosition = {(int)(WIDTH / 2), 1};

void printBoard() {
	buffer = "";
	buffer.reserve(WIDTH * 2 + HEIGHT);

	for (int y = 1; y <= HEIGHT; y++) {
		for (int x = 1; x <= WIDTH; x++) {
			if (x == cursorPosition[0] && y == cursorPosition[1]) {
				buffer += Color::RED;
				buffer += "  ";
				continue;
			}
			buffer += (board[y - 1][x - 1] == 0 ? Color::GRAY : Color::YELLOW);
			buffer += "  ";
		}
		buffer += "\n";
	}

	std::cout << "\033[H";
	std::cout << buffer << std::flush;
}

void updateBoard() {
	for (int y = HEIGHT - 2; y > -1; y--) {
		for (int x = 0; x < WIDTH; x++) {
			if (board[y][x] != 1 || board[y + 1][x] == 1) continue;

			board[y][x] = 0;
			board[y + 1][x] = 1;
		}
	}
}

void getInput() {
	if (_kbhit()) {
		switch (_getch()) {
		case 'w':
			if (cursorPosition[1] > 1) cursorPosition[1]--;
			break;
		case 'a':
			if (cursorPosition[0] > 1) cursorPosition[0]--;
			break;
		case 's':
			if (cursorPosition[1] < HEIGHT) cursorPosition[1]++;
			break;
		case 'd':
			if (cursorPosition[0] < WIDTH) cursorPosition[0]++;
			break;
		case ' ':
			board[cursorPosition[1] - 1][cursorPosition[0] - 1] = 1;
			break;
		}
	}
}

int main() {
	
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	// Debug
	board[0][0] = 1;
	board[1][12] = 1;
	board[0][17] = 1;
	board[3][15] = 1;
	board[7][15] = 1;

	// Initial board print.
	printBoard();
	std::this_thread::sleep_for(std::chrono::milliseconds(250));

	while (true) {
		getInput();
		updateBoard();
		printBoard();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	std::cout << Color::RESET;
	return 0;
}